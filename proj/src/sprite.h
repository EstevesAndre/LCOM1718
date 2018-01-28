#ifndef __SPRITE_H
#define __SPRITE_H

/**
 * @brief Sprite
 */
typedef struct {
	int jogador; 			// 1 if sprite is the player, 2 if obstacle
	int x, y; 				// current position
	int width, height;  	// dimensions
	int xspeed, yspeed; 	// current speed
	char *map;          	// the pixmap
	char *clean;        	// what was on the screen before printing the sprite
} Sprite;


/**
 * @brief Creates a new sprite with pixmap "pic", with specified
 *
 * position (within the screen limits) and speed;
 *
 * Does not draw the sprite on the screen
 *
 * @param pic[] pixmap
 *
 * @param x Initial x position
 *
 * @param y Initial y position
 *
 * @param xspeed Initial x velocity
 *
 * @param yspeed Initial y velocity
 *
 * @param jogador 1 if played, 2 if obstacle
 *
 * @return Return NULL on invalid pixmap and Sprite upon success
 */
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed, int jogador);


/**
 * @brief Sets the speed of the Sprite
 *
 * @param sp Sprite to be set
 *
 * @param xspeed coordinate x speed
 *
 * @param yspeed coordinate y speed
 *
 * @return Return void
 */
void set_speed(Sprite *sp, int xspeed, int yspeed);


/**
 * @brief frees the allocated memory by the Sprite
 *
 * @param sp Respective Sprite
 *
 * @return Return void
 */
void destroy_sprite(Sprite *sp);


/**
 * @brief Draws the Sprite
 *
 * @param sp Respective Sprite
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_sprite(Sprite *sp);


/**
 * @brief Animates the Sprite
 *
 * Draws the previous background where was the Sprite
 * Saves the background behind the Sprite
 * Draws the Sprite
 *
 * @param sp Respective Sprite
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int animate_sprite(Sprite *sp);


#endif /* __SPRITE_H */
