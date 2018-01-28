#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"

int timer_hook_id = 0;
unsigned long global_timer_counter;

int timer_subscribe_int()
{
	int temp = timer_hook_id;

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) == OK) && ((sys_irqenable(&timer_hook_id)) == OK))
		return BIT(temp);

	return -1;
}

int timer_unsubscribe_int()
{
	if ((sys_irqdisable(&timer_hook_id) == OK) && (sys_irqrmpolicy(&timer_hook_id) == OK))
		return 0;

	return 1;
}


void timer_int_handler()
{
	global_timer_counter++;
}
