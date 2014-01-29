/** 
 * @file        time_tool.h
 * @brief       TimeUtility is used to extract time infomation from text 
 * @author      geeseek  
 * @version     0.0
 */

#ifndef CLIB_TIME_TOOL_H
#define CLIB_TIME_TOOL_H

#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <memory>
#include <regex.h>


namespace clib 
{

class TimeUtility
{
    public:
        /**
         * @brief default constructor
         */
        TimeUtility();

        /**
         * @brief default destructor
         */
        ~TimeUtility();
        
        /** 
         * @brief extract time from content 
         *
         * @param text where to retrieve time info 
         * @return extracted time info in the format of unix time stamp
         */
        uint64_t Extract(const std::string& text) const;

        const static uint32_t TIME_MIN_YEAR;

    private:
        /**
         * @brief copy constructor function
         */
        TimeUtility(const TimeUtility &extractor);

        /**
         * @brief overload operator =
         */
        TimeUtility& operator=(const TimeUtility &rhs);

        /**
         * @brief Normalize extracted time and get the formatted time string
         *
         * @param yearStr: if there are only two digits represents year, the
         * prefix "20" will be added, so the "05" will be formatted as "2005"
         * @param monthStr: if there are only one digit represents month, the
         * prefix "0" will be added, so the "6" will be formatted as "06".
         * @param dayStr:day will be formatted like month.
         * @param unixTimeStamp: normalized unix time stamp
         * @return true if normalizing time successfully, false otherwise.
         */
        static bool NormalizeTime(std::string &yearStr, std::string &monthStr, std::string
                &dayStr, uint64_t* unixTimeStamp);
        /**
         * @brief Try to extract time in simple format without year info, such as 06-15 14:30
         *
         * @param text: the string to be extracted
         * @return extracted unix timestamp
         */
        uint64_t ExtractSimpleTime(const std::string& text) const; 
        
        /**
         * @brief regular expression of time
         */
        regex_t mTimeRegex;
        regex_t mSimpleTimeRegex;

        /**
         * @brief the time regex pattern string
         */
        static const std::string TIME_REGEX_PATTERN;
        static const std::string SIMPLE_TIME_PATTERN;
};

typedef std::shared_ptr<TimeUtility> TimeUtilityPtr;

} //end of namespace clib 


#endif
