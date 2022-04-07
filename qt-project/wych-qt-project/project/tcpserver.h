#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include "uv.h"

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    static TcpServer* getInstance();
    virtual ~TcpServer();
    void startServer();

    void on_new_connection(uv_stream_t *server, int status) ;
    int start();
    void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    void echo_write(uv_write_t *req, int status);
    void on_close(uv_handle_t *handle);
    void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);


private:
    uv_tcp_t tcpServer;

    uv_loop_t* loop ;

signals:

};

#endif // TCPSERVER_H
