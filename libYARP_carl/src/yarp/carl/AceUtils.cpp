
#include <yarp/carl/AceUtils.h>
using namespace yarp::carl;

#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include <time.h> // see LocalTimezone

#include <ace/os_include/os_time.h>
#include <yarp/os/impl/PlatformTime.h>
using namespace yarp::os;


const int MysqlTimestamp::LocalTimezone()
{
	time_t rawtime;
	time(&rawtime);
	time_t lt = mktime(localtime(&rawtime));
	time_t gmt = mktime(gmtime(&rawtime));
	int ltz = (int) (lt - gmt);
	

	return ltz;
}

const int MysqlTimestamp::LocalTimezoneDst()
{
	_tzset();
    int daylight;
    _get_daylight( &daylight );
    long timezone;
    _get_timezone( &timezone );

	return (-1)*timezone+daylight*3600;
}




MysqlTimestamp::MysqlTimestamp(int lglvl): m_lglvl(lglvl)
{
	m_timestamp[0] = '\0';
	m_ltz = LocalTimezone(); // in seconds
	m_timezone = m_ltz;
 
	if(m_lglvl>=4)
		cout << __FUNCTION__ << " timezone:" << m_timezone << "\n";
}

MysqlTimestamp::MysqlTimestamp(int timezone, int lglvl): m_timezone(timezone), m_lglvl(lglvl) 
{
	m_ltz = LocalTimezone(); 
	m_timestamp[0] = '\0'; 
}


const ACE_Time_Value& MysqlTimestamp::from(const char* sql_timestamp)
{
	if(m_lglvl>=4) cout << __FUNCTION__<< "\n";

	if(m_lglvl>=4) cout << " sql:" << sql_timestamp << "\n";

	time_t rawtime = 0;
	struct tm* t;
	t = localtime ( &rawtime );


	unsigned int ms; 
	sscanf_s(sql_timestamp, "%4d-%2d-%2d %2d:%2d:%2d.%3d", &t->tm_year, &t->tm_mon, &t->tm_mday, &t->tm_hour, &t->tm_min, &t->tm_sec, &ms); 
	if(m_lglvl>=4) cout << "sscanf: ";
	if(m_lglvl>=4) printf("%04d-%02d-%02d %02d:%02d:%02d.%03d\n", t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ms);


	t->tm_year -= 1900;  
	t->tm_mon -= 1;

	time_t local = mktime(t); // seconds in localtime (without DST)
	if(local == -1) {
		if(m_lglvl>=1) printf("mktime failed\n");
		throw "mktime failed";
	}
	if(m_lglvl>=4) cout << "(time_t) local: " << local << "\n";

	local += m_ltz;

	m_time_value.set(local, ms*1000);
	if(m_lglvl>=4) printf(" ace:%llums", (unsigned long long) m_time_value.get_msec());		

	return m_time_value;
}

const char* MysqlTimestamp::convert(const unsigned long long seconds, const int ms, const int timezone)
{
	time_t time = seconds;
	time = m_timezone - timezone; // to localtime
	struct tm* timeinfo = localtime(&time); // localtime
	size_t n = strftime(m_timestamp, 24, "%Y-%m-%d %X.3f", timeinfo); 
	if(n!=24)
	{
		if(m_lglvl>=1) cerr << "Error:" << n << "\n";
		return "";
	}	
	m_time_value.set(time, ms*1000);
	return m_timestamp; 
}


const char* MysqlTimestamp::convert(const char *iso_timestamp) 
{
	if(m_lglvl>=4) cout << "MysqlTimestamp::convert" << "\n";
	if(m_lglvl>=4) cout << __FUNCTION__;
	if(m_lglvl>=4) cout << " iso_timestamp:" << iso_timestamp << "\n";

	struct tm t, *timeinfo;
	int ms, tz_hours, tz_minutes, tz;
	
	if(strlen(iso_timestamp) != 29)
	{
		m_timestamp[0] = '\0'; 
		if(m_lglvl>=1) cout << "Exception: iso_timestamp in wrong format\n";
		return ""; 
	}

	sscanf_s(iso_timestamp, "%4d-%2d-%2dT%2d:%2d:%2d.%3d%3d:%2d", 
		&t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec, &ms, &tz_hours, &tz_minutes);  

	t.tm_isdst = 0;  // no dayligh saving for internal processing 
	t.tm_year -= 1900;  
	t.tm_mon -= 1; 
	time_t local = mktime(&t); // 
	if(local == -1) 
		if(m_lglvl>=1) printf("gmtime failed\n"); 
	if(m_lglvl>=4) cout << "(time_t) local: " << local << "\n";
	tz = tz_hours*3600+tz_minutes*60;
	if(m_lglvl>=4) cout << "tz:" << tz << "\n";
	local = local - tz + MysqlTimestamp::LocalTimezone() + m_timezone;		

	timeinfo = gmtime(&local);
	char stime[20];
	size_t n = strftime(stime, 20, "%Y-%m-%d %X", timeinfo); // yyyy-mm-dd hh:mm:ss"
	sprintf(m_timestamp, "%s.%03d", stime, ms);
	if(m_lglvl>=4) cout << m_timestamp << "\n";

	m_time_value.set(local, ms*1000);
	if(m_lglvl>=4) printf("ace:%llums\n", (unsigned long long) m_time_value.get_msec());

	return m_timestamp; 
}


ConstString MysqlTimestamp::iso_timestamp()
{
	time_t rawtime = m_time_value.sec();
	errno_t err;
	struct tm timeinfo;
	err = gmtime_s(&timeinfo, &rawtime);	
	char iso1[19+1];
	size_t n = strftime(iso1, 20, "%Y-%m-%dT%X", &timeinfo); // yyyy-mm-ddThh:mm:ss"
	if(m_lglvl>=4) cout << iso1 << "\n";
	char iso2[29+1]; 
    long long ms =  m_time_value.get_msec();
	sprintf(iso2, "%s.%03d%s%02d:%02d", iso1, (unsigned int) (ms%1000), m_timezone>=0?"+":"", m_timezone/3600, m_timezone/60%60);
	if(m_lglvl>=4) cout << iso2 << "\n";
	return ConstString(iso2);
}
