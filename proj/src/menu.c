#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "menu.h"

#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "video_gr.h"
#include "rtc.h"
#include "game.h"
#include "high_score.h"
#include "mouses.h"
#include "characters_name.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char array[3];
unsigned char array_pos;
unsigned char scancode;

static short coordinate_x_mouse = 1;
static short coordinate_y_mouse = 1;

static short mouse_Click = 0;
static int press = 0;

static High_Score high_scores[10];

state_t actual_State;
event_t actual_Event;

void updateMousePosition(unsigned char *packet){

	unsigned h_res = get_h_resolution(), v_res = get_v_resolution();

	if (packet[0] & XSIGN)
		coordinate_x_mouse -= (unsigned char) (~packet[1]) + 1;

	else
		coordinate_x_mouse += packet[1];

	if (packet[0] & YSIGN)
		coordinate_y_mouse += (unsigned char) (~packet[2]) + 1;

	else
		coordinate_y_mouse -= packet[2];

	if(coordinate_x_mouse < 0) coordinate_x_mouse = 0;

	if(coordinate_x_mouse > h_res - 3) coordinate_x_mouse = h_res - 3;

	if(coordinate_y_mouse < 0) coordinate_y_mouse = 0;

	if(coordinate_y_mouse > v_res - 4) coordinate_y_mouse = v_res - 4;

}


void mouse_menu_handler(unsigned char *packet, Mouse_Sprite * rato_1, Mouse_Sprite * rato_2, Mouse_Sprite * rato_3){

	mouse_Click = LB & packet[0];

	updateMousePosition(packet);

	draw_clean_mouse_sprite(rato_1);
	draw_clean_mouse_sprite(rato_2);
	draw_clean_mouse_sprite(rato_3);

	if (coordinate_x_mouse > 330 && coordinate_x_mouse < 690 && coordinate_y_mouse > 280 && coordinate_y_mouse < 350){

		// START GAME on MAIN MENU
		if (actual_State == STATE_INITIAL_MENU){
			if(mouse_Click){
				actual_Event = Game_Play_Menu_Pressed;
			}
			else
				actual_Event = Game_Play_Menu_Border;
			return;
		}


	}
	if (coordinate_x_mouse > 343 && coordinate_x_mouse < 681 && coordinate_y_mouse > 420 && coordinate_y_mouse < 495){

		// HIGH SCORE on MAIN MENU
		if (actual_State == STATE_INITIAL_MENU){
			if(mouse_Click){
				actual_Event = High_Score_Menu_Pressed;
			}
			else
				actual_Event = High_Score_Menu_Border;
			return;
		}

	}
	if (coordinate_x_mouse > 346 && coordinate_x_mouse < 685 && coordinate_y_mouse > 560 && coordinate_y_mouse < 638){

		// QUIT GAME on MAIN MENU
		if (actual_State == STATE_INITIAL_MENU){
			if (mouse_Click){
				actual_Event = Exit_Game_Pressed;
			}
			else
				actual_Event = Exit_Game_Border;
			return;
		}

	}
	if (coordinate_x_mouse > 304 && coordinate_x_mouse < 720 && coordinate_y_mouse > 660 && coordinate_y_mouse < 714){

		// BACK TO MAIN MENU on HIGH SCORE
		if (actual_State == STATE_HIGH_SCORE_MENU){
			if(mouse_Click){
				actual_Event = Back_Main_Menu_Pressed;
			}
			else
				actual_Event = Back_Main_Menu_Border;
			return;
		}

	}
	if ( coordinate_y_mouse > 663 && coordinate_y_mouse < 719 && coordinate_x_mouse > 50 && coordinate_x_mouse < 497){

		// BACK TO MAIN MAIN on GAME OVER
		if (actual_State == STATE_GAME_OVER_MENU){
			if(mouse_Click){
				actual_Event = Back_Main_Menu_Pressed;
			}
			else
				actual_Event = Back_Main_Menu_Border;
			return;
		}

	}
	if (coordinate_x_mouse > 690 && coordinate_x_mouse < 980 && coordinate_y_mouse > 660 && coordinate_y_mouse < 720){

		// QUIT GAME on GAME OVER
		if (actual_State == STATE_GAME_OVER_MENU){
			if(mouse_Click){
				actual_Event = Exit_Game_Pressed;
			}
			else
				actual_Event = Exit_Game_Border;
			return;
		}

	}
	if (coordinate_x_mouse > 310 && coordinate_x_mouse < 950 && coordinate_y_mouse > 500 && coordinate_y_mouse < 527)
	{
		//change Mouse on GAME OVER MENU
		if (actual_State == STATE_GAME_OVER_MENU){
			if(mouse_Click)
				actual_Event = Your_Name_Pressed;
			else
				actual_Event = Your_Name_Border;
			return;
		}
	}
	else
		actual_Event = Do_Nothing;

	return;
}

