#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "video_gr.h"

#define H_RES           1024
#define V_RES		  	768

char *read_xpm(char *map[], int *wd, int *ht);
static int set_clean(Sprite *sp);
static int draw_clean(Sprite *sp);

Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed, int jogador)
{
	//allocate space for the "object"
	Sprite *sp = (Sprite*) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}

	sp->clean = malloc(sp->width * sp->height);

	* sp->clean = -1; //Inicializado com um valor negativo

	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
	sp->jogador = jogador;

	return sp;
}

void destroy_sprite(Sprite *sp)
{
	if( sp == NULL )
		return;
	if( sp ->map )
		free(sp->map);
	if( sp ->clean )
		free(sp->clean);
	free(sp);
	sp = NULL;     // XXX: pointer is passed by value
	//           should do this @ the caller
}

void set_speed(Sprite *sp, int xspeed, int yspeed)
{
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
}

int animate_sprite(Sprite *sp)
{
	//Primeira vez que é mostrada
	if(*sp->clean == -1)
	{
		set_clean(sp);

		sp->x += sp->xspeed;
		sp->y += sp->yspeed;

		draw_sprite(sp);
	}

	//Mostrar na mesma posicao onde já está a ser mostrada
	else if(sp->xspeed == 0 && sp->yspeed == 0)
		return 0;

	//Mover-se
	else
	{
		draw_clean(sp);

		sp->x += sp->xspeed;
		sp->y += sp->yspeed;

		set_clean(sp);

		draw_sprite(sp);
	}

	return 0;
}

int draw_sprite(Sprite *sp)
{
	char *pixel = sp->map;
	int i = 0, j = 0;

	for(;j < sp->height; j++)
	{
		if ((sp->y + j) >= V_RES || (sp->y + j) < 0)
			break;
		for(;i < sp->width; i++, pixel++)
		{
			if((sp->x + i) >= H_RES || (sp->x + i) < 0)
				continue;

			vg_pixel_color(sp->x+i,sp->y+j,*pixel);
		}
		i = 0;
	}

	return 0;
}

static int set_clean(Sprite *sp)
{
	char *pixel = sp->clean;
	int i = 0, j = 0;

	for(;j < sp->height; j++)
	{
		if ((sp->y + j) >= V_RES || (sp->y + j) < 0)
			break;
		for(;i < sp->width; i++, pixel++)
		{
			if((sp->x + i) >= H_RES || (sp->x + i) < 0)
				continue;

			if(sp->jogador)
			{
				char temp = *pixel;

				vg_get_color(sp->x+i,sp->y+j,pixel);

				if(*pixel == 0 || *pixel == 17 || *pixel == 28 || *pixel == 43 || *pixel == 48 || *pixel == 54)
					continue;

				else
				{
					*pixel = temp;
					continue;
				}
			}

			vg_get_color(sp->x+i,sp->y+j,pixel);
		}
		i = 0;
	}

	return 0;
}

static int draw_clean(Sprite *sp)
{
	char *pixel = sp->clean;
	int i = 0, j = 0;

	for(;j < sp->height; j++)
	{
		if ((sp->y + j) >= V_RES || (sp->y + j) < 0)
			break;
		for(;i < sp->width; i++, pixel++)
		{
			if((sp->x + i) >= H_RES || (sp->x + i) < 0)
				continue;

			if(sp->jogador == 2)
				if(*pixel == 0 || *pixel == 17 || *pixel == 28 || *pixel == 43 || *pixel == 48 || *pixel == 54)
				{
					draw_if_not_player(sp->x+i,sp->y+j,*pixel);
					continue;
				}


			vg_pixel_color(sp->x+i,sp->y+j,*pixel);
		}
		i = 0;
	}

	return 0;
}

