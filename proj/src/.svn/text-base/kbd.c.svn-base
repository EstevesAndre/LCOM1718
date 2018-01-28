#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "kbd.h"

int kbd_hook_id = 1;
unsigned char scancode;

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

	for(tries = 1; tries <= 5; tries++)
	{
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
