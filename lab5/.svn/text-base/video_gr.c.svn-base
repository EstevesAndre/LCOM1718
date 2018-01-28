#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>
#include "video_test.h"
#include "video_gr.h"
#include "vbe.h"
#include "sprite.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM. */
#define VRAM_PHYS_ADDR	0xE0000000

#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	8

/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void* vg_init(unsigned short mode)
{
	vbe_mode_info_t info;

	vbe_get_mode_info(mode, &info);

	h_res = info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;

	struct reg86u reg86;

	reg86.u.w.ax = SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	reg86.u.b.intno = BIOS_VIDEO_SERVICES;

	if( sys_int86(&reg86) != OK )
	{
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	int r;
	struct mem_range mr;
	unsigned int vram_base = info.PhysBasePtr; /* VRAM’s physical addresss*/
	unsigned int vram_size = h_res * v_res * bits_per_pixel / BITS_PER_PIXEL; /* VRAM’s size, but you can use the frame-buffer size, instead */


	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;


	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
		panic("couldn’t map video memory");

	return video_mem;
}

int vg_exit()
{
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VIDEO_SERVICES; /* BIOS video services */

	reg86.u.b.ah = VIDEO_MODE_FUNCTION;    /* Set Video Mode function */
	reg86.u.b.al = TEXT_MODE_80x25;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

int vg_pixel_color(unsigned short x, unsigned short y, unsigned short color)
{
	char *pixel = video_mem + (((y * H_RES) + x) * bits_per_pixel / BITS_PER_PIXEL);

	*pixel = color;

	return 0;
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


int vg_draw_square (unsigned short x, unsigned short y, unsigned short size, unsigned long color){

	unsigned short x_centro_tela = h_res / 2;
	unsigned short y_centro_tela = v_res / 2;

	unsigned short x_centro_quadrado = x_centro_tela + x;
	unsigned short y_centro_quadrado = y_centro_tela + y;

	unsigned short x_primeiro_pixel;
	unsigned short y_primeiro_pixel;

	if ((size / 2) >= x_centro_quadrado)
		x_primeiro_pixel = 0;

	else x_primeiro_pixel = x_centro_quadrado - (size / 2);

	if ((size / 2) >= y_centro_quadrado)
		y_primeiro_pixel = 0;

	else y_primeiro_pixel = y_centro_quadrado - (size / 2);

	unsigned int i = 0;

	for (;i < size; i++)
	{
		vg_draw_line(x_primeiro_pixel, y_primeiro_pixel + i, x_primeiro_pixel + (size - 1), y_primeiro_pixel + i, color);
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

void video_draw_black()
{
	int i = 0, j = 0;

	for(;j < V_RES; j++)
	{
		for(;i < H_RES; i++)
		{
			vg_pixel_color(i, j, 0);
		}
		i = 0;
	}
}
