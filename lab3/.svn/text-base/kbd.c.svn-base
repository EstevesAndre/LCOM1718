#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "test3.h" //only to use global_cnt in both test3.c and this file

#ifdef LAB3
unsigned int global_cnt;
#endif

int kbd_hook_id = 0;

int timer_hook_id = 1;

unsigned long global_timer_counter;
unsigned char scancode;

int kbd_subscribe_scan()
{
	int temp = kbd_hook_id;

	if ( (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id) == OK) && ((sys_irqenable(&kbd_hook_id)) == OK) ) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int timer_subscribe_int(void ) {

	int temp = timer_hook_id;

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) == OK) && ((sys_irqenable(&timer_hook_id)) == OK)) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int kbd_unsubscribe_scan()
{
	if ((sys_irqdisable(&kbd_hook_id) == OK) && (sys_irqrmpolicy(&kbd_hook_id) == OK)) // check if for some reason the functions didn't return OK
			return 0;

		return 1;
}

int timer_unsubscribe_int() {

	if ((sys_irqdisable(&timer_hook_id) == OK) && (sys_irqrmpolicy(&timer_hook_id) == OK)) // check if for some reason the functions didn't return OK
		return 0;

	return 1;
}

void kbd_scan_handler()
{
	unsigned long stat, data;
	unsigned int tries;

	for(tries = 1; tries <= 5; tries++) {
		sys_inb(STAT_REG, &stat);	/* assuming it returns OK	*/
		#ifdef LAB3
		global_cnt++;
		#endif
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /*	assuming it returns OK */
			#ifdef LAB3
			global_cnt++;
			#endif
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

void timer_int_handler() {
	global_timer_counter++;
}

int kbd_issue_command(unsigned long cmd)
{
	unsigned long stat;
	unsigned int tries;

	for (tries = 1; tries <= 5; tries++) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		#ifdef LAB3
		global_cnt++;
		#endif
		/* loop while 8042 input buffer is not empty */

		if( (stat & IBF) == 0 ) {
			sys_outb(KBC_CMD_REG, cmd); /* no args command */
			return 0;
			}

		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	return 1;
}

unsigned long kbd_read_command_byte()
{
	unsigned long cmd_byte = kbd_issue_command(READ_CMD_BYTE);

	sys_inb(OUT_BUF,&cmd_byte);
	#ifdef LAB3
	global_cnt++;
	#endif

	return cmd_byte;

}

int kbd_write_command_byte(unsigned long cmd_byte)
{
	kbd_issue_command(WRITE_CMD_BYTE);

	sys_outb(OUT_BUF,(cmd_byte | ENABLE_INTERRUPT));

	return 0;
}




