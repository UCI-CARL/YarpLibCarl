#ifndef __YARP_CARL_BOTTLEDECODER__
#define __YARP_CARL_BOTTLEDECODER__

#include <yarp/os/Bottle.h>
#include <yarp/os/impl/PlatformTime.h>
using namespace yarp::os;

#include <yarp/carl/api.h>
#include <yarp/carl/StlUtils.h>
#include <yarp/carl/AceUtils.h>

#include <carl/boost_ext.hpp>

namespace yarp {
	namespace carl {
		class BottleDecoder;
	}
}

class YARP_carl_API yarp::carl::BottleDecoder
{
protected:
	Bottle &m_msg; 
	int m_timezone;
	IsoTimestamp *m_iso_timestamp;  
	MysqlTimestamp *m_mysql_timestamp;
public:
	BottleDecoder(Bottle& msg, int timezone, int lglvl=4); // Timezone of MySQL session
	~BottleDecoder();

	//virtual ENUM_BOTTLE_MSG_TYPE msg_type();

};

#endif