int menu_handler(Mouse_Sprite * rato_1, Mouse_Sprite * rato_2, Mouse_Sprite * rato_3){


	draw_clean_mouse_sprite(rato_1);
	draw_clean_mouse_sprite(rato_2);
	draw_clean_mouse_sprite(rato_3);

	if (actual_State == STATE_INITIAL_MENU && actual_Event == Do_Nothing)
	{
		// ensure that all borders are black (0)
		draw_border(333,560,690, 638,0);
		draw_border(319, 280, 706,350,0);
		draw_border(327,420,698,495,0);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_1,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_1);
	}
	else if (actual_State == STATE_INITIAL_MENU && (actual_Event == Game_Play_Menu_Pressed || actual_Event == Game_Play_Menu_Border) )
	{
		//draw border in start game (main menu)
		draw_border(319, 280, 706,350,54);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_3);
	}
	else if (actual_State == STATE_INITIAL_MENU && (actual_Event == High_Score_Menu_Pressed || actual_Event == High_Score_Menu_Border) )
	{
		// draw border in high score (main menu)
		draw_border(327,420,698,495,54);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);

		animate_mouse_sprite(rato_3);
	}
	else if (actual_State == STATE_INITIAL_MENU && (actual_Event == Exit_Game_Pressed || actual_Event == Exit_Game_Border) )
	{

		// draw border in quit game (main menu)
		draw_border(333,560,690, 638,54);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_3);
	}
	else if (actual_State == STATE_HIGH_SCORE_MENU && actual_Event == Do_Nothing)
	{
		// ensure that the border is black (0)
		draw_border(292,660,733,714,0);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_1,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_1);
	}
	else if (actual_State == STATE_HIGH_SCORE_MENU && (actual_Event == Back_Main_Menu_Pressed || actual_Event == Back_Main_Menu_Border) )
	{
		// draw border in back to main menu (high score)
		draw_border(292,660,733,714,32);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);
		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_3);
	}
	else if (actual_State == STATE_GAME_OVER_MENU && actual_Event == Do_Nothing)
	{
		// ensure that all borders are black (0)
		draw_border(50,663,497,719,0);
		draw_border(690,660,980,720,0);

		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);

		// draw Mouse
		set_coordenates(rato_1,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_1);
	}
	else if (actual_State == STATE_GAME_OVER_MENU && (actual_Event == Back_Main_Menu_Pressed || actual_Event == Back_Main_Menu_Border) )
	{
		// draw border in back to main menu (game over menu)
		draw_border(50,663,497,719,32);

		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);

		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_3);
	}
	else if (actual_State == STATE_GAME_OVER_MENU && (actual_Event == Exit_Game_Pressed || actual_Event == Exit_Game_Border) )
	{
		// draw border in exit game (game over menu)
		draw_border(690,660,980,720,32);

		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);

		// draw Mouse
		set_coordenates(rato_3,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_3);
	}
	else if(actual_State == STATE_GAME_OVER_MENU && (actual_Event == Your_Name_Border || actual_Event == Your_Name_Pressed))
	{

		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);

		// draw Mouse
		set_coordenates(rato_2,coordinate_x_mouse, coordinate_y_mouse);
		animate_mouse_sprite(rato_2);

	}
	copy_to_videomem();

	mouse_Click = 0;

	if (actual_State == STATE_INITIAL_MENU && actual_Event == Game_Play_Menu_Pressed){

		actual_State = STATE_GAME_OVER_MENU;
		actual_Event = Do_Nothing;

		unsigned long hora = rtc_get_hour();

		if(hora >= 0x8 && hora < 0x20)
			game(1);

		else
			game(0);

		int timer_irq_set = BIT(0);
		int kbd_irq_set = BIT(1);
		int mouse_irq_set = BIT(2);

		// gets the final score of the game
		int final_score = get_final_score();

		fill_black();


		if(final_score > high_scores[9].score)
		{
			draw_game_over_menu(1);
			copy_to_videomem();
			set_clean_mouse_sprite(rato_1);

			set_clean_mouse_sprite(rato_2);

			set_clean_mouse_sprite(rato_3);

			High_Score novo_score;

			novo_score.score = final_score;
			rtc_get_date(novo_score.date);

			char nome[20] = {'\0'};
			int nome_pos = 0;

			int ipc_status;
			message msg;


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
						if (msg.NOTIFY_ARG & timer_irq_set) //Timer not used here, do nothing
						{}

						if (msg.NOTIFY_ARG & kbd_irq_set) //Keyboard not used here, only read scancode and ignore it
						{
							kbd_int_handler();
							scancode = 0;
						}

						if (msg.NOTIFY_ARG & mouse_irq_set)
						{
							mouse_int_handler();

							if (array_pos == 3)
							{
								array_pos = 0;
								mouse_menu_handler(array,rato_1,rato_2,rato_3);
							}
						}
						break;
					default:
						continue; // no other notifications expected: do nothing
					}
				}
				if(array_pos == 0)
					menu_handler(rato_1,rato_2,rato_3);

				printf("%s",nome);

			} while(press == 0 && actual_State == STATE_GAME_OVER_MENU && (actual_Event == Do_Nothing || actual_Event == Your_Name_Border || actual_Event == Your_Name_Pressed));

			array_pos = 0;

			if(press){
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
							if (msg.NOTIFY_ARG & timer_irq_set) //Timer not used here, do nothing
							{}
							if (msg.NOTIFY_ARG & mouse_irq_set)
							{
								mouse_int_handler();

								if (array_pos == 3)
								{
									array_pos = 0;
									mouse_menu_handler(array,rato_1,rato_2,rato_3);
								}
							}
							if (msg.NOTIFY_ARG & kbd_irq_set)
							{
								kbd_int_handler();

								char caracter;

								// scancode
								if (scancode == 0x0e)
								{
									if(nome_pos > 0)
									{
										nome_pos--;
										nome[nome_pos] = '\0';
									}
								}

								else
								{
									caracter = get_caracter();

									if(caracter == '\0')
										break;

									if(nome_pos < 18)
									{
										nome[nome_pos] = caracter;
										nome_pos++;
									}
								}

								draw_name(nome);
								copy_to_videomem();
							}
							break;
						default:
							continue; // no other notifications expected: do nothing
						}
					}
					if(array_pos == 0)
						menu_handler(rato_1,rato_2,rato_3);

					printf("%s",nome);

				} while(scancode != ENTER_RELEASE && actual_State == STATE_GAME_OVER_MENU);

				unsigned int j = 0;

				for(;j < 20; j++)
				{
					novo_score.name[j] = nome[j];
				}

				insert_sort_high_score(high_scores,novo_score);

				array_pos = 0;
				scancode = 0;
			}
			press = 0;
		}

		else{
			draw_game_over_menu(0);

			set_clean_mouse_sprite(rato_1);

			set_clean_mouse_sprite(rato_2);

			set_clean_mouse_sprite(rato_3);
		}
	}
	else if (actual_State == STATE_INITIAL_MENU && actual_Event == High_Score_Menu_Pressed){
		actual_Event = Do_Nothing;
		actual_State = STATE_HIGH_SCORE_MENU;
		fill_black();
		draw_high_score_menu();
		draw_high_score(high_scores);
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);


	}

	else if (actual_State == STATE_INITIAL_MENU && actual_Event == Exit_Game_Pressed){
		actual_Event = Do_Nothing;
		actual_State = EXIT_GAME;
	}

	else if (actual_State == STATE_HIGH_SCORE_MENU && actual_Event == Back_Main_Menu_Pressed){
		actual_Event = Do_Nothing;
		actual_State = STATE_INITIAL_MENU;
		fill_black();
		draw_main_menu();
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);

	}

	else if (actual_State == STATE_GAME_OVER_MENU && actual_Event == Your_Name_Pressed){
		press = 1;
	}

	else if (actual_State == STATE_GAME_OVER_MENU && actual_Event == Back_Main_Menu_Pressed){
		actual_Event = Do_Nothing;
		actual_State = STATE_INITIAL_MENU;
		fill_black();
		draw_main_menu();
		set_clean_mouse_sprite(rato_1);

		set_clean_mouse_sprite(rato_2);

		set_clean_mouse_sprite(rato_3);


	}
	else if (actual_State == STATE_GAME_OVER_MENU && actual_Event == Exit_Game_Pressed){
		actual_Event = Do_Nothing;
		actual_State = EXIT_GAME;
	}

	copy_to_videomem();

	return 0;
}


