
#ifndef __YARP_CARL_STLUTILS__
#define __YARP_CARL_STLUTILS__

#include <fstream>

#include <yarp/carl/api.h>


namespace yarp { 
	namespace carl {
		class IsoTimestamp;	
	}
}

/* Converts a datetimes and ms to an ISO Timestamps, with the corresponding timezone instanciated once in the object.
 * The string value can be used after the conversion with the c_str method.
 */
class YARP_carl_API yarp::carl::IsoTimestamp {
	char m_timestamp[30];
	int m_timezone;
	std::ofstream* m_dll_log;
	int m_lglvl;
public:
	IsoTimestamp(int timezone, std::ofstream* dll_log=NULL, int lglvl = 0): 
	m_timezone(timezone), m_dll_log(dll_log), m_lglvl(lglvl) { m_timestamp[0] = '\0'; }
	const char* convert(unsigned long long seconds, unsigned int ms);
	const char* c_str();
};


#endif