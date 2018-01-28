#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"

int mouse_hook_id = 2;
unsigned char array[3];
unsigned char array_pos;

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
