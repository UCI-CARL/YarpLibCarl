#include <yarp/carl/BottleEncoder.h>
using namespace yarp::carl;

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <ace/os_include/os_time.h>
#include <yarp/os/impl/PlatformTime.h>
using namespace yarp::os;

#include <boost/container/vector.hpp>
#include <boost/container/string.hpp>

BottleEncoder::BottleEncoder(Bottle& msg, int timezone, int loglvl, ofstream* logfile): m_msg(msg), m_timezone(timezone) 
	{
		m_timestamp = new yarp::carl::IsoTimestamp(m_timezone, logfile, loglvl); 
	}


BottleEncoder::~BottleEncoder() 
{
	delete m_timestamp; 
}

