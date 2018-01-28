#include "rtc.h"
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <stdlib.h>

void rtc_get_date(char * data)
{
	unsigned long ano,mes,dia;

	sys_outb(RTC_ADDR_REG, RTC_ADDR_YEAR);
	sys_inb(RTC_DATA_REG, &ano);

	sys_outb(RTC_ADDR_REG, RTC_ADDR_MONTH);
	sys_inb(RTC_DATA_REG, &mes);

	sys_outb(RTC_ADDR_REG, RTC_ADDR_DAY);
	sys_inb(RTC_DATA_REG, &dia);

	char year[3] = {'\0'}, month[3] = {'\0'}, day[3] = {'\0'};

	sprintf(year, "%02x", ano, sizeof(year));
	sprintf(month, "%02x", mes, sizeof(month));
	sprintf(day, "%02x", dia, sizeof(day));

	data[0] = '2';
	data[1] = '0';
	data[2] = year[0];
	data[3] = year[1];
	data[4] = '-';
	data[5] = month[0];
	data[6] = month[1];
	data[7] = '-';
	data[8] = day[0];
	data[9] = day[1];
	data[10] = '\0';

	//data é um array de caracteres com a data no formato yyyy-mm-dd
}

unsigned long rtc_get_hour()
{
	unsigned long hour;

	sys_outb(RTC_ADDR_REG, RTC_ADDR_HOUR);
	sys_inb(RTC_DATA_REG, &hour);

	return hour;
}