int start_menu()
{
	if(read_high_scores("/home/lcom/proj/src/high_score.txt",high_scores,10))
		return 1;

	if (vg_init (0x105) == NULL)
	{
		vg_exit();
		return 1;
	}

	int ipc_status;
	message msg;

	actual_State = STATE_INITIAL_MENU;
	actual_Event = Do_Nothing;

	fill_black();
	draw_main_menu();
	copy_to_videomem();

	Mouse_Sprite * rato_1 = create_mouse_sprite(mouse_1,1023,767);
	Mouse_Sprite * rato_2 = create_mouse_sprite(mouse_2,1023,767);
	Mouse_Sprite * rato_3 = create_mouse_sprite(mouse_3,1023,767);


	int timer_irq_set;
	int kbd_irq_set;
	int mouse_irq_set;

	if(mouse_stream_mode())
		return 1;

	if ((mouse_irq_set = mouse_subscribe_int()) == -1)
		return 1;

	if ((kbd_irq_set = kbd_subscribe_int()) == -1)
		return 1;

	if ((timer_irq_set = timer_subscribe_int()) == -1)
		return 1;


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
				if (msg.NOTIFY_ARG & timer_irq_set) //Timer not used here, do nothing
				{}

				if (msg.NOTIFY_ARG & kbd_irq_set) //Keyboard not used here, only read scancode and ignore it
				{
					kbd_int_handler();
					scancode = 0;
				}

				if (msg.NOTIFY_ARG & mouse_irq_set)
				{
					mouse_int_handler();

					if (array_pos == 3)
					{
						array_pos = 0;
						mouse_menu_handler(array,rato_1,rato_2,rato_3);
					}
				}
				break;
			default:
				continue; // no other notifications expected: do nothing
			}
		}
		if(array_pos == 0)
			menu_handler(rato_1,rato_2,rato_3);

	} while(actual_State != EXIT_GAME);

	if (kbd_unsubscribe_int() == 1)
		return 1;

	if (timer_unsubscribe_int() == 1)
		return 1;

	if (mouse_disable_data_report() == 1)
		return 1;

	if (mouse_unsubscribe_int() == 1)
		return 1;

	destroy_mouse_sprite(rato_1);
	destroy_mouse_sprite(rato_2);
	destroy_mouse_sprite(rato_3);

	vg_exit();

	if(save_high_scores("/home/lcom/proj/src/high_score.txt",high_scores,10))
		return 1;

	return 0;
}

