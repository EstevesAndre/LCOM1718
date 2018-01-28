#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>
#include <math.h>
#include "video_gr.h"
#include "vbe.h"
#include "sprite.h"
#include "numbers.h"
#include "alphabet.h"
#include "characters.h"
#include "sprites_menu.h"
#include "high_score.h"


/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *second_buff;    /* For use in double buffer */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int global_times_hit = 0, global_levels = 0, global_final_score = 0;

#define x_ini 930
#define y_fixed 717

void* vg_init(unsigned short mode)
{
	vbe_mode_info_t info;

	vbe_get_mode_info(mode, &info);

	h_res = info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;

	struct reg86u reg86;

	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	reg86.u.b.intno = 0x10;

	if( sys_int86(&reg86) != OK )
	{
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	int r;
	struct mem_range mr;
	unsigned int vram_base = info.PhysBasePtr; /* VRAM’s physical addresss*/
	unsigned int vram_size = h_res * v_res * bits_per_pixel / 8; /* VRAM’s size, but you can use the frame-buffer size, instead */


	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;


	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
		panic("couldn’t map video memory");

	//For double buffering

	second_buff = (char*) malloc ( h_res * v_res * bits_per_pixel / 8);

	return video_mem;
}

int vg_exit()
{
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( second_buff )
		free(second_buff);

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void copy_to_videomem()
{
	memcpy(video_mem,second_buff,h_res*v_res*bits_per_pixel/8);
}

int vg_pixel_color(unsigned short x, unsigned short y, unsigned short color)
{
	if (color > 63)
		return 0;

	char *pixel = second_buff + (((y * h_res) + x) * bits_per_pixel / 8);

	*pixel = color;

	return 0;
}

void vg_get_color(unsigned short x, unsigned short y, char *pixel)
{
	char *pixel_screen = second_buff + (((y * h_res) + x) * bits_per_pixel / 8);

	*pixel = *pixel_screen;
}

unsigned get_h_resolution(){
	return h_res;
}

unsigned get_v_resolution(){
	return v_res;
}


int vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color)
{
	unsigned short x,y;

	//Vertical line or point
	if (xi == xf)
	{
		if (xi >= h_res)
			return 0;

		if ( yf >= yi)
		{
			for (y = yi ; y <= yf; y++)
			{
				if (y >= v_res)
					return 0;

				vg_pixel_color(xi,y,color);
			}
		}

		else
		{
			for (y = yf ; y >= yi; y--)
			{
				if (y >= v_res)
					continue;

				vg_pixel_color(xi,y,color);
			}
		}
	}

	//linha y = mx + b
	else
	{
		float m = (float) (yf - yi)/(xf - xi);
		float b = (float) (yi - (m * xi));

		if (xf > xi)
		{
			for (x = xi; x <= xf; x++)
			{
				if (x >= h_res)
					return 0;

				float y_temp = (m * x) + b;

				if (y_temp >= v_res)
					continue;

				vg_pixel_color(x,(unsigned short) y_temp, color);
			}
		}

		else
		{
			for (x = xf; x >= xi; x--)
			{
				if (x >= h_res)
					continue;

				float y_temp = (m * x) + b;

				if (y_temp >= v_res)
					continue;

				vg_pixel_color(x,(unsigned short) y_temp, color);
			}
		}
	}

	return 0;
}

int video_draw_xpm(char *xpm[], unsigned short xi, unsigned short yi)
{
	int width, height;
	char *map;

	// get the pix map from the XPM
	map = read_xpm(xpm, &width, &height);

	// copy it to graphics memory
	int i = 0, j = 0;

	for(;j < height; j++)
	{
		if ((yi + j) >= v_res)
			break;
		for(;i < width; i++, map++)
		{
			if((xi + i) >= h_res)
				continue;
			vg_pixel_color(xi + i, yi + j, *map);
		}
		i = 0;
	}

	return 0;
}


void display_time(int tempo)
{

	int hour_e = (int) ((int) ( ( (int)tempo / 60) / 60) % 24)/10, hour_d = (int) ((int) ( ( (int)tempo / 60)/ 60) % 24)%10;
	if (hour_e >= 2 && hour_d >= 4){
		hour_e %= 2;
		hour_d %= 4;
	}
	int min_e = (int)((int)((int)tempo / 60) % 60)/10, min_d = (int)((int)((int)tempo / 60) % 60)%10 ;

	int seg_e = (int)((int)tempo % 60)/10, seg_d = (int)((int)tempo % 60)%10;

	int posicao_x = x_ini + 70;
	int separador = 1;
	for(;separador <= 6; separador++){
		int indice = 0;
		if (separador == 6) indice = hour_e;
		if (separador == 5) indice = hour_d;
		if (separador == 4) indice = min_e;
		if (separador == 3) indice = min_d;
		if (separador == 2) indice = seg_e;
		if (separador == 1) indice = seg_d;


		switch(indice){
		case 0:
			video_draw_xpm(num_0, posicao_x, y_fixed);
			break;
		case 1:
			video_draw_xpm(num_1, posicao_x, y_fixed);
			break;
		case 2:
			video_draw_xpm(num_2, posicao_x, y_fixed);
			break;
		case 3:
			video_draw_xpm(num_3, posicao_x, y_fixed);
			break;
		case 4:
			video_draw_xpm(num_4, posicao_x, y_fixed);
			break;
		case 5:
			video_draw_xpm(num_5, posicao_x, y_fixed);
			break;
		case 6:
			video_draw_xpm(num_6, posicao_x, y_fixed);
			break;
		case 7:
			video_draw_xpm(num_7, posicao_x, y_fixed);
			break;
		case 8:
			video_draw_xpm(num_8, posicao_x, y_fixed);
			break;
		case 9:
			video_draw_xpm(num_9, posicao_x, y_fixed);
			break;
			//default:
			// do nothing;
		}

		posicao_x -= 20;
		if (separador % 2 == 0)
			posicao_x -= 10;
	}

}


void display_points(int tempo, int hit_obs){

	//function: x ^ 0.8 * x / 5
	float val = pow((float)tempo, 0.8) * (float)tempo / 5;

	//points for each hit on an obstacle, +300

	val += 300 * global_times_hit;
	if (hit_obs){
		global_times_hit++;
		val += 300;
	}

	val += global_levels * 300;
	if(tempo % 40 == 0){ // 40 in 40 seconds the player gets 300 points
		global_levels++;
		val += 300;
	}

	int points = (int)val % 999999; // resets the score when it's higher than 999.999

	global_final_score = (int)val;

	int posicao_x = 1000;

	do{
		int indice = points % 10;
		points /= 10;

		switch(indice){
		case 0:
			video_draw_xpm(num_0, posicao_x, y_fixed + 25);
			break;
		case 1:
			video_draw_xpm(num_1, posicao_x, y_fixed + 25);
			break;
		case 2:
			video_draw_xpm(num_2, posicao_x, y_fixed + 25);
			break;
		case 3:
			video_draw_xpm(num_3, posicao_x, y_fixed + 25);
			break;
		case 4:
			video_draw_xpm(num_4, posicao_x, y_fixed + 25);
			break;
		case 5:
			video_draw_xpm(num_5, posicao_x, y_fixed + 25);
			break;
		case 6:
			video_draw_xpm(num_6, posicao_x, y_fixed + 25);
			break;
		case 7:
			video_draw_xpm(num_7, posicao_x, y_fixed + 25);
			break;
		case 8:
			video_draw_xpm(num_8, posicao_x, y_fixed + 25);
			break;
		case 9:
			video_draw_xpm(num_9, posicao_x, y_fixed + 25);
			break;
		default:break;
			// do nothing;
		}

		posicao_x -= 20;

	}while(points > 0);

}



void draw_inicial_stuff(int background){

	int j,q = 936;

	// paints all pixels, if (background) grey (28), else blue (17)
	for(j = 0; j < 768;j++)
	{
		if(background)
			vg_draw_line(0,j,1023,j,28);
		else
			vg_draw_line(0,j,1023,j,17);
	}


	// paints top side background, light blue (43) in day light and black (0) otherwise
	for(j = 0; j < 468;j++)
	{
		if(background)
			vg_draw_line(0,j,q,j,43);
		else
			vg_draw_line(0,j,q,j,0);
		q-=2;
	}

	// draws the division of the "ground" (playable place) and the top side background in white (48)
	vg_draw_line(0,468,936,0,48);
	vg_draw_line(0,469,938,0,48);
	vg_draw_line(0,470,940,0,48);
	vg_draw_line(0,471,942,0,48);
	vg_draw_line(0,472,944,0,48);
	vg_draw_line(0,473,946,0,48);
	vg_draw_line(0,474,948,0,48);

	q = 350;

	// paints bottom side background, light blue (43) in day light and black (0) otherwise
	for(j = 768; j > 433; j--)
	{
		if(background)
			vg_draw_line(q,j,1024,j,43);
		else
			vg_draw_line(q,j,1024,j,0);
		q+=2;
	}

	// draws the division of the "ground" (playable place) and the bottom side background in white (48)
	vg_draw_line(342,768,1024,427,48);
	vg_draw_line(344,768,1024,428,48);
	vg_draw_line(346,768,1024,429,48);
	vg_draw_line(348,768,1024,430,48);
	vg_draw_line(350,768,1024,431,48);
	vg_draw_line(350,769,1024,432,48);
	vg_draw_line(350,770,1024,433,48);

	// paints a "color wheel" on the bottom when there's displayed the score and the time in yellow (54)
	for(j = 705; j < 768; j++){
		vg_draw_line(732,j,1023,j,54);
	}

	// paints the score and time place in black (0)
	for(j = 710; j < 768; j++){
		vg_draw_line(737,j,1023,j,0);
	}

	// draws "POINTS" on the score place in red (36)
	video_draw_xpm(letter_P, 744, y_fixed + 25);
	video_draw_xpm(letter_O, 767, y_fixed + 25);
	video_draw_xpm(letter_I, 790, y_fixed + 25);
	video_draw_xpm(letter_N, 813, y_fixed + 25);
	video_draw_xpm(letter_T, 836, y_fixed + 25);
	video_draw_xpm(letter_S, 859, y_fixed + 25);
	// draws ':' on the score place in yellow (54)
	video_draw_xpm(two_points, 886, y_fixed + 25);

	// draws initial score ( 000000 ) in green (26)
	video_draw_xpm(num_0, 900, y_fixed + 25);
	video_draw_xpm(num_0, 920, y_fixed + 25);
	video_draw_xpm(num_0, 940, y_fixed + 25);
	video_draw_xpm(num_0, 960, y_fixed + 25);
	video_draw_xpm(num_0, 980, y_fixed + 25);
	video_draw_xpm(num_0, 1000, y_fixed + 25);

	// draws "TIME" on the time place in red (36)
	video_draw_xpm(letter_T, 779, y_fixed);
	video_draw_xpm(letter_I, 801, y_fixed);
	video_draw_xpm(letter_M, 824, y_fixed);
	video_draw_xpm(letter_E, 847, y_fixed);
	// draws ':' on the time place in yellow (54)
	video_draw_xpm(two_points, 870, y_fixed);

	// draws initial time ( 00 : 00 : 00) in green (26)
	video_draw_xpm(num_0, 880, y_fixed);
	video_draw_xpm(num_0, 900, y_fixed);
	// draws ':' on the time place in yellow (54)
	video_draw_xpm(two_points, 920, y_fixed);
	video_draw_xpm(num_0, 930, y_fixed);
	video_draw_xpm(num_0, 950, y_fixed);
	// draws ':' on the time place in yellow (54)
	video_draw_xpm(two_points, 970, y_fixed);
	video_draw_xpm(num_0, 980, y_fixed);
	video_draw_xpm(num_0, 1000, y_fixed);
}


void draw_high_score(High_Score high_scores[])
{
	unsigned int i = 0;

	for(;i < 10; i++)
	{
		switch(i)
		{
		case 0:
			video_draw_xpm(num_1, 100, 100 + 50);
			break;
		case 1:
			video_draw_xpm(num_2, 100, 150 + 50);
			break;
		case 2:
			video_draw_xpm(num_3, 100, 200 + 50);
			break;
		case 3:
			video_draw_xpm(num_4, 100, 250 + 50);
			break;
		case 4:
			video_draw_xpm(num_5, 100, 300 + 50);
			break;
		case 5:
			video_draw_xpm(num_6, 100, 350 + 50);
			break;
		case 6:
			video_draw_xpm(num_7, 100, 400 + 50);
			break;
		case 7:
			video_draw_xpm(num_8, 100, 450 + 50);
			break;
		case 8:
			video_draw_xpm(num_9, 100, 500 + 50);
			break;
		case 9:
			video_draw_xpm(num_1, 80, 550 + 50);
			video_draw_xpm(num_0, 100, 550 + 50);
			break;
		default: break;
		}

		char temp_string[11] = {'\0'};

		sprintf(temp_string, "%10u", (unsigned int) high_scores[i].score, sizeof(temp_string));

		unsigned int j= 0;

		while (high_scores[i].name[j] != '\0' && j <= 20)
		{
			switch(high_scores[i].name[j])
			{
			case ' ': video_draw_xpm(space, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'a':
			case 'A': video_draw_xpm(letter_A, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'b':
			case 'B': video_draw_xpm(letter_B, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'c':
			case 'C': video_draw_xpm(letter_C, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'd':
			case 'D': video_draw_xpm(letter_D, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'e':
			case 'E': video_draw_xpm(letter_E, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'f':
			case 'F': video_draw_xpm(letter_F, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'g':
			case 'G': video_draw_xpm(letter_G, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'h':
			case 'H': video_draw_xpm(letter_H, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'i':
			case 'I': video_draw_xpm(letter_I, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'j':
			case 'J': video_draw_xpm(letter_J, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'k':
			case 'K': video_draw_xpm(letter_K, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'l':
			case 'L': video_draw_xpm(letter_L, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'm':
			case 'M': video_draw_xpm(letter_M, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'n':
			case 'N': video_draw_xpm(letter_N, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'o':
			case 'O': video_draw_xpm(letter_O, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'p':
			case 'P': video_draw_xpm(letter_P, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'q':
			case 'Q': video_draw_xpm(letter_Q, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'r':
			case 'R': video_draw_xpm(letter_R, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 's':
			case 'S': video_draw_xpm(letter_S, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 't':
			case 'T': video_draw_xpm(letter_T, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'u':
			case 'U': video_draw_xpm(letter_U, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'v':
			case 'V': video_draw_xpm(letter_V, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'w':
			case 'W': video_draw_xpm(letter_W, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'x':
			case 'X': video_draw_xpm(letter_X, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'y':
			case 'Y': video_draw_xpm(letter_Y, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			case 'z':
			case 'Z': video_draw_xpm(letter_Z, 160 + 25 * j, 100 + 50 * i + 50);
			break;
			default: break;
			}

			j++;
		}

		j = 0;

		while (temp_string[j] != '\0' && j <= 10)
		{
			switch(temp_string[j])
			{
			case '0': video_draw_xpm(num_0, 560 + 20 * j , 100 + 50 * i + 50);
			break;
			case '1': video_draw_xpm(num_1, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '2': video_draw_xpm(num_2, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '3': video_draw_xpm(num_3, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '4': video_draw_xpm(num_4, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '5': video_draw_xpm(num_5, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '6': video_draw_xpm(num_6, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '7': video_draw_xpm(num_7, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '8': video_draw_xpm(num_8, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			case '9': video_draw_xpm(num_9, 560 + 20 * j, 100 + 50 * i + 50);
			break;
			default: break;
			}

			j++;
		}

		//Date is in yyyy-mm-dd format, displayind as dd MONTH yy (last 2 digits)

		switch(high_scores[i].date[8])
		{
		case '0': video_draw_xpm(num_0, 800, 100 + 50 * i + 50);
		break;
		case '1': video_draw_xpm(num_1, 800, 100 + 50 * i + 50);
		break;
		case '2': video_draw_xpm(num_2, 800, 100 + 50 * i + 50);
		break;
		case '3': video_draw_xpm(num_3, 800, 100 + 50 * i + 50);
		break;
		case '4': video_draw_xpm(num_4, 800, 100 + 50 * i + 50);
		break;
		case '5': video_draw_xpm(num_5, 800, 100 + 50 * i + 50);
		break;
		case '6': video_draw_xpm(num_6, 800, 100 + 50 * i + 50);
		break;
		case '7': video_draw_xpm(num_7, 800, 100 + 50 * i + 50);
		break;
		case '8': video_draw_xpm(num_8, 800, 100 + 50 * i + 50);
		break;
		case '9': video_draw_xpm(num_9, 800, 100 + 50 * i + 50);
		break;
		default: break;
		}

		switch(high_scores[i].date[9])
		{
		case '0': video_draw_xpm(num_0, 820, 100 + 50 * i + 50);
		break;
		case '1': video_draw_xpm(num_1, 820, 100 + 50 * i + 50);
		break;
		case '2': video_draw_xpm(num_2, 820, 100 + 50 * i + 50);
		break;
		case '3': video_draw_xpm(num_3, 820, 100 + 50 * i + 50);
		break;
		case '4': video_draw_xpm(num_4, 820, 100 + 50 * i + 50);
		break;
		case '5': video_draw_xpm(num_5, 820, 100 + 50 * i + 50);
		break;
		case '6': video_draw_xpm(num_6, 820, 100 + 50 * i + 50);
		break;
		case '7': video_draw_xpm(num_7, 820, 100 + 50 * i + 50);
		break;
		case '8': video_draw_xpm(num_8, 820, 100 + 50 * i + 50);
		break;
		case '9': video_draw_xpm(num_9, 820, 100 + 50 * i + 50);
		break;
		default: break;
		}

		if(high_scores[i].date[5] == '1')
		{
			switch(high_scores[i].date[6])
			{
			case '0': video_draw_xpm(letter_O, 840, 100 + 50 * i + 50);
			video_draw_xpm(letter_C, 865, 100 + 50 * i + 50);
			video_draw_xpm(letter_T, 890, 100 + 50 * i + 50);
			break;
			case '1': video_draw_xpm(letter_N, 840, 100 + 50 * i + 50);
			video_draw_xpm(letter_O, 865, 100 + 50 * i + 50);
			video_draw_xpm(letter_V, 890, 100 + 50 * i + 50);
			break;
			case '2': video_draw_xpm(letter_D, 840, 100 + 50 * i + 50);
			video_draw_xpm(letter_E, 865, 100 + 50 * i + 50);
			video_draw_xpm(letter_C, 890, 100 + 50 * i + 50);
			break;
			default: break;
			}
		}

		else
		{
			switch(high_scores[i].date[6])
			{
			case '1':
				video_draw_xpm(letter_J, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_A, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_N, 890, 100 + 50 * i + 50);
				break;
			case '2':
				video_draw_xpm(letter_F, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_E, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_B, 890, 100 + 50 * i + 50);
				break;
			case '3':
				video_draw_xpm(letter_M, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_A, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_R, 890, 100 + 50 * i + 50);
				break;
			case '4':
				video_draw_xpm(letter_A, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_P, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_R, 890, 100 + 50 * i + 50);
				break;
			case '5':
				video_draw_xpm(letter_M, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_A, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_Y, 890, 100 + 50 * i + 50);
				break;
			case '6':
				video_draw_xpm(letter_J, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_U, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_N, 890, 100 + 50 * i + 50);
				break;
			case '7':
				video_draw_xpm(letter_J, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_U, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_L, 890, 100 + 50 * i + 50);
				break;
			case '8':
				video_draw_xpm(letter_A, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_U, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_G, 890, 100 + 50 * i + 50);
				break;
			case '9':
				video_draw_xpm(letter_S, 840, 100 + 50 * i + 50);
				video_draw_xpm(letter_E, 865, 100 + 50 * i + 50);
				video_draw_xpm(letter_P, 890, 100 + 50 * i + 50);
				break;
			default: break;
			}
		}

		switch(high_scores[i].date[2])
		{
		case '0': video_draw_xpm(num_0, 915, 100 + 50 * i + 50);
		break;
		case '1': video_draw_xpm(num_1, 915, 100 + 50 * i + 50);
		break;
		case '2': video_draw_xpm(num_2, 915, 100 + 50 * i + 50);
		break;
		case '3': video_draw_xpm(num_3, 915, 100 + 50 * i + 50);
		break;
		case '4': video_draw_xpm(num_4, 915, 100 + 50 * i + 50);
		break;
		case '5': video_draw_xpm(num_5, 915, 100 + 50 * i + 50);
		break;
		case '6': video_draw_xpm(num_6, 915, 100 + 50 * i + 50);
		break;
		case '7': video_draw_xpm(num_7, 915, 100 + 50 * i + 50);
		break;
		case '8': video_draw_xpm(num_8, 915, 100 + 50 * i + 50);
		break;
		case '9': video_draw_xpm(num_9, 915, 100 + 50 * i + 50);
		break;
		default: break;
		}

		switch(high_scores[i].date[3])
		{
		case '0': video_draw_xpm(num_0, 935, 100 + 50 * i + 50);
		break;
		case '1': video_draw_xpm(num_1, 935, 100 + 50 * i + 50);
		break;
		case '2': video_draw_xpm(num_2, 935, 100 + 50 * i + 50);
		break;
		case '3': video_draw_xpm(num_3, 935, 100 + 50 * i + 50);
		break;
		case '4': video_draw_xpm(num_4, 935, 100 + 50 * i + 50);
		break;
		case '5': video_draw_xpm(num_5, 935, 100 + 50 * i + 50);
		break;
		case '6': video_draw_xpm(num_6, 935, 100 + 50 * i + 50);
		break;
		case '7': video_draw_xpm(num_7, 935, 100 + 50 * i + 50);
		break;
		case '8': video_draw_xpm(num_8, 935, 100 + 50 * i + 50);
		break;
		case '9': video_draw_xpm(num_9, 935, 100 + 50 * i + 50);
		break;
		default: break;
		}
	}
}

void draw_border(short xi, short yi, short xf, short yf, unsigned long color){

	vg_draw_line(xi,yi,xi,yf,color);
	vg_draw_line(xf,yi,xf,yf,color);
	vg_draw_line(xi,yi,xf,yi,color);
	vg_draw_line(xi,yf,xf,yf,color);

	vg_draw_line(xi-1,yi,xi-1,yf,color);
	vg_draw_line(xf-1,yi,xf-1,yf,color);
	vg_draw_line(xi,yi-1,xf,yi-1,color);
	vg_draw_line(xi,yf-1,xf,yf-1,color);

	vg_draw_line(xi+1,yi,xi+1,yf,color);
	vg_draw_line(xf+1,yi,xf+1,yf,color);
	vg_draw_line(xi,yi+1,xf,yi+1,color);
	vg_draw_line(xi,yf+1,xf,yf+1,color);
}


void fill_black(){
	unsigned i = 0;
	for(;i < 768; i++){
		vg_draw_line(0,i,1023,i,0);
	}
}

void draw_main_menu()
{
	video_draw_xpm(Avoid_IT_A,182,30);
	video_draw_xpm(Avoid_IT_V,271,30);
	video_draw_xpm(Avoid_IT_O,375,30);
	video_draw_xpm(Avoid_IT_I,472,30);
	video_draw_xpm(Avoid_IT_D,525,30);

	video_draw_xpm(Avoid_IT_I,702,30);
	video_draw_xpm(Avoid_IT_T,755,30);

	video_draw_xpm(Menu_S, 329, 290);
	video_draw_xpm(Menu_t, 368, 290);
	video_draw_xpm(Menu_a, 395, 290);
	video_draw_xpm(Menu_r, 429, 290);
	video_draw_xpm(Menu_t, 468, 290);

	video_draw_xpm(Menu_G, 525, 290);
	video_draw_xpm(Menu_a, 574, 290);
	video_draw_xpm(Menu_m, 608, 290);
	video_draw_xpm(Menu_e, 664, 290);


	video_draw_xpm(Menu_H, 337, 430);
	video_draw_xpm(Menu_i, 391, 430);
	video_draw_xpm(Menu_g, 414, 430);
	video_draw_xpm(Menu_h, 450, 430);

	video_draw_xpm(Menu_S, 522, 429);
	video_draw_xpm(Menu_c, 561, 429);
	video_draw_xpm(Menu_o, 591, 429);
	video_draw_xpm(Menu_r, 624, 429);
	video_draw_xpm(Menu_e, 656, 429);


	video_draw_xpm(Menu_Q, 343, 570);
	video_draw_xpm(Menu_u, 391, 570);
	video_draw_xpm(Menu_i, 431, 570);
	video_draw_xpm(Menu_t, 454, 570);

	video_draw_xpm(Menu_G, 511, 570);
	video_draw_xpm(Menu_a, 560, 570);
	video_draw_xpm(Menu_m, 594, 570);
	video_draw_xpm(Menu_e, 650, 570);

}

void draw_high_score_menu()
{
	// HIGH SCORE

	video_draw_xpm(Menu_H, 337, 30);
	video_draw_xpm(Menu_i, 391, 30);
	video_draw_xpm(Menu_g, 414, 30);
	video_draw_xpm(Menu_h, 450, 30);

	video_draw_xpm(Menu_S, 522, 29);
	video_draw_xpm(Menu_c, 561, 29);
	video_draw_xpm(Menu_o, 591, 29);
	video_draw_xpm(Menu_r, 624, 29);
	video_draw_xpm(Menu_e, 656, 29);


	//video_draw_xpm(Back_to_Main_Menu, 309, 670);
	video_draw_xpm(Menu_B_petit, 299, 670);
	video_draw_xpm(Menu_a_petit, 332, 670);
	video_draw_xpm(Menu_c_petit, 358, 670);
	video_draw_xpm(Menu_k_petit, 380, 670);

	video_draw_xpm(Menu_t_petit, 418, 670);
	video_draw_xpm(Menu_o_petit, 438, 670);

	video_draw_xpm(Menu_M_petit, 472, 670);
	video_draw_xpm(Menu_a_petit, 516, 670);
	video_draw_xpm(Menu_i_petit, 542, 670);
	video_draw_xpm(Menu_n_petit, 559, 670);

	video_draw_xpm(Menu_M_petit, 599, 670);
	video_draw_xpm(Menu_e_petit, 643, 670);
	video_draw_xpm(Menu_n_petit, 666, 670);
	video_draw_xpm(Menu_u_petit, 696, 670);
}


void draw_game_over_menu(int great_score){

	// GAME OVER
	//video_draw_xpm(Game_Over_Menu, 106, 50);
	video_draw_xpm(Game_Over_G, 131, 50);
	video_draw_xpm(Game_Over_A, 224, 50);
	video_draw_xpm(Game_Over_M, 327, 50);
	video_draw_xpm(Game_Over_E, 443, 50);

	video_draw_xpm(Game_Over_O, 536, 50);
	video_draw_xpm(Game_Over_V, 637, 50);
	video_draw_xpm(Game_Over_E, 738, 50);
	video_draw_xpm(Game_Over_R, 811, 50);
	video_draw_xpm(Game_Over_excla, 897, 48);

	if (great_score){

		// Nice Score
		video_draw_xpm(letter_N, 387, 200);
		video_draw_xpm(letter_I, 412, 200);
		video_draw_xpm(letter_C, 437, 200);
		video_draw_xpm(letter_E, 462, 200);

		video_draw_xpm(letter_S, 512, 200);
		video_draw_xpm(letter_C, 537, 200);
		video_draw_xpm(letter_O, 562, 200);
		video_draw_xpm(letter_R, 587, 200);
		video_draw_xpm(letter_E, 612, 200);



		video_draw_xpm(letter_P, 200, 300);
		video_draw_xpm(letter_L, 225, 300);
		video_draw_xpm(letter_E, 250, 300);
		video_draw_xpm(letter_A, 275, 300);
		video_draw_xpm(letter_S, 300, 300);
		video_draw_xpm(letter_E, 325, 300);
		video_draw_xpm(virgula, 350, 300);

		video_draw_xpm(letter_C, 383, 300);
		video_draw_xpm(letter_L, 408, 300);
		video_draw_xpm(letter_I, 433, 300);
		video_draw_xpm(letter_C, 458, 300);
		video_draw_xpm(letter_K, 483, 300);

		video_draw_xpm(letter_T, 533, 300);
		video_draw_xpm(letter_O, 558, 300);

		video_draw_xpm(letter_W, 608, 300);
		video_draw_xpm(letter_R, 633, 300);
		video_draw_xpm(letter_I, 658, 300);
		video_draw_xpm(letter_T, 683, 300);
		video_draw_xpm(letter_E, 708, 300);

		video_draw_xpm(letter_Y, 758, 300);
		video_draw_xpm(letter_O, 783, 300);
		video_draw_xpm(letter_U, 808, 300);
		video_draw_xpm(letter_R, 833, 300);

		video_draw_xpm(letter_N, 200, 400);
		video_draw_xpm(letter_A, 225, 400);
		video_draw_xpm(letter_M, 250, 400);
		video_draw_xpm(letter_E, 275, 400);

		video_draw_xpm(letter_A, 325, 400);
		video_draw_xpm(letter_N, 350, 400);
		video_draw_xpm(letter_D, 375, 400);

		video_draw_xpm(letter_T, 425, 400);
		video_draw_xpm(letter_H, 450, 400);
		video_draw_xpm(letter_E, 475, 400);
		video_draw_xpm(letter_N, 500, 400);

		video_draw_xpm(letter_P, 550, 400);
		video_draw_xpm(letter_R, 575, 400);
		video_draw_xpm(letter_E, 600, 400);
		video_draw_xpm(letter_S, 625, 400);
		video_draw_xpm(letter_S, 650, 400);

		video_draw_xpm(letter_E, 700, 400);
		video_draw_xpm(letter_N, 725, 400);
		video_draw_xpm(letter_T, 750, 400);
		video_draw_xpm(letter_E, 775, 400);
		video_draw_xpm(letter_R, 800, 400);

		// Insert your name:
		video_draw_xpm(Your_Name, 100, 500);
		draw_border(310, 500, 820,527,54);
	}

	else
	{
		video_draw_xpm(GameOver_Character, 200,320);
		video_draw_xpm(GameOver_Obstacle, 700,300);
	}

	// Back to Main Menu
	video_draw_xpm(Menu_B_petit, 60, 673);
	video_draw_xpm(Menu_a_petit, 93, 673);
	video_draw_xpm(Menu_c_petit, 119, 673);
	video_draw_xpm(Menu_k_petit, 141, 673);

	video_draw_xpm(Menu_t_petit, 179, 673);
	video_draw_xpm(Menu_o_petit, 199, 673);
	//space 10
	video_draw_xpm(Menu_M_petit, 233, 673);
	video_draw_xpm(Menu_a_petit, 277, 673);
	video_draw_xpm(Menu_i_petit, 303, 673);
	video_draw_xpm(Menu_n_petit, 320, 673);

	video_draw_xpm(Menu_M_petit, 360, 673);
	video_draw_xpm(Menu_e_petit, 404, 673);
	video_draw_xpm(Menu_n_petit, 427, 673);
	video_draw_xpm(Menu_u_petit, 457, 673);


	// Quit Game
	video_draw_xpm(Quit_Game_Q, 700, 670);
	video_draw_xpm(Quit_Game_u, 734, 670);
	video_draw_xpm(Quit_Game_i, 768, 670);
	video_draw_xpm(Quit_Game_t, 788, 670);

	video_draw_xpm(Quit_Game_G, 830, 670);
	video_draw_xpm(Quit_Game_a, 869, 670);
	video_draw_xpm(Quit_Game_m, 897, 670);
	video_draw_xpm(Quit_Game_e, 942, 670);
}


int get_final_score()
{
	int temp = global_final_score;
	global_final_score = 0, global_times_hit = 0, global_levels = 0;
	return temp;
}

int draw_if_not_player(unsigned short x, unsigned short y, unsigned short color)
{
	if (color > 63)
		return 0;

	char *pixel = second_buff + (((y * h_res) + x) * bits_per_pixel / 8);

	if(*pixel == 40 || *pixel == 8 || *pixel == 56 || *pixel == 63 || *pixel == 34 || *pixel == 16 || *pixel ==	18)
		return 0;

	*pixel = color;

	return 0;
}
