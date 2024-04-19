#include <ctime>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <ace/os_include/os_time.h>
#include <yarp/os/impl/PlatformTime.h>
using namespace yarp::os;

#include <yarp/carl/BottleDecoder.h>
using namespace yarp::carl;

using namespace yarp::os::impl;


BottleDecoder::BottleDecoder(Bottle& msg, int timezone, int lglvl): m_msg(msg) 
{
	m_iso_timestamp = new IsoTimestamp(timezone, NULL, lglvl); 
	m_mysql_timestamp = new MysqlTimestamp(timezone, lglvl);  
}

BottleDecoder::~BottleDecoder()
{
	delete m_iso_timestamp;
	delete m_mysql_timestamp;
}