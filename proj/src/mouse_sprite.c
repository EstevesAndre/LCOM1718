
#include <stdio.h>
#include <stdlib.h>
#include "mouse_sprite.h"
#include "video_gr.h"

#define H_RES           1024
#define V_RES		  	768

char *read_xpm(char *map[], int *wd, int *ht);

Mouse_Sprite *create_mouse_sprite(char *pic[], int x, int y){
	Mouse_Sprite * msp = (Mouse_Sprite*) malloc (sizeof(Mouse_Sprite));

	if ( msp == NULL)
		return NULL;

	msp->map = read_xpm(pic, &(msp->width), &(msp->height));
	if (msp->map == NULL){
		free(msp);
		return NULL;
	}

	msp->clean = malloc(msp->width * msp->height);

	*msp->clean = -1;

	msp->x = x;
	msp->y = y;
	msp->valx = x;
	msp->valy = y;

	return msp;
}

void destroy_mouse_sprite(Mouse_Sprite * msp){
	if (msp == NULL)
		return;
	if (msp->map)
		free(msp->map);
	if (msp->clean)
		free(msp->clean);

	free(msp);
	msp = NULL;
}


int animate_mouse_sprite(Mouse_Sprite * msp){

	if (*msp->clean == -1)
	{
		set_clean_mouse_sprite(msp);

		draw_mouse_sprite(msp);
	}

	//Mover-se
	else
	{
		draw_clean_mouse_sprite(msp);
		msp->x = msp->valx;
		msp->y = msp->valy;

		set_clean_mouse_sprite(msp);
		draw_mouse_sprite(msp);
	}
	return 0;
}

int draw_mouse_sprite(Mouse_Sprite * msp){

	char *pixel = msp->map;
	int i = 0, j= 0;
	for(;j < msp->height; j++)
	{
		if ((msp->y + j) >= V_RES || (msp->y + j) < 0)
			break;
		for(;i < msp->width; i++, pixel++)
		{
			if((msp->x + i) >= H_RES || (msp->x + i) < 0)
				continue;

			vg_pixel_color(msp->x+i,msp->y+j,*pixel);
		}
		i = 0;
	}

	return 0;

}

void set_coordenates(Mouse_Sprite * msp, int valx, int valy){
	msp->valx = valx;
	msp->valy = valy;
}

int set_clean_mouse_sprite(Mouse_Sprite * msp)
{
	char *pixel = msp->clean;
	int i = 0, j = 0;

	for(;j < msp->height; j++)
	{
		if ((msp->y + j) >= V_RES || (msp->y + j) < 0)
			break;
		for(;i < msp->width; i++, pixel++)
		{
			if((msp->x + i) >= H_RES || (msp->x + i) < 0)
				continue;

			vg_get_color(msp->x+i,msp->y+j,pixel);
		}
		i = 0;
	}

	return 0;
}

int draw_clean_mouse_sprite(Mouse_Sprite * msp)
{
	char *pixel = msp->clean;
	int i = 0, j = 0;

	for(;j < msp->height; j++)
	{
		if ((msp->y + j) >= V_RES || (msp->y + j) < 0)
			break;
		for(;i < msp->width; i++, pixel++)
		{
			if((msp->x + i) >= H_RES || (msp->x + i) < 0)
				continue;

			vg_pixel_color(msp->x+i,msp->y+j,*pixel);
		}
		i = 0;
	}

	return 0;
}
