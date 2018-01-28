#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "kbd.h"
#include "video_test.h"

int kbd_hook_id = 0;
int timer_hook_id = 1;
unsigned char scancode;
unsigned long global_timer_counter;


int kbd_subscribe_int()
{
	int temp = kbd_hook_id;

	if ( (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id) == OK) && ((sys_irqenable(&kbd_hook_id)) == OK) ) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int kbd_unsubscribe_int()
{
	if ((sys_irqdisable(&kbd_hook_id) == OK) && (sys_irqrmpolicy(&kbd_hook_id) == OK)) // check if for some reason the functions didn't return OK
		return 0;

	return 1;
}

void kbd_int_handler()
{
	unsigned long stat, data;
	unsigned int tries;

	for(tries = 1; tries <= 5; tries++) {
		sys_inb(STAT_REG, &stat);	/* assuming it returns OK	*/
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /*	assuming it returns OK */
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
			{
				scancode = data;
				return;
			}

		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	//If there was an error, or there is no scancode to be read, scancode = 0
	scancode = 0;
	return;
}

int timer_subscribe_int(void ) {

	int temp = timer_hook_id;

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) == OK) && ((sys_irqenable(&timer_hook_id)) == OK)) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int timer_unsubscribe_int() {

	if ((sys_irqdisable(&timer_hook_id) == OK) && (sys_irqrmpolicy(&timer_hook_id) == OK)) // check if for some reason the functions didn't return OK
		return 0;

	return 1;
}

void timer_int_handler() {
	global_timer_counter++;
}
