#include "video_gr.h"
#include <stddef.h>
#include <unistd.h>
#include "kbd.h"
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "video_test.h"
#include "vbe.h"
#include "sprite.h"
#include "test5.h"

unsigned char scancode = 0;
unsigned long global_timer_counter = 0;

void *video_test_init(unsigned short mode, unsigned short delay)
{
	void *video_mem;

	if ((video_mem = vg_init(mode)) == NULL)
	{
		vg_exit();
		return NULL;
	}

	sleep(delay);

	vg_exit();

	return video_mem;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	if (vg_init (MODE_0x105) == NULL)
	{
		vg_exit();
		return 1;
	}

	vg_draw_square (x,y,size,color);

	video_dump_fb();

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;

	if ((irq_set = kbd_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

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
					kbd_int_handler();
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

	if (kbd_unsubscribe_int() == 1)
		return 1;

	vg_exit();

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color)
{
	if (vg_init (MODE_0x105) == NULL)
	{
		vg_exit();
		return 1;
	}


	vg_draw_line(xi,yi,xf,yf,color);

	video_dump_fb();

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;

	if ((irq_set = kbd_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

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
					kbd_int_handler();
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

	if (kbd_unsubscribe_int() == 1)
		return 1;

	vg_exit();

	return 0;
}

int video_test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	if (vg_init (MODE_0x105) == NULL)
	{
		vg_exit();
		return 1;
	}


	video_draw_xpm (xpm,xi,yi);

	video_dump_fb();

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int irq_set;

	if ((irq_set = kbd_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

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
					kbd_int_handler();
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

	if (kbd_unsubscribe_int() == 1)
		return 1;

	vg_exit();

	return 0;

}	

int video_test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	if (vg_init (MODE_0x105) == NULL)
	{
		vg_exit();
		return 1;
	}


	video_draw_xpm (xpm,xi,yi);

	video_dump_fb();

	if (xf < xi)
	{
		unsigned short temp = xi;
		xi = xf;
		xf = temp;
	}

	if (yf < yi)
	{
		unsigned short temp = yi;
		yi = xf;
		yf = temp;
	}

	if (yf-yi > 0 && xf - xi > 0)
		return 1;

	int xspeed;
	int yspeed;

	if(xf > xi)
	{
		xspeed = s;
		yspeed = 0;
	}

	else
	{
		yspeed = s;
		xspeed = 0;
	}

	Sprite * imagem = create_sprite(xpm,xi,yi,xspeed,yspeed);

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int kbd_irq_set;
	int timer_irq_set;

	if ((kbd_irq_set = kbd_subscribe_int()) == -1)
		return 1;

	if ((timer_irq_set = timer_subscribe_int()) == -1)
		return 1;

	//Handling interrupts

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
				if (msg.NOTIFY_ARG & kbd_irq_set)
				{
					kbd_int_handler();
				}

				if (msg.NOTIFY_ARG & timer_irq_set)
				{
					timer_int_handler();

					if(imagem->x < xf || imagem->y < yf)
					{
						float segundos = (float)global_timer_counter / FREQ_TIMER_0;

						if (s > 0)
							if (segundos == (float)1/f)
							{
								animate_sprite(imagem);
								video_dump_fb();
								global_timer_counter = 0;
							}

						if (s < 0)
							if (segundos == (((float)1/f) * -s))
							{
								animate_sprite(imagem);
								video_dump_fb();
								global_timer_counter = 0;
							}
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
	} while(scancode != ESC_RELEASE); // stops when it receives the ESC key release breakcode.

	//Unsubscribe interrupts

	if (kbd_unsubscribe_int() == 1)
		return 1;

	if (timer_unsubscribe_int() == 1)
		return 1;

	vg_exit();

	return 0;
}	

int test_controller()
{
	vbe_config_info info;

	vbe_get_info(&info);

	unsigned char versao_maior,versao_menor;

	versao_maior = (info.VbeVersion / 0x100);
	versao_menor = (info.VbeVersion % 0x100);

	printf("%u.%u\n",versao_maior,versao_menor);

	unsigned short *mode = (unsigned short *) info.Reserved;

	while(*mode != 0x0FFFF)
	{
		printf("0x%X:",*mode);
		mode++;
	}
	printf("\n");

	unsigned int memoria = (info.TotalMemory * 64);

	printf("%u\n",memoria);

	return 0;
}	
