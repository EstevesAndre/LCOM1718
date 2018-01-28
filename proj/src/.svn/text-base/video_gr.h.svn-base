#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "high_score.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);


 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);


/**
 * @brief draws a line segment with the specified end points and the input color
 *
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 * @param xf horizontal coordinate of "last" endpoint, starts at 0 (leftmost pixel)
 * @param yf vertical coordinate of "last" endpoint, starts at 0 (top pixel)
 * @param color color of the line segment to draw
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);


/**
 * @brief paint the specified pixel with a given color
 *
 * @param x horizontal coordinate of the particular pixel, starts at 0 (leftmost pixel)
 * @param y vertical coordinate of the particular pixel, starts at 0 (top pixel)
 * @param color color to set the pixel
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_pixel_color(unsigned short x, unsigned short y, unsigned short color);


/**
 * @brief Gets the color of the specific pixel
 *
 * @param x coordinate x
 *
 * @param y coordinate y
 *
 * @param pixel pixel to get the color
 *
 * @return Return void
 */
void vg_get_color(unsigned short x, unsigned short y, char *pixel);


/**
 * @brief draws the xpm on the screen
 *
 * @param *xpm[] xpm to be drawn
 *
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)
 *
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 *
 * @return 0 upon success, non-zero upon failure
 */
int video_draw_xpm(char *xpm[], unsigned short xi, unsigned short yi);


/**
 * @brief Gets the horizontal resolution of the graphics mode
 *
 * @return horizontal resolution
 */
unsigned get_h_resolution();


/**
 * @brief Gets the vertical resolution of the graphics mode
 *
 * @return vertical resolution
 */
unsigned get_v_resolution();


/**
 * @brief Updates the time of the game every second
 *
 * @param tempo Time of the game, starting at zero (0)
 *
 * @return Return void
 */
void display_time(int tempo);


/**
 * @brief Updates the score every second
 *
 * @param tempo Time of the game, starting at zero (0)
 *
 * @param hit_obs if hit_obs its added 300 points on the score
 *
 * @return Return void
 */
void display_points(int tempo, int hit_obs);


/**
 * @brief Draws Initial stuff for the game play
 *
 * @param blue Depending on the clock (RTC) the colors of the background are different
 *
 * @return Return void
 */
void draw_inicial_stuff(int blue);


/**
 * @brief Draws Initial Menu
 *
 * Draws "AVOID IT"
 *
 * Draws "Start Game"
 *
 * Draws "High Score"
 *
 * Draws "Quit Game"
 *
 * @return Return void
 */
void draw_main_menu();


/**
 * @brief Draws High Score Menu
 *
 * Draws "High Score"
 *
 * Calls the function to draw the 10 best scores
 *
 * Draws "Back to Main Menu"
 *
 * @return Return void
 */
void draw_high_score_menu();


/**
 * @brief Draws Game Over Menu
 *
 * Draws "GAME OVER !"
 * Draws "Back to Main Menu" and "Quit Game"
 *
 * @param great_score if 0 is done an animation, otherwise is asked to write your name to be stored in the high scores
 *
 * @return Return void
 */
void draw_game_over_menu(int great_score);


/**
 * Draws a rectangle (border) of size 3 in the given parameters
 *
 * @param xi top left and bottom left x coordinate
 *
 * @param yi top left and top right y coordinate
 *
 * @param xf top right and bottom right x coordinate
 *
 * @param yf bottom right and bottom left y coordinate
 *
 * @param color color of the border
 *
 * @return Return void
 */
void draw_border(short xi, short yi, short xf, short yf, unsigned long color);


/**
 * @brief Paints the all screen in black (color 0)
 *
 * @return Return void
 */
void fill_black();


/**
 * Draws the high_score to be displayed in High Score Menu
 *
 * @param high_scores parameters of each High Score
 *
 * @return Return void
 */
void draw_high_score(High_Score high_scores[]);


/**
 * @brief Gets the final score when the game is over
 *
 * @return Returns final score
 */
int get_final_score();

char *read_xpm(char *map[], int *wd, int *ht);


/**
 * @brief Copies video data from second buffer to main buffer
 */
void copy_to_videomem();


/**
 * @brief Used when an obstacle tries to draw what was on screen before is what was drawn, checks if it is not drawing in a player pixel
 *
 * @param x x pixel position
 *
 * @param y y pixel position
 *
 * @param color pixel color
 *
 * @return Return 0 upon sucess
 */
int draw_if_not_player(unsigned short x, unsigned short y, unsigned short color);


 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
