#ifndef HV_BUFFER_HPP_
#define HV_BUFFER_HPP_

#include <memory>

#include "base/hbuf.h"

namespace hv {

typedef HBuf Buffer;
typedef std::shared_ptr<Buffer>     BufferPtr;

}

#endif // HV_BUFFER_HPP_
