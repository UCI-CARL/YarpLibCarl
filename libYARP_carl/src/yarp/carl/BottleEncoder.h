#ifndef __YARP_CARL_BOTTLEENCODER__
#define __YARP_CARL_BOTTLEENCODER__

#include <yarp/carl/api.h>
#include <yarp/carl/StlUtils.h>
#include <yarp/carl/AceUtils.h>

#include <yarp/os/Bottle.h>
#include <yarp/os/Property.h>
#include <yarp/os/impl/PlatformTime.h>
using namespace yarp::os;

#include <carl/boost_ext.hpp>

namespace yarp {
	namespace carl {
		class BottleEncoder;

	}
}

class YARP_carl_API yarp::carl::BottleEncoder
{
protected:
	int m_timezone;
	yarp::carl::IsoTimestamp *m_timestamp;  
	Bottle &m_msg; 
public:
	BottleEncoder(Bottle& msg, int timezone, int lglvl=4, std::ofstream* logfile=NULL);
	~BottleEncoder();
};



#endif