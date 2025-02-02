#include "faker-cxx/Date.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <sstream>

#include "data/TimezoneNames.h"
#include "../../common/FormatHelper.h"
#include "data/MonthNames.h"
#include "data/WeekdayNames.h"
#include "faker-cxx/Helper.h"
#include "faker-cxx/Number.h"

namespace faker
{
namespace
{
std::string serializeTimePoint(const auto& timePoint, Date::DateFormat dateFormat)
{
    time_t timePointTimeT = std::chrono::system_clock::to_time_t(timePoint);

    std::tm utcTime = *std::gmtime(&timePointTimeT);

    std::stringstream ss;

    if (dateFormat == Date::DateFormat::Timestamp)
    {
        ss << std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
    }

    else
    {
        ss << std::put_time(&utcTime, "%Y-%m-%dT%H:%M:%SZ");
    }

    return ss.str();
}

std::string betweenDate(const auto& from, const auto& to, Date::DateFormat dateFormat)
{
    if (from > to)
    {
        throw std::runtime_error{FormatHelper::format("Start date is greater than end date. {{from: {}, to: {}}}",
                                                      serializeTimePoint(from, dateFormat),
                                                      serializeTimePoint(to, dateFormat))};
    }

    const auto size = std::chrono::duration_cast<std::chrono::seconds>(to - from).count();

    const auto randomDateWithinRange = from + std::chrono::seconds{Number::integer(size - 1)};

    return serializeTimePoint(randomDateWithinRange, dateFormat);
}

const auto numberOfHoursInDay = 24;
const auto numberOfDaysInYear = 365;
}

std::string Date::futureDate(int years, Date::DateFormat dateFormat)
{
    const auto startDate = std::chrono::system_clock::now() + std::chrono::hours{1};

    const auto endDate = startDate + std::chrono::hours{numberOfHoursInDay * numberOfDaysInYear * years};

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::pastDate(int years, Date::DateFormat dateFormat)
{
    const auto startDate =
        std::chrono::system_clock::now() - std::chrono::hours{numberOfHoursInDay * numberOfDaysInYear * years};

    const auto endDate = std::chrono::system_clock::now() - std::chrono::hours{1};

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::soonDate(int days, Date::DateFormat dateFormat)
{
    const auto startDate = std::chrono::system_clock::now() + std::chrono::hours{1};

    const auto endDate = startDate + std::chrono::hours{numberOfHoursInDay * days};

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::recentDate(int days, Date::DateFormat dateFormat)
{
    const auto startDate = std::chrono::system_clock::now() - std::chrono::hours{numberOfHoursInDay * days};

    const auto endDate = std::chrono::system_clock::now() - std::chrono::hours{1};

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::birthdateByAge(int minAge, int maxAge, Date::DateFormat dateFormat)
{
    const auto startDate =
        std::chrono::system_clock::now() - std::chrono::hours{numberOfHoursInDay * numberOfDaysInYear * maxAge};

    const auto endDate =
        std::chrono::system_clock::now() - std::chrono::hours{numberOfHoursInDay * numberOfDaysInYear * minAge};

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::birthdateByYear(int minYear, int maxYear, Date::DateFormat dateFormat)
{
    tm startDateTime{};
    startDateTime.tm_year = minYear - 1900;
    startDateTime.tm_mon = 0;
    startDateTime.tm_mday = 1;
    startDateTime.tm_hour = 0;
    startDateTime.tm_min = 0;
    startDateTime.tm_sec = 0;
    startDateTime.tm_isdst = -1;

    const auto startDate = std::chrono::system_clock::from_time_t(mktime(&startDateTime));

    tm endDateTime{};
    endDateTime.tm_year = maxYear - 1900;
    endDateTime.tm_mon = 11;
    endDateTime.tm_mday = 31;
    endDateTime.tm_hour = 23;
    endDateTime.tm_min = 59;
    endDateTime.tm_sec = 59;
    endDateTime.tm_isdst = -1;

    const auto endDate = std::chrono::system_clock::from_time_t(mktime(&endDateTime));

    return betweenDate(startDate, endDate, dateFormat);
}

std::string Date::weekdayName()
{
    return Helper::arrayElement<std::string>(weekdayNames);
}

std::string Date::weekdayAbbreviatedName()
{
    return Helper::arrayElement<std::string>(weekdayAbbreviatedNames);
}

std::string Date::monthName()
{
    return Helper::arrayElement<std::string>(monthNames);
}

std::string Date::monthAbbreviatedName()
{
    return Helper::arrayElement<std::string>(monthAbbreviatedNames);
}

unsigned int Date::year()
{
    unsigned int year = 1000 + (rand() % 3000);
    return year;
}

unsigned int Date::month()
{
    unsigned int month = 1 + (rand() % 12);
    return month;
}

unsigned int Date::hour()
{
    unsigned int hour = (rand() & 23);
    return hour;
}

unsigned int Date::minute()
{
    unsigned int minute = (rand() % 59);
    return minute;
}

unsigned int Date::second()
{
    unsigned int second = (rand() % 59);
    return second;
}

std::string Date::time()
{
    int hour = 1 + (rand() % 12);
    int minute = 1 + (rand() % 59);
    std::string time;
    {
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minute;
        time = ss.str();
    }
    return time;
}

unsigned int Date::dayOfMonth()
{
    unsigned int dayOfMonth = 1 + (rand() % 31);
    return dayOfMonth;
}

unsigned int Date::dayOfWeek()
{
    unsigned int dayOfWeek = 1 + (rand() % 7);
    return dayOfWeek;
}

std::string Date::timezone()
{
    // int index = rand() % timezoneNames.size() - 1;
    // return timezoneNames[index];
    return Helper::arrayElement<std::string>(timezoneNames);
}

}
}
