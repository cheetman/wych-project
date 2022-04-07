#ifndef LIBHV_H
#define LIBHV_H

class Libhv
{
public:
    Libhv();
    bool start(int port, const char* host);
};

#endif // LIBHV_H
