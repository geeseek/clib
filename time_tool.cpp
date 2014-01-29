#include "time_tool.h"

#include<cctype>
#include<cstdlib>
#include<ctime>
#include<stdexcept>

using namespace std;

namespace clib 
{

const string TimeUtility::TIME_REGEX_PATTERN = "(^|[^0-9])([0-9]{2,4})(年|-|—|/|\\.)\\s*(0?[1-9]|1[0-2])(月|-|—|/|\\.)\\s*(0?[1-9]|[1-2][0-9]|3[0-1])(日|号)?"; 
const string TimeUtility::SIMPLE_TIME_PATTERN = "([0-9]{1,2})(月|-)(\\s)*([0-9]{1,2})(日|号)?(\\s)*([0-9]{2}):([0-9]{2})";
const uint32_t TimeUtility::TIME_MIN_YEAR = 1990; 

TimeUtility::TimeUtility()
{
    //compile the time regex
    if (regcomp(&mTimeRegex, TIME_REGEX_PATTERN.c_str(), REG_EXTENDED) != 0)
    {
        throw runtime_error("Invalid regex pattern: " + TIME_REGEX_PATTERN); 
    }
    if (regcomp(&mSimpleTimeRegex, SIMPLE_TIME_PATTERN.c_str(), REG_EXTENDED) != 0)
    {
        regfree(&mTimeRegex);
        throw runtime_error("Invalid regex pattern: " + SIMPLE_TIME_PATTERN); 
    }
}

TimeUtility::~TimeUtility()
{
    regfree(&mTimeRegex);
    regfree(&mSimpleTimeRegex);
}

uint64_t TimeUtility::Extract(const string& text) const
{
    string timeStr;
    regmatch_t value[9];
    //extract the time with specified format
    if (0 == regexec(&mTimeRegex, text.c_str(), 9, value, 0))
    {
        uint64_t unixTimeStamp = 0;
        string year(text.c_str() + value[2].rm_so, text.c_str() + value[2].rm_eo);
        string month(text.c_str() + value[4].rm_so, text.c_str() + value[4].rm_eo);
        string day(text.c_str() + value[6].rm_so, text.c_str() + value[6].rm_eo);
        if (NormalizeTime(year, month, day, &unixTimeStamp))
        {
            return unixTimeStamp;
        }
    }
    return ExtractSimpleTime(text);
}

bool TimeUtility::NormalizeTime(string &yearStr, string &monthStr, string &dayStr, uint64_t* unixTimeStamp)
{
    //validate the time
    uint32_t year = 0;
    uint32_t month = 0;
    uint32_t day = 0;
    year = std::atoi(yearStr.c_str());
    month = std::atoi(monthStr.c_str());
    day = std::atoi(dayStr.c_str());

    if (yearStr.size() != 2 && year < TIME_MIN_YEAR)
    {
        return false;
    }
    
    //normalize year
    if (yearStr.size() == 2)
    {
        if (year < 70) 
        {
            yearStr = "20" + yearStr;
            year += 2000;
        }
        else
        {
            yearStr = "19" + yearStr;
            year += 1900;
        }
    }
    if (year < TIME_MIN_YEAR)
    {
        return false;
    }

    //Normalize month
    if (monthStr.size() == 1)
    {
        monthStr = "0" + monthStr;
    }

    //Normalize day
    if (dayStr.size() == 1)
    {
        dayStr = "0" + dayStr;
    }

    //if current time is before extracted time, return false
    time_t currentTime = time(NULL);
    tm timeInfo;
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    time_t extractedTime = mktime(&timeInfo);
    if (extractedTime > currentTime)
    {
        return false;
    }
    *unixTimeStamp = (uint64_t)extractedTime;
    return true;
}

uint64_t TimeUtility::ExtractSimpleTime(const string& text) const
{
    time_t curTime = time(NULL);
    struct tm timeInfo;
    struct tm* ptm = NULL;
    ptm = gmtime(&curTime);
    if (!ptm)
        return 0;
    timeInfo.tm_year = ptm->tm_year; 
    regmatch_t value[9];
    if (0 == regexec(&mSimpleTimeRegex, text.c_str(), 9, value, 0))
    {
        try
        {
	    string tmp = string(text.c_str() + value[1].rm_so, text.c_str() + value[1].rm_eo);
            timeInfo.tm_mon = std::atoi(tmp.c_str()) - 1;
	    tmp = string(text.c_str() + value[4].rm_so, text.c_str() + value[4].rm_eo);
            timeInfo.tm_mday  = std::atoi(tmp.c_str());
            if (timeInfo.tm_mon > ptm->tm_mon
                || timeInfo.tm_mon == ptm->tm_mon && timeInfo.tm_mday <= ptm->tm_mday)
                timeInfo.tm_year -= 1;
        }
        catch (...)
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    time_t extractedTime = mktime(&timeInfo);
    return (uint64_t)extractedTime;
}

} //end of namespace clib 
