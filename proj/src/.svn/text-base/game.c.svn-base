#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "video_gr.h"

#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "sprite.h"
#include "main_character.h"
#include "obstacles.h"
#include "linked_list.h"

//For rand
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

unsigned long global_timer_counter = 0;
unsigned long global_time = 0;
unsigned char scancode = 0;
unsigned char array[3] = {0,0,0};
unsigned char array_pos = 0;


int check_collision(Sprite *jogador, Sprite *obstaculo)
{
	unsigned int jogador_max = jogador->y + jogador->height - 1;
	unsigned int obstaculo_max = obstaculo->y + obstaculo->height - 1;

	if(obstaculo->yspeed == 1 && jogador_max != obstaculo_max)
		return 0;

	if(obstaculo->yspeed == 1 || jogador_max == obstaculo_max)
	{
		if ((jogador->x + jogador->width) < obstaculo->x){
			return 0;
		}
		if (jogador->x > (obstaculo->x + obstaculo->width))
			return 0;

		return 1;
	}

	if(jogador_max > obstaculo_max || obstaculo_max - jogador_max >= obstaculo->yspeed)
		return 0;

	//Calculate x when they have same y
	unsigned int last_x = obstaculo->x + (2 * (obstaculo_max - jogador_max));

	if ((jogador->x + jogador->width) < last_x){
		return 0;
	}
	if (jogador->x > (last_x + obstaculo->width))
		return 0;

	return 1;
}

