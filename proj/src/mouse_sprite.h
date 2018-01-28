#ifndef __MOUSE_SPRITE_H
#define __MOUSE_SPRITE_H

/**
 * @brief Mouse_Sprite
 */
typedef struct {
	int x, y;				// current position
	int valx, valy;			// new position
	int width, height;		// dimensions
	char *map;				// the pixmap
	char *clean;			// what was on the screen before printing the sprite
} Mouse_Sprite;


/**
 * @brief Creates a new sprite with pixmap "pic", with specified
 *
 * position (within the screen limits);
 *
 * Does not draw the sprite on the screen
 *
 * @param pic[] pixmap
 *
 * @param x Initial x position
 *
 * @param y Initial y position
 *
 * @return Returns NULL on invalid pixmap.
 */
Mouse_Sprite *create_mouse_sprite(char *pic[], int x, int y);


/**
 * @brief frees the allocated memory by the Mouse_Sprite
 *
 * @param msp Respective Sprite
 *
 * @return Return void
 */
void destroy_mouse_sprite(Mouse_Sprite * msp);


/**
 * @brief Draws the Mouse_Sprite
 *
 * @param msp Respective Mouse_Sprite
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_mouse_sprite(Mouse_Sprite * msp);


/**
 * @brief Animates the Mouse_Sprite
 *
 * Draws the previous background where was the Mouse_Sprite
 * Saves the background behind the Mouse_Sprite
 * Draws the Mouse_Sprite
 *
 * @param msp Respective Mouse_Sprite
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int animate_mouse_sprite(Mouse_Sprite * msp);


/**
 * @brief Updates the coordinates of the Mouse_Sprite
 *
 * @param msp Mouse_Sprite to be updated
 *
 * @param valx coordinate valx
 *
 * @param valy coordinate valy
 *
 * @return Return void
 */
void set_coordenates(Mouse_Sprite * msp, int valx, int valy);


/**
 * @brief Sets what was in the screen before Mouse_Sprite being displayed
 *
 * @param msp Mouse_Sprite to be clean set
 *
 * @return Return 0 upon success
 */
int set_clean_mouse_sprite(Mouse_Sprite * msp);


/**
 * @brief Displays what was in the screen before Mouse_Sprite being displayed
 *
 * @param msp Mouse_Sprite to be updated
 *
 * @return Return 0 upon success
 */
int draw_clean_mouse_sprite(Mouse_Sprite * msp);

#endif
