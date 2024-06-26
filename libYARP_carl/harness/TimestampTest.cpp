
#include <yarp/sig/Matrix.h>
#include <yarp/os/impl/BufferedConnectionWriter.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Port.h>
#include <yarp/os/Time.h>
#include <yarp/os/DummyConnector.h>
#include <yarp/os/PortablePair.h>

#include <math.h>
#include <vector>

#include "TestList.h"

#include <yarp/carl/all.h>

using namespace yarp::os::impl;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::carl;

class TimestampTest : public UnitTest {

public:
    virtual ConstString getName() { return "TimestampTest"; }

    void checkIsoTimestampBasic()
    {	
		report(0,"Checking ISO-Timestamps for several timezones...");  

		unsigned long long seconds = 10*3600+31*60+25;
		int ms = 123;
        IsoTimestamp iso1(3600);	
        IsoTimestamp iso2(7200);	
        IsoTimestamp iso0(0);		// UTC
		
		checkEqual(iso1.convert(seconds,ms), "1970-01-01T10:31:25.123+01:00", "+01:00 Berlin,Bern,.. in Winter without no DLZ)");
		checkEqual(iso2.convert(seconds,ms), "1970-01-01T10:31:25.123+02:00", "+02:00 UK");
		checkEqual(iso0.convert(seconds,ms), "1970-01-01T10:31:25.123+00:00", "+00:00 UTC (Coordinated Univesial Time)");
    }

	void checkMysqlTimestampBasic()
	{
		report(0,"Checking Mysql-Timestamps...");  

		int ltz = MysqlTimestamp::LocalTimezone();
		checkEqual(ltz, 3600, "Local Timezone without DST is +01:00");
		ltz = MysqlTimestamp::LocalTimezoneDst();
		checkEqual(ltz, 7200, "Local Timezone UTC+01,Berlin => with DST is +02:00 in Summer");

		// ltz to 3600
		ltz = 3600;
		MysqlTimestamp ltz_mysql(ltz,1); 
		//MysqlTimestamp ltz_mysql(1);  // equivalant
		checkEqual(ltz_mysql.convert("1970-01-01T10:31:25.123+01:00"), "1970-01-01 10:31:25.123", "Local Timezone: 1970-01-01T10:31:25.123+01:00 --> 1970-01-01 10:31:25.123");
		checkEqual(ltz_mysql.convert("1970-01-01T10:31:25.123+02:00"), "1970-01-01 09:31:25.123", "Local Timezone: 1970-01-01T10:31:25.123+02:00 --> 1970-01-01 09:31:25.123");
		checkEqual(ltz_mysql.convert("1970-01-01T10:31:25.123-06:00"), "1970-01-01 17:31:25.123", "Local Timezone: 1970-01-01T10:31:25.123-06:00 --> 1970-01-01 17:31:25.123");

		report(0, "Session timezone +03:00");
		MysqlTimestamp stz_mysql(3600*3,1); 
		checkEqual(stz_mysql.convert("1970-01-01T10:31:25.123+01:00"), "1970-01-01 12:31:25.123", "Session Timezone: 1970-01-01T10:31:25.123+01:00 --> 1970-01-01 10:31:25.123");
		checkEqual(stz_mysql.convert("1970-01-01T10:31:25.123+02:00"), "1970-01-01 11:31:25.123", "Session Timezone: 1970-01-01T10:31:25.123+02:00 --> 1970-01-01 09:31:25.123");
		checkEqual(stz_mysql.convert("1970-01-01T10:31:25.123-06:00"), "1970-01-01 19:31:25.123", "Session Timezone: 1970-01-01T10:31:25.123-06:00 --> 1970-01-01 19:31:25.123");
	}


	 void checkIsoTimestampRealValues()
    {	
		report(0,"Checking ISO-Timestamps for real values generated  ...");  

		// UK
		unsigned long long seconds1 = 1360950612;	
		int ms1 = 523;
		IsoTimestamp iso1(7200);	

		// Demo
		unsigned long long seconds2 = 1360947778;
		int ms2 = 223;
        IsoTimestamp iso2(3600);	
		
		checkEqual(iso1.convert(seconds1,ms1), "2013-02-15T17:50:12.523+02:00", "+02:00 UK-MT5");
		checkEqual(iso2.convert(seconds2,ms2), "2013-02-15T17:02:58.223+01:00", "+01:00 Demo");
    }

	void checkMysqlTimestampRealValues()
	{
		report(0,"Convert Real ISO-Timestamps to Mysql-Timestamps...");  
		MysqlTimestamp ltz_mysql(3600,1); 
		checkEqual(ltz_mysql.convert("2013-02-15T17:50:12.523+02:00"), "2013-02-15 16:50:12.523", "+02:00 --> +01:00");
		checkEqual(ltz_mysql.convert("2013-02-15T17:02:58.223+01:00"), "2013-02-15 17:02:58.223", "+01:00 --> +01:00");
	}


