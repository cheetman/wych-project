#ifndef HV_TCP_CLIENT_HPP_
#define HV_TCP_CLIENT_HPP_

#include "base/hsocket.h"
#include "ssl/hssl.h"
#include "base/hlog.h"

#include "EventLoopThread.h"
#include "Channel.h"

namespace hv {

typedef struct reconn_setting_s ReconnectInfo; // Deprecated

template<class TSocketChannel = SocketChannel>
class TcpClientTmpl {
public:
    typedef std::shared_ptr<TSocketChannel> TSocketChannelPtr;

    TcpClientTmpl() {
        tls = false;
        connect_timeout = 5000;
        reconn_setting = NULL;
        unpack_setting = NULL;
    }

    virtual ~TcpClientTmpl() {
        HV_FREE(reconn_setting);
        HV_FREE(unpack_setting);
    }

    const EventLoopPtr& loop() {
        return loop_thread.loop();
    }

    //@retval >=0 connfd, <0 error
    int createsocket(int port, const char* host = "127.0.0.1") {
        memset(&peeraddr, 0, sizeof(peeraddr));
        int ret = sockaddr_set_ipport(&peeraddr, host, port);
        if (ret != 0) {
            return -1;
        }
        return createsocket(&peeraddr.sa);
    }
    int createsocket(struct sockaddr* peeraddr) {
        int connfd = socket(peeraddr->sa_family, SOCK_STREAM, 0);
        // SOCKADDR_PRINT(peeraddr);
        if (connfd < 0) {
            perror("socket");
            return -2;
        }

        hio_t* io = hio_get(loop_thread.hloop(), connfd);
        assert(io != NULL);
        hio_set_peeraddr(io, peeraddr, SOCKADDR_LEN(peeraddr));
        channel.reset(new TSocketChannel(io));
        return connfd;
    }
    // closesocket thread-safe
    void closesocket() {
        setReconnect(NULL);
        if (channel) {
            channel->close(true);
        }
    }

    int startConnect() {
        assert(channel != NULL);
        if (tls) {
            channel->enableSSL();
        }
        if (connect_timeout) {
            channel->setConnectTimeout(connect_timeout);
        }
        channel->onconnect = [this]() {
            if (unpack_setting) {
                channel->setUnpack(unpack_setting);
            }
            channel->startRead();
            if (onConnection) {
                onConnection(channel);
            }
            if (reconn_setting) {
                reconn_setting_reset(reconn_setting);
            }
        };
        channel->onread = [this](Buffer* buf) {
            if (onMessage) {
                onMessage(channel, buf);
            }
        };
        channel->onwrite = [this](Buffer* buf) {
            if (onWriteComplete) {
                onWriteComplete(channel, buf);
            }
        };
        channel->onclose = [this]() {
            if (onConnection) {
                onConnection(channel);
            }
            // reconnect
            if (reconn_setting) {
                startReconnect();
            } else {
                channel = NULL;
                // NOTE: channel should be destroyed,
                // so in this lambda function, no code should be added below.
            }
        };
        return channel->startConnect();
    }

    int startReconnect() {
        if (!reconn_setting) return -1;
        if (!reconn_setting_can_retry(reconn_setting)) return -2;
        uint32_t delay = reconn_setting_calc_delay(reconn_setting);
        loop_thread.loop()->setTimeout(delay, [this](TimerID timerID){
            hlogi("reconnect... cnt=%d, delay=%d", reconn_setting->cur_retry_cnt, reconn_setting->cur_delay);
            createsocket(&peeraddr.sa);
            startConnect();
        });
        return 0;
    }

    void start(bool wait_threads_started = true) {
        loop_thread.start(wait_threads_started, std::bind(&TcpClientTmpl::startConnect, this));
    }
    // stop thread-safe
    void stop(bool wait_threads_stopped = true) {
        setReconnect(NULL);
        loop_thread.stop(wait_threads_stopped);
    }

    bool isConnected() {
        if (channel == NULL) return false;
        return channel->isConnected();
    }

    // send thread-safe
    int send(const void* data, int size) {
        if (!isConnected()) return -1;
        return channel->write(data, size);
    }
    int send(Buffer* buf) {
        return send(buf->data(), buf->size());
    }
    int send(const std::string& str) {
        return send(str.data(), str.size());
    }

    // deprecated: use withTLS(opt) after createsocket
    int withTLS(const char* cert_file = NULL, const char* key_file = NULL, bool verify_peer = false) {
        if (cert_file) {
            hssl_ctx_init_param_t param;
            memset(&param, 0, sizeof(param));
            param.crt_file = cert_file;
            param.key_file = key_file;
            param.verify_peer = verify_peer ? 1 : 0;
            param.endpoint = HSSL_CLIENT;
            if (hssl_ctx_init(&param) == NULL) {
                fprintf(stderr, "hssl_ctx_init failed!\n");
                return -1;
            }
        }
        tls = true;
        return 0;
    }
    int withTLS(hssl_ctx_opt_t* opt) {
        if (!channel) return -1;
        return channel->newSslCtx(opt);
    }

    void setConnectTimeout(int ms) {
        connect_timeout = ms;
    }

    void setReconnect(reconn_setting_t* setting) {
        if (setting == NULL) {
            HV_FREE(reconn_setting);
            return;
        }
        if (reconn_setting == NULL) {
            HV_ALLOC_SIZEOF(reconn_setting);
        }
        *reconn_setting = *setting;
    }
    bool isReconnect() {
        return reconn_setting && reconn_setting->cur_retry_cnt > 0;
    }

    void setUnpack(unpack_setting_t* setting) {
        if (setting == NULL) {
            HV_FREE(unpack_setting);
            return;
        }
        if (unpack_setting == NULL) {
            HV_ALLOC_SIZEOF(unpack_setting);
        }
        *unpack_setting = *setting;
    }

public:
    TSocketChannelPtr       channel;

    sockaddr_u              peeraddr;
    bool                    tls;
    int                     connect_timeout;
    reconn_setting_t*       reconn_setting;
    unpack_setting_t*       unpack_setting;

    // Callback
    std::function<void(const TSocketChannelPtr&)>           onConnection;
    std::function<void(const TSocketChannelPtr&, Buffer*)>  onMessage;
    // NOTE: Use Channel::isWriteComplete in onWriteComplete callback to determine whether all data has been written.
    std::function<void(const TSocketChannelPtr&, Buffer*)>  onWriteComplete;

private:
    EventLoopThread         loop_thread;
};

typedef TcpClientTmpl<SocketChannel> TcpClient;

}

#endif // HV_TCP_CLIENT_HPP_