char get_caracter(){
	char ret;

	/*
	case 0x0e: //backspace
	break;
	 */

	switch(scancode)
	{
	case 0x39:	// Space
		ret = ' ';
		break;

	case 0x1e:  // Name_letter_A
		ret = 'A';
		break;

	case 0x30:  // Name_letter_B
		ret = 'B';
		break;

	case 0x2e:  // Name_letter_C
		ret = 'C';
		break;

	case 0x20:  // Name_letter_D
		ret = 'D';
		break;

	case 0x12:  // Name_letter_E
		ret = 'E';
		break;

	case 0x21:  // Name_letter_F
		ret = 'F';
		break;

	case 0x22:  // Name_letter_G
		ret = 'G';
		break;

	case 0x23:  // Name_letter_H
		ret = 'H';
		break;

	case 0x17:  // Name_letter_I
		ret = 'I';
		break;

	case 0x24:  // Name_letter_J
		ret = 'J';
		break;

	case 0x25:  // Name_letter_K
		ret = 'K';
		break;

	case 0x26: 	// Name_letter_L
		ret = 'L';
		break;

	case 0x32:    // Name_letter_M
		ret = 'M';
		break;

	case 0x31:    // Name_letter_N
		ret = 'N';
		break;

	case 0x18:    // Name_letter_O
		ret = 'O';
		break;

	case 0x19:    // Name_letter_P
		ret = 'P';
		break;

	case 0x10:    // Name_letter_Q
		ret = 'Q';
		break;

	case 0x13:    // Name_letter_R
		ret = 'R';
		break;

	case 0x1f:    // Name_letter_S
		ret = 'S';
		break;

	case 0x14:    // Name_letter_T
		ret = 'T';
		break;

	case 0x16:    // Name_letter_U
		ret = 'U';
		break;

	case 0x2f:    // Name_letter_V
		ret = 'V';
		break;

	case 0x11:    // Name_letter_W
		ret = 'W';
		break;

	case 0x2d:    // Name_letter_X
		ret = 'X';
		break;

	case 0x15:    // Name_letter_Y
		ret = 'Y';
		break;

	case 0x2c:    // Name_letter_Z
		ret = 'Z';
		break;
	default: ret = '\0';
	break;
	}
	return ret;
}