	 void checkIsoTimestampRealValuesDst()
    {	

		report(0,"Checking ISO-Timestamps for real values after DST to Summertime...");  

		int ltz = MysqlTimestamp::LocalTimezoneDst();
		checkEqual(ltz, 7200, "Local, System-Timezone (UTC+01 Berlin, adjusted to DST) in Summer is +02:00");

		// AlpariUK-MT5
		unsigned long long seconds1 = 1360950612 + 44*24*3600;	
		int ms1 = 523;
		IsoTimestamp iso1(7200);	

		// MetaQuotes-Demo
		unsigned long long seconds2 = 1360947778 + 44*24*3600;
		int ms2 = 223;
        IsoTimestamp iso2(3600);	
		
		checkEqual(iso1.convert(seconds1,ms1), "2013-03-31T17:50:12.523+02:00", "+02:00 UK");
		checkEqual(iso2.convert(seconds2,ms2), "2013-03-31T17:02:58.223+01:00", "+01:00 Demo");
    }



	void checkDbWriterUseCase() 
	{
		int ltz = MysqlTimestamp::LocalTimezoneDst();
		checkEqual(ltz, 7200, "Local, System-Timezone (UTC+01 Berlin, adjusted to DST) in Summer is +02:00");
		MysqlTimestamp ltz_mysql(ltz,1); 
		
		//Bottle IsoTimestamp = "2013-04-05T19:16:11.371+03:00";
		
		//decode
		checkEqual(ltz_mysql.convert("2013-04-05T19:16:11.371+03:00"), "2013-04-05 18:16:11.371", "+03:00 --> +02:00");
		
	}

	void checkNcTickMql5ReaderUseCase()
	{
		unsigned long long seconds = 1365190179;
		int ms = 998;
		IsoTimestamp isoTimestamp(10800);  // +03:00
		
		checkEqual(isoTimestamp.convert(seconds,ms), "2013-04-05T19:29:39.998+03:00", "+03:00 UK");

	}


	void checkNcDbReaderUseCase()
	{
	/*
		NcBottleMysqlProcessor::bottleTick id:1
		tick.time: 2013-04-05 18:46:56.376  (Mysql - Return)
		ace: 1365180416.376000
		iso: 2013-04-05T18:46:56.376+02:00
	*/
		
		// MysqlTimestamp		
		char * sql_result = "2013-04-05 18:46:56.376";
		MysqlTimestamp mysql_timestamp(MysqlTimestamp::LocalTimezoneDst(), 1); // +02:00 
		const ACE_Time_Value& ace_tv = mysql_timestamp.from(sql_result);
		checkEqual(mysql_timestamp.iso_timestamp().c_str(), "2013-04-05T18:46:56.376+02:00", "mysql_timestamp.iso_timestamp()");

		// Check internal ACE Time Value
		sql_result = "1970-01-01 10:00:00.100";
//		sql_result = "2012-01-01 10:00:01.101";
		MysqlTimestamp mysql_timestamp2(MysqlTimestamp::LocalTimezoneDst(), 1); // +02:00 
		checkEqual(mysql_timestamp2.from(sql_result).get_msec(), 10*3600*1000+100, "Local Timezone with DST");

		sql_result = "1970-01-01 10:00:00.200";
		MysqlTimestamp mysql_timestamp3(0, 1); // UTC
		checkEqual(mysql_timestamp3.from(sql_result).get_msec(), 10*3600*1000+200, "UTC");


		// IsoTimestamp
		//unsigned long long seconds = 1365180416;
		//int ms = 376;
		unsigned long long seconds = ace_tv.get_msec() / 1000;
		int ms = ace_tv.get_msec() % 1000;
		IsoTimestamp isoTimestamp(MysqlTimestamp::LocalTimezoneDst());  // +02:00		
		checkEqual(isoTimestamp.convert(seconds,ms), "2013-04-05T18:46:56.376+02:00", "isoTimestamp.convert(second,ms)");

	}

    virtual void runTests() {
            checkIsoTimestampBasic();
            checkMysqlTimestampBasic();
            checkIsoTimestampRealValues();
			checkMysqlTimestampRealValues();
			checkIsoTimestampRealValuesDst();
			checkDbWriterUseCase();
			checkNcTickMql5ReaderUseCase();
			checkNcDbReaderUseCase();
        }
};

static TimestampTest theTimestampTest;

UnitTest& getTimestampTest() {
    return theTimestampTest;
}
