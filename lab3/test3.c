#include "test3.h"
#include "kbd.h"
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>

unsigned int global_cnt = 0;
unsigned long global_timer_counter = 0;
unsigned char scancode = 0; //To read the scan codes

int kbd_test_scan(unsigned short assembly) { //DONE
	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;

	if ((irq_set = kbd_subscribe_scan()) == -1)
		return 1;

	//Handling interrupts

	int two_bytes = 0; //To know if make code is two byte long

	do
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
									if (assembly == 0)
										kbd_scan_handler();

									else
										kbd_scan_handler_asm();


									if (scancode == TWO_BYTE)
										two_bytes = 1;

									else if (scancode & BREAKCODE_BIT)
									{
										if (two_bytes)
										{
											printf("Breakcode: 0xe0 0x%02x\n", scancode); // Breakcode, prints two bits keyboard code, ex: right enter
											two_bytes = 0;
										}

										else printf("Breakcode: 0x%02x\n", scancode); // Breakcode, prints one bit keyboard code, pretty almost keys
									}

									else if (two_bytes)
										{
											printf("Makecode: 0xe0 0x%02x\n", scancode); // Makecode, prints two bits keyboard code, ex: right enter
											two_bytes = 0;
										}
									else printf("Makecode: 0x%02x\n", scancode); // Makecode, prints one bit keyboard code, pretty almost keys

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
	} while(scancode != ESC_RELEASE); // stops when it receives the ESC key release breakcode.

	//Unsubscribe interrupts

	if (kbd_unsubscribe_scan() == 1)
		return 1;

	if (assembly == 0)
		printf("\nNumber of sys_inb kernel calls: %u\n", global_cnt);

	return 0;
}


int kbd_test_poll() { //DONE

	unsigned long cmd_byte = kbd_read_command_byte(); //Needed for re-enabling interrupts

	int two_bytes = 0; //To know if make code is two byte long

	do
	{
		kbd_scan_handler();

		if (scancode == 0)
			continue;

		if (scancode == TWO_BYTE)
			two_bytes = 1;

		else if (scancode & BREAKCODE_BIT)
		{
			if (two_bytes)
			{
				printf("Breakcode: 0xe0 0x%02x\n", scancode);
				two_bytes = 0;
			}

			else printf("Breakcode: 0x%02x\n", scancode);
		}

		else if (two_bytes)
		{
			printf("Makecode: 0xe0 0x%02x\n", scancode);
			two_bytes = 0;
		}
		else
			printf("Makecode: 0x%02x\n", scancode);

	} while (scancode != ESC_RELEASE);

	printf("\nNumber of sys_inb kernel calls: %u\n", global_cnt); // prints the sys_inb kernel calls

	kbd_write_command_byte(cmd_byte); //Re-enables interrupts

	return 0;
}


int kbd_test_timed_scan(unsigned short n) { //DONE

	int ipc_status;
	message msg;

	//Subscribe keyboard interrupts

	int irq_set_kbd;

	if ((irq_set_kbd = kbd_subscribe_scan()) == -1)
		return 1;

	//Subscirbe timer 0 interrupts

	int irq_set_timer;

	if ((irq_set_timer = timer_subscribe_int()) == -1)
		return 1;

	//Handling keyboard interrupts

	int two_bytes = 0; //To know if make code is two byte long

	do
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
								if (msg.NOTIFY_ARG & irq_set_kbd) /*Keyboard notification */
								{
									global_timer_counter = 0; //when a key is pressed/released, the time since last scancode is 0

									kbd_scan_handler();

									if (scancode == TWO_BYTE)
										two_bytes = 1;

									else if (scancode & BREAKCODE_BIT)
									{
										if (two_bytes)
										{
											printf("Breakcode: 0xe0 0x%02x\n", scancode);
											two_bytes = 0;
										}
										else printf("Breakcode: 0x%02x\n", scancode);
									}

									else if (two_bytes)
									{
										printf("Makecode: 0xe0 0x%02x\n", scancode);
										two_bytes = 0;
									}
									else printf("Makecode: 0x%02x\n", scancode);

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
	} while(scancode != ESC_RELEASE && (n > (global_timer_counter / FREQ_TIMER_0))); // stops when it receives the ESC key release breakcode or when it hasn't received a scancode in n seconds

	//Unsubscribe keyboard interrupts

	if (kbd_unsubscribe_scan() == 1)
		return 1;

	//Unsubscribe timer 0 interrupts

	if (timer_unsubscribe_int() == 1)
		return 1;

	return 0;
}
