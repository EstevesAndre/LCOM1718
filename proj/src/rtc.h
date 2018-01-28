#ifndef RTC_H
#define RTC_H

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_ADDR_HOUR 0x4
#define RTC_ADDR_DAY 0x7
#define RTC_ADDR_MONTH 0x8
#define RTC_ADDR_YEAR 0x9


/**
 * @brief Gets current date using RTC and returns it on data param as a string with format yyyy-mm-dd
 *
 * @param data string whete date will be stored
 */
void rtc_get_date(char * data);


/**
 * @brief Gets current hour using RTC and returns it in binary
 *
 * @return Returns hour in binary
 */
unsigned long rtc_get_hour();

#endif
