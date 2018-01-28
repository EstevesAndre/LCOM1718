#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"

int hook_id = 0; //global pq é necessaria para a funcao subscribe e unsubscribe

static unsigned long global_counter = 0; //static (only loaded in this file) // long (because time argument is long)

int timer_set_frequency(unsigned char timer, unsigned long freq) { // 4º DONE

	if (timer > 2 || freq == 0) //Impossivel a frequencia ser 0
		return 1;

	unsigned char status;

	timer_get_conf (timer, &status);

	unsigned long read_back, valor;

	valor = TIMER_FREQ / freq; //Valor a ser carregado para o timer

	if (valor == 0)  //Impossivel carregar o valor 0 para o timer
		return 1;


	switch(timer)
	{
	case 0: read_back = (TIMER_SEL0 | TIMER_LSB_MSB | (status % 0x10)); // status % 0x10 obtem os 4 bits menos significativos de status
			sys_outb(TIMER_CTRL, read_back);
			sys_outb(TIMER_0, valor); //Carrega o LSB
			sys_outb(TIMER_0, (valor / 0x100)); //Carrega o MSB
			return 0;
	case 1: read_back = (TIMER_SEL1 | TIMER_LSB_MSB | (status % 0x10));
			sys_outb(TIMER_CTRL, read_back);
			sys_outb(TIMER_1, valor);
			sys_outb(TIMER_1, (valor / 0x100));
			return 0;
	case 2: read_back = (TIMER_SEL2 | TIMER_LSB_MSB | (status % 0x10));
			sys_outb(TIMER_CTRL, read_back);
			sys_outb(TIMER_2, valor);
			sys_outb(TIMER_2, (valor / 0x100));
			return 0;
	default: return 1;
	}

	return 1;
}

int timer_subscribe_int(void ) { // 7º DONE

	int temp = hook_id;

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK) && ((sys_irqenable(&hook_id)) == OK)) // check if for some reason the functions didn't return OK
		return BIT(temp);

	return -1;
}

int timer_unsubscribe_int() { // 8º DONE

	if ((sys_irqdisable(&hook_id) == OK) && (sys_irqrmpolicy(&hook_id) == OK)) // check if for some reason the functions didn't return OK
		return 0;

	return 1;
}


void timer_int_handler() { // 6º DONE
	global_counter++;
}


int timer_get_conf(unsigned char timer, unsigned char *st) { // 1º DONE

	unsigned long read_back = TIMER_RB_CMD | TIMER_RB_COUNT_; // 11100000
	unsigned long status;
	switch(timer)
	{
	case 0:
		read_back |=  TIMER_RB_SEL(0); // 11100010
		sys_outb(TIMER_CTRL, read_back);
		sys_inb(TIMER_0, &status);
		break;
	case 1:
		read_back |= TIMER_RB_SEL(1); // 11100100
		sys_outb(TIMER_CTRL, read_back);
		sys_inb(TIMER_1, &status);
		break;
	case 2:
		read_back |= TIMER_RB_SEL(2); // 11101000
		sys_outb(TIMER_CTRL, read_back);
		sys_inb(TIMER_2, &status);
		break;
	default:
		return 1;
	}

	*st = status;

	return 0;
}


int timer_display_conf(unsigned char conf) { // 2º DONE

	if (conf & BIT(7))
		printf (" Timer's OUT is low.\n");
	else
		printf (" Timer's OUT is high.\n");

	if (conf & BIT(6))
		printf (" Count available for reading.\n");
	else
		printf (" Null Count.\n");

	switch(conf & (BIT(5) | BIT(4)))
	{
	case BIT(4):
			printf(" Read/Write least significant byte only.\n");
			break;
	case BIT(5):
			printf(" Read/Write most significant byte only.\n");
			break;
	case (BIT(4) | BIT(5)):
			printf(" Read/Write least significant byte first, then most significant byte.\n");
			break;
	}

	switch (conf & (BIT(3) | BIT(2) | BIT(1)))
	{
	case 0:
		printf (" Mode 0.\n");
		break;
	case BIT(1):
		printf (" Mode 1.\n");
		break;
	case BIT(2):
		printf (" Mode 2.\n");
		break;
	case (BIT(3) | BIT(2)):
		printf (" Mode 2.\n");
		break;
	case ( BIT(1) | BIT(2) ):
		printf (" Mode 3.\n");
		break;
	case ( BIT(1) | BIT(2) | BIT(3) ):
		printf (" Mode 3.\n");
		break;
	case BIT(3):
		printf (" Mode 4.\n");
		break;
	case ( BIT(1) | BIT(3) ):
		printf (" Mode 5.\n");
		break;
	}
	
	if (conf & BIT(0))
		printf (" Binary Counter.\n");
	else
		printf (" BCD Counter.\n");

	return 0;
}

int timer_test_time_base(unsigned long freq) { // 5º DONE

	if (timer_set_frequency(0,freq) == 0){ // check if some error happened
		return 0;
	}

	return 1;
}

int timer_test_int(unsigned long time) { // 9º DONE

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;

	if ((irq_set = timer_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

	if ((timer_set_frequency(0, 60)) == 1) //Assegurar que a frequencia é 60
		return 1;

	while(time > (global_counter / FREQ_TIMER_0))
	{
		/* Get a request message */
		int r;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
				{
					printf("driver_receive failed with: %d", r);
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
									timer_int_handler();

									if ((global_counter % FREQ_TIMER_0) == 0)
									{
										printf("Mensagem %lu.\n", global_counter / FREQ_TIMER_0);
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

	//Unsubscribe interrupts

	if (timer_unsubscribe_int() == 1)
		return 1;

	return 0;
}

int timer_test_config(unsigned char timer) { // 3º DONE

	if (timer > 2) // out of range (0-2)
		return 1;

	unsigned char config;

	timer_get_conf(timer, &config); //only problem if timer > 2, verified before

	timer_display_conf(config); // display of the config
	
	return 0;
}
