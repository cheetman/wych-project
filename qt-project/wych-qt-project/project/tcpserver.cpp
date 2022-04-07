#include "tcpserver.h"
#include "uv.h"




TcpServer* m_Instance = nullptr;



TcpServer::TcpServer(QObject *parent)
    : QObject{parent}
{


}

TcpServer::~TcpServer()
{

}

TcpServer* TcpServer::getInstance()
{
  if(m_Instance == nullptr)
  {
    m_Instance = new TcpServer();
  }
  return m_Instance;
}

///**
// *
// * @param:  server  libuv的tcp server对象
// * @param:  status  状态，小于0表示新连接有误
// * @author: sherlock
// */
//void TcpServer::on_new_connection(uv_stream_t *server, int status) {
//  if (status < 0) {
//    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
//    return;
//  }

//  uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));//为tcp client申请资源

////  auto loop = uv_default_loop();
//  uv_tcp_init(uv_default_loop(), client);//初始化tcp client句柄

//  //判断accept是否成功
//  if (uv_accept(server, (uv_stream_t *) client) == 0) {
//    //从传入的stream中读取数据，read_cb会被多次调用，直到数据读完，或者主动调用uv_read_stop方法停止
//    uv_read_start((uv_stream_t *) client, TcpServer::alloc_buffer, TcpServer::read_cb);
//  } else {
//    uv_close((uv_handle_t *) client, NULL);
//  }
//}


//int TcpServer::start()
//{
//    struct sockaddr_in addr;
//    uv_tcp_init(uv_default_loop(), &tcpServer);
//    uv_ip4_addr("0.0.0.0",9402,&addr);
//    uv_tcp_bind(&tcpServer,(const struct sockaddr*)&addr,0);
//    int r = uv_listen((uv_stream_t * )&tcpServer,128,TcpServer::on_new_connection);
//    if(r){
//        qDebug() << "Listen error " << uv_strerror(r);
//    }
//    uv_run(uv_default_loop(),UV_RUN_DEFAULT);

//}





////负责为新来的消息申请空间
//void TcpServer::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
//  buf->len = suggested_size;
//  buf->base = static_cast<char *>(malloc(suggested_size));
//}

//void TcpServer::on_close(uv_handle_t *handle) {
//  if (handle != NULL)
//    free(handle);
//}

//void TcpServer::echo_write(uv_write_t *req, int status) {
//  if (status) {
//    fprintf(stderr, "Write error %s\n", uv_strerror(status));
//  }

//  free(req);
//}

///**
// * @brief: 负责处理新来的消息
// * @param: client
// * @param: nread>0表示有数据就绪，nread<0表示异常，nread是有可能为0的，但是这并不是异常或者结束
// * @author: sherlock
// */
//void TcpServer::read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
//  if (nread > 0) {
////    buf->base[nread] = 0;
//    fprintf(stdout, "recv:%s\n", buf->base);
//    fflush(stdout);

//    uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));

//    uv_buf_t uvBuf = uv_buf_init(buf->base, nread);//初始化write的uv_buf_t

//    //发送buffer数组，第四个参数表示数组大小
//    uv_write(req, client, &uvBuf, 1, TcpServer::echo_write);

//    return;
//  } else if (nread < 0) {
//    if (nread != UV_EOF) {
//      fprintf(stderr, "Read error %s\n", uv_err_name(nread));
//    } else {
//      fprintf(stderr, "client disconnect\n");
//    }
//    uv_close((uv_handle_t *) client, TcpServer::on_close);
//  }

//  //释放之前申请的资源
//  if (buf->base != NULL) {
//    free(buf->base);
//  }
//}

