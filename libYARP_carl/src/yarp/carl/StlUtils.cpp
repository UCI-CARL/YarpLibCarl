
#include <yarp/carl/StlUtils.h>
using namespace yarp::carl;

#include <ctime>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;


const char* IsoTimestamp::convert(unsigned long long seconds, unsigned int ms)  // seconds,ms are in timezone
{
	time_t time = seconds; 

	//errno_t err;
	int err = 0;
	if(m_dll_log && m_lglvl>=4) *m_dll_log << "seconds: " << time << "\n";
	struct tm timeinfo;

//	err = gmtime_s(&timeinfo, &time); 
	
	if(err!=0)
	{
		if(m_dll_log && m_lglvl>=1) *m_dll_log << "err:" << err << "\n";
		return "";
	}
	char s_time[19+1];
	size_t n = strftime(s_time, 19+1, "%Y-%m-%dT%X", &timeinfo); // ISO part 1: yyyy-mm-ddThh:mm:ss 
	if(m_dll_log && m_lglvl>=4) *m_dll_log << "s_time:" << s_time << "\n"; 
	//n = sprintf_s(m_timestamp,29+1, "%s.%03d%s%02d:%02d", s_time, ms, m_timezone>=0?"+":"", m_timezone/3600, m_timezone/60%60);
	sprintf(m_timestamp, "%s.%03d%s%02d:%02d", s_time, ms, m_timezone>=0?"+":"", m_timezone/3600, m_timezone/60%60);
	if(m_dll_log && m_lglvl>=4) *m_dll_log << "ISO Timestamp:"<<m_timestamp<<"\n";

	return m_timestamp; 
}


const char* IsoTimestamp::c_str() { return m_timestamp; };