int game(int background_color)
{
	global_timer_counter = 0;
	global_time = 0;
	scancode = 0;
	array_pos = 0;
	srand(time(NULL));

	int freq = 30;//Refresh rate

	draw_inicial_stuff(background_color);
	display_time(0);

	Sprite * jogador = create_sprite(main_character,175,498,0,0,1);

	animate_sprite(jogador);

	list_t *lista = new_list();

	int difficulty = 1;
	unsigned int num_obstacles = rand() % 2 + 1;

	if (num_obstacles == 1)
	{
		int random = rand() % 250;

		Sprite * temp = create_sprite(obstacle_1,random+710,random+10,-2,1,2);

		push_back_list(lista,temp);
	}

	if (num_obstacles == 2)
	{
		int random1, random2;

		do
		{
			random1 = rand() % 250;
			random2 = rand() % 250;

			if (random2 < random1)
			{
				int swap = random1;

				random1 = random2;
				random2 = swap;
			}
		} while(random1 + 80 >= random2);

		Sprite * temp = create_sprite(obstacle_1,random2+710,random2+10,-2,1,2);
		push_back_list(lista,temp);

		temp = create_sprite(obstacle_1,random1+710,random1+10,-2,1,2);
		push_back_list(lista,temp);
	}

	copy_to_videomem();

	float segundos = (float)global_timer_counter / FREQ_TIMER_0;
	int gameover = 0; //1 if gameover
	int need_to_draw = -1; //0 if we need to animate player, 1 we need to animate obstacle
	int generate_obstacles = 0;
	int hit_obs = 0;

	int ipc_status;
	message msg;

	//Subscribe interrupts

	int timer_irq_set = BIT(0);
	int kbd_irq_set = BIT(1);
	int mouse_irq_set = BIT(2);

	//Handling interrupts

	do
	{
		//Get a request message
		int r;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			// received notification
			switch (_ENDPOINT_P(msg.m_source))
			{

			case HARDWARE: // hardware interrupt notification
				if (msg.NOTIFY_ARG & mouse_irq_set)
				{
					mouse_int_handler();

					if (array_pos == 3)
					{
						if (array[0] & LB)
						{
							if ((array[0] & XSIGN) && (unsigned char) (~array[1]) + 1 <= 10 && (jogador->y + jogador->height - 1 >= 485))
							{
								set_speed(jogador,-3,-3);
								need_to_draw = 0;
							}

							else if (!(array[0] & XSIGN) && array[1] >= 10 && (jogador->y + jogador->height - 1 <= 743))
							{
								set_speed(jogador,3,3);
								need_to_draw = 0;
							}

							else set_speed(jogador,0,0);
						}

						else set_speed(jogador,0,0);
						array_pos = 0;
					}
				}
				if (msg.NOTIFY_ARG & kbd_irq_set)
				{
					kbd_int_handler();

					if(scancode == 0x1e && (jogador->y + jogador->height - 1 >= 485))
					{
						set_speed(jogador,-10,-10);
						need_to_draw = 0;
					}

					else if(scancode == 0x20 && (jogador->y + jogador->height - 1 <= 743))
					{
						set_speed(jogador,10,10);
						need_to_draw = 0;
					}

					else if(scancode == 0x9e || scancode == 0xa0)
					{
						set_speed(jogador,0,0);
					}

				}
				if (msg.NOTIFY_ARG & timer_irq_set)
				{
					timer_int_handler();

					float segundos_temp = (float)global_timer_counter / FREQ_TIMER_0;

					if((int) segundos_temp - (int) segundos != 0)
					{
						difficulty = ((int) segundos_temp / 30) + 1;
						display_time( (int)segundos_temp);

						display_points( (int)segundos_temp, hit_obs );
						if(hit_obs != 0)
							hit_obs = 0;

						if ((int)segundos_temp % 5 == 0)
							generate_obstacles = 1;
					}


					segundos = segundos_temp;

					if ((int)segundos % (int)1/freq == 0)
						need_to_draw = 1;

				}
				break;
			default:
				continue; // no other notifications expected: do nothing
			}
		}

		else
		{
			//received a standard message, not a notification
			//no standard messages expected: do nothing
			continue;
		}

		if (generate_obstacles)
		{
			num_obstacles = rand() % 2 + 1;

			if (num_obstacles == 1)
			{
				int random = rand() % 250;

				Sprite * temp = create_sprite(obstacle_1,random+710,random+10,-2*difficulty,1*difficulty,2);

				push_back_list(lista,temp);
			}

			if (num_obstacles == 2)
			{
				int random1, random2;

				do
				{
					random1 = rand() % 250;
					random2 = rand() % 250;

					if (random2 < random1)
					{
						int swap = random1;
						random1 = random2;
						random2 = swap;
					}
				} while(random1 + 80 >= random2);

				Sprite * temp = create_sprite(obstacle_1,random2+710,random2+10,-2*difficulty,1*difficulty,2);
				push_back_list(lista,temp);

				temp = create_sprite(obstacle_1,random1+710,random1+10,-2*difficulty,1*difficulty,2);
				push_back_list(lista,temp);
			}

			generate_obstacles = 0;
		}

		if (need_to_draw == 0)
		{
			int animou_jogador = 0;
			struct list_element * temp = lista->back;

			while(temp != NULL && !gameover)
			{
				Sprite * obstacle = temp->element;

				if(!animou_jogador && (jogador->y + jogador->height) <= (obstacle->y + obstacle->height))
				{
					animate_sprite(jogador);
					draw_sprite(obstacle);
					animou_jogador = 1;
				}

				else draw_sprite(obstacle);

				if(check_collision(jogador,obstacle))
					gameover = 1;

				temp = temp->before;
			}

			if(!animou_jogador)
				animate_sprite(jogador);

			copy_to_videomem();
			need_to_draw = -1;
		}

		if (need_to_draw == 1)
		{
			int desenhou_jogador = 0;
			struct list_element * temp = lista->back;

			while(temp != NULL && !gameover)
			{
				Sprite * obstacle = temp->element;

				if(!desenhou_jogador && (jogador->y + jogador->height) <= (obstacle->y + obstacle->height))
				{
					draw_sprite(jogador);
					animate_sprite(obstacle);
					desenhou_jogador = 1;
				}

				else animate_sprite(obstacle);

				if(check_collision(jogador,obstacle))
					gameover = 1;

				temp = temp->before;
			}

			if(!desenhou_jogador)
				draw_sprite(jogador);

			copy_to_videomem();
			need_to_draw = -1;
		}

		while(lista->front != NULL)
		{
			Sprite * temp = lista->front->element;

			if(temp->x + temp->width < 0)
			{
				destroy_sprite(temp);
				pop_front_list(lista);
			}

			else break;
		}

	} while(scancode != ESC_RELEASE && !gameover); // stops when it receives the ESC key release breakcode or the game is over

	while(lista->front != NULL)
	{
		Sprite * temp = lista->front->element;
		destroy_sprite(temp);
		pop_front_list(lista);
	}

	delete_list(lista);

	destroy_sprite(jogador);


	global_timer_counter = 0;
	global_time = 0;
	scancode = 0;
	array_pos = 0;

	return 0;
}
