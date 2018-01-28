#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "video_gr.h"
#include "video_test.h"

#define H_RES           1024
#define V_RES		  	768

char *read_xpm(char *map[], int *wd, int *ht);
static int draw_sprite(Sprite *sp);

Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed)
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

	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;

	return sp;
}


void destroy_sprite(Sprite *sp)
{
	if( sp == NULL )
		return;
	if( sp ->map )
		free(sp->map);
	free(sp);
	sp = NULL;     // XXX: pointer is passed by value
	//           should do this @ the caller
}

int animate_sprite(Sprite *sp)
{
	if (sp->xspeed >= 0)
		sp->x += sp->xspeed;

	else sp->x += 1;

	if (sp->y >= 0)
		sp->y += sp->yspeed;

	else sp->y += 1;

	video_draw_black();

	draw_sprite(sp);

	return 0;
}

static int draw_sprite(Sprite *sp)
{
	char *pixel = sp->map;
	int i = 0, j = 0;

	for(;j < sp->height; j++)
	{
		if ((sp->y + j) >= V_RES)
			break;
		for(;i < sp->width; i++, pixel++)
		{
			if((sp->x + i) >= H_RES)
				continue;

			vg_pixel_color(sp->x+i,sp->y+j,*pixel);
		}
		i = 0;
	}

	return 0;
}

