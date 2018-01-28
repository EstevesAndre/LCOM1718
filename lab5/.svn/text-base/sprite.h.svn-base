#ifndef __SPRITE_H
#define __SPRITE_H

typedef struct {
	int x, y; // current position
	int width, height;  // dimensions
	int xspeed, yspeed; // current speed
	char *map;          // the pixmap
} Sprite;


/** Creates a new sprite with pixmap "pic", with specified
*position (within the screen limits) and speed;
*Does not draw the sprite on the screen
*Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed);


/**
 * @brief frees allocated memory from sprite
 */
void destroy_sprite(Sprite *sp);


/**
 * @brief draws the next frame from the sprite
 *
 * @return 0 upon success, non-zero upon failure
 */
int animate_sprite(Sprite *sp);

#endif /* __SPRITE_H */
