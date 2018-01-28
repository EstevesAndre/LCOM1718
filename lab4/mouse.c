#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "test4.h" //For global variables

int mouse_hook_id = 0;
int timer_hook_id = 1;
unsigned char array[3];
unsigned int array_pos;
unsigned long global_timer_counter;

int mouse_subscribe_int()
{
	int temp = mouse_hook_id;

	if ( (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id) == OK) && ((sys_irqenable(&mouse_hook_id)) == OK) ) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int mouse_unsubscribe_int()
{
	if ((sys_irqdisable(&mouse_hook_id) == OK) && (sys_irqrmpolicy(&mouse_hook_id) == OK)) // check if for some reason the functions didn't return OK
		return 0;

	return 1;
}


int mouse_issue_command(unsigned long command)
{
	unsigned long stat;


	sys_inb(STAT_REG,&stat);

	if (stat & (IBF | OBF))
		return 1;

	sys_outb(CMD_REG,WRITE_TO_MOUSE);

	sys_outb(OUT_BUF,command);

	tickdelay(micros_to_ticks(WAIT_KBC));

	sys_inb(OUT_BUF,&stat);

	if (stat == ACK) //Comando bem executado
		return 0;

	if (stat == NACK) //Tentar outra vez
		return mouse_issue_command(command);

	if (stat == ERROR) //Comando falhou
		return 1;

	return 1;
}

int mouse_stream_mode()
{
	if (mouse_issue_command(SET_STREAM_MODE) == 1)
		return 1;

	if (mouse_issue_command(ENABLE_DATA_REPORT) == 1)
		return 1;

	return 0;

}

void mouse_int_handler()
{
	unsigned long data;

	sys_inb(OUT_BUF, &data);

	if (array_pos == 0 && ((data & BIT(3)) == 0))
		return;

	array[array_pos] = data;

	array_pos++;

}

int mouse_disable_data_report()
{
	if (mouse_issue_command(DISABLE_DATA_REPORT) == 1)
		return 1;

	return 0;
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

unsigned int kbc_issue_command(unsigned long cmd)
{
	unsigned long stat;
	unsigned int tries;

	for (tries = 1; tries <= 10; tries++) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			sys_outb(CMD_REG, cmd); /* no args command */
			return 0;
		}

		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	return 1;
}

unsigned long mouse_read_command_byte()
{
	unsigned long cmd_byte;

	kbc_issue_command(READ_CMD_BYTE);

	sys_inb(OUT_BUF,&cmd_byte);

	return cmd_byte;
}

int mouse_disable_interrupts(unsigned long command_byte)
{
	if (kbc_issue_command(WRITE_CMD_BYTE))
		return 1;

	sys_outb(OUT_BUF, command_byte & (0xFF ^ MOUSE_INT));//0xFF ^ MOUSE_INT results in every bit at 1 less mouse_int bit(is 0). AND with command_byte ensures command byte statys the same, but mouse_interrupt is 0

	return 0;
}

int mouse_enable_interrupts(unsigned long command_byte)
{
	if (kbc_issue_command(WRITE_CMD_BYTE))
		return 1;

	sys_outb(OUT_BUF, command_byte);

	return 0;
}

int mouse_remote_mode()
{
	if (mouse_issue_command(ENABLE_DATA_REPORT) == 1) //When we booted minix and tested this function before test_packet() the mouse was not recognizing the clikcs/moves, this was our solution
		return 1;

	if (mouse_issue_command(DISABLE_DATA_REPORT) == 1)
		return 1;

	if (mouse_issue_command(SET_REMOTE_MODE) == 1)
		return 1;

	return 0;

}

int mouse_remote_handler()
{
	if (mouse_issue_command(READ_DATA))
		return 1;

	unsigned long stat, data;

	while(array_pos < 3)
	{
		sys_inb(STAT_REG, &stat);

		if( stat & (OBF | AUX))
		{
			sys_inb(OUT_BUF, &data);
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
			{
				if (array_pos == 0 && ((data & BIT(3)) == 0))
					return 1;

				array[array_pos] = data;

				array_pos++;
			}
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	return 0;
}

int mouse_reset_stream_mode()
{
	if (mouse_issue_command(DISABLE_DATA_REPORT) == 1)
		return 1;

	if (mouse_issue_command(SET_STREAM_MODE) == 1)
		return 1;

	return 0;

}
