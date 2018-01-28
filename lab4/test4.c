#include <stdlib.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "mouse.h"
#include "i8042.h"
#include "test4.h" //For global variables

unsigned char array[3] = {0,0,0};
unsigned int array_pos = 0;
unsigned long global_timer_counter = 0;


void display_packet()
{
	if (array[0] > 15)
		printf("B1=0x%x ", array[0]);
	else
		printf("B1=0x%x  ", array[0]);

	if (array[1] > 15)
		printf("B2=0x%x ", array[1]);
	else
		printf("B2=0x%x  ", array[1]);

	if (array[2] > 15)
		printf("B3=0x%x", array[2]);
	else
		printf("B3=0x%x ", array[2]);

	if (array[0]&LB)
		printf (" LB=1");
	else
		printf (" LB=0");

	if (array[0]&MB)
		printf (" MB=1");
	else
		printf (" MB=0");

	if (array[0]&RB)
		printf (" RB=1");
	else
		printf (" RB=0");

	if (array[0]&XOV)
		printf (" XOV=1");
	else
		printf (" XOV=0");

	if (array[0]&YOV)
		printf (" YOV=1");
	else
		printf (" YOV=0");

	if (array[0] & XSIGN)
	{
		if (((unsigned char) (~array[1]) + 1) < 10)
			printf (" X=-%u  ", (unsigned char) (~array[1]) + 1); //Obter o valor absoluto do complemento para 2
		else if (((unsigned char) (~array[1]) + 1) < 100)
			printf (" X=-%u ", (unsigned char) (~array[1]) + 1);
		else
			printf (" X=-%u", (unsigned char) (~array[1]) + 1);
	}
	else
	{
		if (array[1] < 10)
			printf (" X=%u   ", array[1]);
		else if (array[1] < 100)
			printf (" X=%u  ", array[1]);
		else
			printf (" X=%u ", array[1]);
	}
	if (array[0] & YSIGN)
		printf (" Y=-%u\n", (unsigned char) (~array[2]) + 1); //Obter o valor absoluto do complemento para 2
	else
		printf (" Y=%u\n", array[2]);
}


int mouse_test_packet(unsigned short cnt){

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;


	if (mouse_stream_mode() == 1) //Enables mouse stream mode
		return 1;

	if ((irq_set = mouse_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

	unsigned int packet_count = 0; //Counts how many packets have been shown

	while (cnt > packet_count)
	{
		/* Get a request message */
		int r;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set)
				{
					mouse_int_handler();
					if (array_pos == 3)
					{
						display_packet();
						packet_count++;
						array_pos = 0;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}

		else
		{
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (mouse_disable_data_report() == 1)
		return 1;

	//Unsubscribe interrupts

	if (mouse_unsubscribe_int() == 1)
		return 1;

	return 0;

}	


int mouse_test_async(unsigned short idle_time){

	int ipc_status;
	message msg;

	//Subscribe mouse interrupts

	int irq_set_mouse;

	if (mouse_stream_mode() == 1) //Enables mouse stream mode
		return 1;

	if ((irq_set_mouse = mouse_subscribe_int()) == -1)
		return 1;

	//Subscribe timer 0 interrupts

	int irq_set_timer;

	if ((irq_set_timer = timer_subscribe_int()) == -1)
		return 1;


	//Handling interrupts

	while (idle_time > (global_timer_counter / FREQ_TIMER_0))
	{
		/* Get a request message */
		int r;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) //Mouse Interrupt
				{

					mouse_int_handler();
					if (array_pos == 3)
					{
						display_packet();
						global_timer_counter = 0;
						array_pos = 0;
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_timer) //Timer 0 interrupt
				{
					timer_int_handler();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}

		else
		{
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (mouse_disable_data_report() == 1)
		return 1;

	//Unsubscribe interrupts

	if (mouse_unsubscribe_int() == 1)
		return 1;

	return 0;
}	



int mouse_test_remote(unsigned long period, unsigned short cnt)
{
	unsigned long command_byte = mouse_read_command_byte();

	if (mouse_disable_interrupts(command_byte))
		return 1;

	if (mouse_remote_mode()) //Enables mouse remote mode
		return 1;

	unsigned int packet_count = 0; //Counts how many packets have been shown

	while (cnt > packet_count)
	{
		if (mouse_remote_handler())
			continue;

		display_packet();
		packet_count++;
		array_pos = 0;

		tickdelay(micros_to_ticks(period*1000));
	}

	if (mouse_reset_stream_mode())
		return 1;

	if (mouse_enable_interrupts(command_byte))
		return 1;

	return 0;
}


typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOWN, RUP, RIGHT_MOVE, WRONG_MOVE} ev_type_t;

static state_t st = INIT; //initial state; keep state


void check_positive_slope(ev_type_t *evt, short length)
{
	static unsigned short tamanho = 0;

	switch (st)
	{
	case INIT:
		if (*evt == RDOWN)
			st = DRAW;
		break;

	case DRAW:
		if (*evt == RIGHT_MOVE)
		{
			if (length > 0)
				tamanho += array[1];
			else
				tamanho += (unsigned char) ((~array[1]) + 1);

			if (tamanho >= abs(length))
				st = COMP;
		}

		else if (*evt == WRONG_MOVE)
			tamanho = 0;

		else if (*evt == RUP)
			st = INIT;

		break;

	default: break;
	}

}


int mouse_test_gesture(short length){

	if (length == 0)
		return 0;

	int ipc_status;
	message msg;

	ev_type_t evento;
	char processar_state_machine = 0;  // when necessary to process the state machine (read packet) = 1, 0 otherwise
	char r_pressed = 0;//If r is pressed = 1, else 0

	//Subscribe interrupts

	int irq_set;


	if (mouse_stream_mode() == 1) //Enables mouse stream mode
		return 1;

	if ((irq_set = mouse_subscribe_int()) == -1)
		return 1;

	//Handling interrupts
	while (st != COMP)
	{
		/* Get a request message */
		int r;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set)
				{
					mouse_int_handler();
					if (array_pos == 3)
					{
						display_packet();
						processar_state_machine = 1;
						array_pos = 0;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}

		else
		{
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}

		/* Now, do application dependent event handling */
		if (processar_state_machine == 1)
		{
			if ((r_pressed == 0) && (array[0] & RB))
			{
				evento = RDOWN;
				check_positive_slope(&evento, length);
				r_pressed = 1;
			}

			if ((r_pressed == 1) && !(array[0] & RB))
			{
				evento = RUP;
				check_positive_slope(&evento, length);
				r_pressed = 0;
			}

			if (length > 0)
			{
				if ((array[0] & XSIGN) || (array[0] & YSIGN) || !(array[1]) || !(array[2]))//Assuming straight horizontal and straight vertical lines are not accepted
				{
					evento = WRONG_MOVE;
					check_positive_slope(&evento, length);
				}

				else
				{
					evento = RIGHT_MOVE;
					check_positive_slope(&evento, length);
				}

			}

			else
			{
				if (!(array[0] & XSIGN) || !(array[0] & YSIGN) || !(array[1]) || !(array[2]))//Assuming straight horizontal and straight vertical lines are not accepted
				{
					evento = WRONG_MOVE;
					check_positive_slope(&evento, length);
				}

				else
				{
					evento = RIGHT_MOVE;
					check_positive_slope(&evento, length);
				}


			}

			processar_state_machine = 0;

		}
	}

	if (mouse_disable_data_report() == 1)
		return 1;

	//Unsubscribe interrupts

	if (mouse_unsubscribe_int() == 1)
		return 1;

	return 0;
}	