void draw_name(char nome[20]){

	int i = 0, x = 315; // y = 504

	for(; i < 20; i++)
	{
		switch(nome[i])
		{
		case 'a':
		case 'A': video_draw_xpm(Name_letter_A, x + i * 25, 504);
		break;
		case 'b':
		case 'B': video_draw_xpm(Name_letter_B, x + i * 25, 504);
		break;
		case 'c':
		case 'C': video_draw_xpm(Name_letter_C, x + i * 25, 504);
		break;
		case 'd':
		case 'D': video_draw_xpm(Name_letter_D, x + i * 25, 504);
		break;
		case 'e':
		case 'E': video_draw_xpm(Name_letter_E, x + i * 25, 504);
		break;
		case 'f':
		case 'F': video_draw_xpm(Name_letter_F, x + i * 25, 504);
		break;
		case 'g':
		case 'G': video_draw_xpm(Name_letter_G, x + i * 25, 504);
		break;
		case 'h':
		case 'H': video_draw_xpm(Name_letter_H, x + i * 25, 504);
		break;
		case 'i':
		case 'I': video_draw_xpm(Name_letter_I, x + i * 25, 504);
		break;
		case 'j':
		case 'J': video_draw_xpm(Name_letter_J, x + i * 25, 504);
		break;
		case 'k':
		case 'K': video_draw_xpm(Name_letter_K, x + i * 25, 504);
		break;
		case 'l':
		case 'L': video_draw_xpm(Name_letter_L, x + i * 25, 504);
		break;
		case 'm':
		case 'M': video_draw_xpm(Name_letter_M, x + i * 25, 504);
		break;
		case 'n':
		case 'N': video_draw_xpm(Name_letter_N, x + i * 25, 504);
		break;
		case 'o':
		case 'O': video_draw_xpm(Name_letter_O, x + i * 25, 504);
		break;
		case 'p':
		case 'P': video_draw_xpm(Name_letter_P, x + i * 25, 504);
		break;
		case 'q':
		case 'Q': video_draw_xpm(Name_letter_Q, x + i * 25, 504);
		break;
		case 'r':
		case 'R': video_draw_xpm(Name_letter_R, x + i * 25, 504);
		break;
		case 's':
		case 'S': video_draw_xpm(Name_letter_S, x + i * 25, 504);
		break;
		case 't':
		case 'T': video_draw_xpm(Name_letter_T, x + i * 25, 504);
		break;
		case 'u':
		case 'U': video_draw_xpm(Name_letter_U, x + i * 25, 504);
		break;
		case 'v':
		case 'V': video_draw_xpm(Name_letter_V, x + i * 25, 504);
		break;
		case 'w':
		case 'W': video_draw_xpm(Name_letter_W, x + i * 25, 504);
		break;
		case 'x':
		case 'X': video_draw_xpm(Name_letter_X, x + i * 25, 504);
		break;
		case 'y':
		case 'Y': video_draw_xpm(Name_letter_Y, x + i * 25, 504);
		break;
		case 'z':
		case 'Z': video_draw_xpm(Name_letter_Z, x + i * 25, 504);
		break;
		case '\0':
		case ' ': video_draw_xpm(Name_space, x + i * 25, 504);
		break;
		default: break;
		}
	}

}
