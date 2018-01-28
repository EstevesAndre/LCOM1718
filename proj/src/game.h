#ifndef GAME_H
#define GAME_H

#include "sprite.h"


/**
 * @brief Checks collisions between the player and the obstacle
 *
 * @param jogador Sprite player
 * @param obstaculo Sprite obstacle
 *
 * @return 0 if not collide and 1 otherwise
 */
int check_collision(Sprite *jogador, Sprite *obstaculo);


/**
 * @brief Main function of the game play
 *
 * @param  background_color Depending on the time (RTC), the game play has different colors
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int game(int background_color);

#endif
