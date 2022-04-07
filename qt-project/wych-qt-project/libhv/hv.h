#ifndef HV_H_
#define HV_H_

/**
 * @copyright 2018 HeWei, all rights reserved.
 */

// platform
#include "hconfig.h"
#include "hexport.h"
#include "base/hplatform.h"

// c
#include "base/hdef.h"   // <stddef.h>
#include "base/hatomic.h"// <stdatomic.h>
#include "base/herr.h"   // <errno.h>
#include "base/htime.h"  // <time.h>
#include "base/hmath.h"  // <math.h>

#include "base/hbase.h"
#include "base/hversion.h"
#include "base/hsysinfo.h"
#include "base/hproc.h"
#include "base/hthread.h"
#include "base/hmutex.h"
#include "base/hsocket.h"

#include "base/hlog.h"
#include "base/hbuf.h"

// cpp
#ifdef __cplusplus
#include "cpputil/hmap.h"       // <map>
#include "cpputil/hstring.h"    // <string>
#include "cpputil/hfile.h"
#include "cpputil/hpath.h"
#include "cpputil/hdir.h"
#include "cpputil/hurl.h"
#endif

#endif  // HV_H_
