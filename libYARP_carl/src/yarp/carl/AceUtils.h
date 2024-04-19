
#ifndef __YARP_CARL_ACEUTILS__
#define __YARP_CARL_ACEUTILS__

#include <yarp/carl/api.h>
//#include "ace/os_include/os_time.h"
#include <yarp/os/impl/PlatformTime.h>

#include <yarp/os/ConstString.h>

namespace yarp {
	namespace carl {
		class MysqlTimestamp;
	}
}

// Converts MySqlTimestamps TimeStamp Info 
class YARP_carl_API yarp::carl::MysqlTimestamp {
	int m_timezone;  // of the underlying MySQL Session, default is local timezone which corresponds to standard MySQL configuration
	int m_ltz; // Local Timezone of the System
	char m_timestamp[23+1]; // 1970.01.01 00:00:00.000    --> faction seconds are ms, no timezone	
	ACE_Time_Value m_time_value; // high precsion value automatically set at any conversion
	int m_lglvl; 
public:
	static const int LocalTimezone();		// LocalTimezone without DST, to be used for internal calculations based on ctime.h, for instance localtime and gmttime
	static const int LocalTimezoneDst();	// LocalTimezone with DST (e.g. UTC+01,Berlin in Summer => ISO6801-Timezone = +02:00
	MysqlTimestamp(int lglvl = 1); // initializes timezone to local timezone 
	MysqlTimestamp(int timezone, int lglvl); // sets timezone of MySQL Session (in seconds)
	const ACE_Time_Value& from(const char* sql_timestamp); // 1:1 copies to m_timestamp and sets m_time_value (timezone must match the session) 
	const char* convert(const char *iso_timestamp);
	const char* convert(const unsigned long long seconds, int ms, int timezone);
	const char* c_str() { return m_timestamp; } // %Y-%m-%d %X.%3f  (timezone is omitted)
	const ACE_Time_Value& ace_time_value() { return m_time_value; }
	yarp::os::ConstString iso_timestamp();
	const unsigned long long ms_utc() { return m_time_value.get_msec() - m_timezone * 1000L; }
};


#endif