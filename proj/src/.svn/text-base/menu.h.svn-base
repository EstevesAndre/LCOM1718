#ifndef MENU_H
#define MENU_H

#include "mouse_sprite.h"

extern unsigned char array[3];
extern unsigned char array_pos;
extern unsigned char scancode;

/**
 * @brief Menus State Machine, state_t
 *
 * State Initial Menu, STATE_INITIAL_MENU
 *
 * State Game Play Menu, STATE_GAME_PLAY_MENU
 *
 * State Game Over Menu, STATE_GAME_OVER_MENU
 *
 * State High Score Menu, STATE_HIGH_SCORE_MENU
 *
 * State Exit Game, EXIT_GAME
 */
typedef enum {
	STATE_INITIAL_MENU, STATE_GAME_PLAY_MENU, STATE_GAME_OVER_MENU, STATE_HIGH_SCORE_MENU, EXIT_GAME
}state_t;

extern state_t actual_State;

/**
 * @brief Event of the State Machine, event_t
 *
 * Event Game_Play_Menu_Pressed, if Mouse position is between Start Game button border and left button is pressed
 *
 * Event Back_Main_Menu_Pressed, if Mouse position is between Back to Main button border and left button is pressed
 *
 * Event High_Score_Menu_Pressed, if Mouse position is between High Score button border and left button is pressed,
 *
 * Event Exit_Game_Pressed, if Mouse position is between Quit Game button border and left button is pressed
 *
 * Event Your_Name_Pressed, if Mouse position is between Your Name rectangle border and left button is pressed
 *
 * Event Game_Play_Menu_Border, if Mouse position is between Start Game button border
 *
 * Event Back_Main_Menu_Border, if Mouse position is between Back to Main button border
 *
 * Event High_Score_Menu_Border, if Mouse position is between High Score button border
 *
 * Event Exit_Game_Border, if Mouse position is between Quit Game button border
 *
 * Event Your_Name_Border, if Mouse position is between Your Name rectangle border
 *
 * Event Do_Nothing, if Mouse position is in the rest position of the screen
 */
typedef enum {
	Game_Play_Menu_Pressed, Back_Main_Menu_Pressed, High_Score_Menu_Pressed, Exit_Game_Pressed,
	Game_Play_Menu_Border, Back_Main_Menu_Border, High_Score_Menu_Border, Exit_Game_Border,
	Your_Name_Border, Your_Name_Pressed, Do_Nothing
}event_t;

extern event_t actual_Event;


/**
 * @brief Updates Mouse position
 *
 * @param packet packet sent by the mouse with the information of the movement
 *
 * Changes coordinate_x_mouse and coordinate_y_mouse
 *
 * @return Return void
 */
void updateMousePosition(unsigned char *packet);


/**
 * @brief Perform the changes in the State Machine actual_Event
 *
 * @param packet Receives the packet to check if the left button of the mouse was pressed
 *
 * @param rato_1 Mouse_Sprite to draw clean in the screen
 *
 * @param rato_2 Mouse_Sprite to draw clean in the screen
 *
 * @param rato_3 Mouse_Sprite to draw clean in the screen
 *
 * @return Return void
 */
void mouse_menu_handler(unsigned char *packet, Mouse_Sprite * rato_1, Mouse_Sprite * rato_2, Mouse_Sprite * rato_3);


/**
 * @brief Perform the changes in the State Machine actual_State
 *
 * Main function that relates every state and calls the right functions to be drawn in the different menus
 *
 * @param rato_1 Mouse_Sprite to draw clean and set clean in the screen
 *
 * @param rato_2 Mouse_Sprite to draw clean and set clean in the screen
 *
 * @param rato_3 Mouse_Sprite to draw clean and set clean in the screen
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int menu_handler(Mouse_Sprite * rato_1, Mouse_Sprite * rato_2, Mouse_Sprite * rato_3);


/**
 * @brief Initial function for the menu
 *
 * Initializes the video module in graphics mode
 *
 * Initial do while to circulate over the different menus
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int start_menu();


/**
 * @brief Draws the name in the Game Over Menu if the score was high enough
 *
 * @param nome Name to be drawn
 *
 * @return Return void
 */
void draw_name(char nome[20]);


/**
 * @brief switches the scancode given by the keyboard and set the char associated
 *
 * @return Returns the respective char
 */
char get_caracter();

#endif
