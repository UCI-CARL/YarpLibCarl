

#ifndef _YARP2_carl_API_
#define _YARP2_carl_API_

#include <yarp/conf/api.h>
#ifndef YARP_carl_API
#  ifdef YARP_carl_EXPORTS
#    define YARP_carl_API YARP_EXPORT
#  else
#    define YARP_carl_API YARP_IMPORT
#  endif
#endif

#endif
