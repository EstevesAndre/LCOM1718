#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */


#define SET_VBE_MODE				0x4F02 /**< @brief command to VBE mode */
#define BIOS_VIDEO_SERVICES			0x10 /**< @brief command to BIOS Video Services */
#define VIDEO_MODE_FUNCTION			0x00 /**< @brief command to Video mode function */
#define TEXT_MODE_80x25				0x03 /**< @brief command to Text mode */


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
 * @brief draws a square with a given color
 *
 * Draws a filled square with the specified size and color
 * The square center with respect to the center of the screen in video mode 0x105
 *
 * @param x horizontal coordinate of square center, with regard to the center of the screen
 * @param y vertical coordinate of square center, with regard to the center of the screen
 * @param size of each side in pixels
 * @param color color to set the pixel
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_draw_square (unsigned short x, unsigned short y, unsigned short size, unsigned long color);


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

//FALTAAAAA ACABAR

/**
 * @brief draws the xpm on the screen
 *
 * @param *xpm[] xpm to be drawn
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 *
 * @return 0 upon success, non-zero upon failure
 */
int video_draw_xpm(char *xpm[], unsigned short xi, unsigned short yi);

char *read_xpm(char *map[], int *wd, int *ht);


/**
 * @brief clears the screen
 *
 */
void video_draw_black();

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
