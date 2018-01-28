#ifndef __TEST5_H
#define __TEST5_H

/** @defgroup lab5 lab5
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */


#define MODE_0x105		0x105 /**< @brief command to 0x105 graphics mode */

/**
 * @brief Tests initialization of video card in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode. After delay seconds, it should go 
 *  back to Minix's default text mode.
 * 
 * @param mode 16-bit VBE mode to set
 * @param delay delay in seconds after which returns to text mode
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *video_test_init(unsigned short mode, unsigned short delay);

/**
 * @brief Tests drawing a square with a given color
 * 
 * Tests drawing a (filled) square with the specified size and color, at the specified
 *  coordinates (which specify the square center with respect to the center 
 *  of the screen) in video mode 0x105
 * 
 * Calls video_dump_fb() immediately after writing the sprite to the frame-buffer.
 * 
 * @param x horizontal coordinate of square center, wrt the center of the screen
 * @param y vertical coordinate of square center, wrt the center of the screen
 * @param size of each side in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);

/**
 * @brief Tests drawing a line segment with specifed end points and color
 * 
 * Tests drawing a line segment with the specified end points and the input color, 
 *  by writing to VRAM in video mode 0x105
 * 
 * Calls video_dump_fb() immediately after writing the line to the frame-buffer.
 * 
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)  
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 * @param xf horizontal coordinate of "last" endpoint, starts at 0 (leftmost pixel)  
 * @param yf vertical coordinate of "last" endpoint, starts at 0 (top pixel)  
 * @param color color of the line segment to draw
 * @return 0 on success, non-zero otherwise
 */
int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color);
		           
/**
 * @brief Tests drawing a XPM on the screen at specified coordinates
 * 
 * Tests drawing a sprite from an XPM on the screen at specified coordinates
 *  by writing to VRAM in video mode 0x105
 * Calls video_dump_fb() immediately after writing the sprite to the frame-buffer.
 *
 * @param xpm array with XPM (assuming indexed color mode)   
 * @param xi horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)  
 * @param yi vertical coordinate of upper-left corner, starts at 0 (top pixel)  
 * @return 0 upon success, non-zero upon failure
 */
int video_test_xpm(char *xpm[], unsigned short xi, unsigned short yi);

/**
 * @brief Tests moving sprite on the screen along horizontal/vertical axes
 * 
 * Tests moving a XPM on the screen along horizontal/vertical axes, at the specified speed and frame-rate, in video mode 0x105
 * 
 * If speed is positive, then it specifies the displacement in pixels between consecutive frames
 * If speed is negative, then it specifies the number of frames required for a 1 pixel displacement
 *  
 * Calls video_dump_fb() every time it copies the XPM to the frame buffer.
 * 
 * @param xpm array with XPM (assuming indexed color mode) 
 * @param xi initial horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)  
 * @param yi initial vertical coordinate of upper-left corner, starts at 0 (top pixel)
 * @param xf final horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)  
 * @param yf final vertical coordinate of upper-left corner, starts at 0 (top pixel) * 
 * @param s  speed of movement.
 * @param f  frame-rate in frames per second
 * @return 0 upon success, non-zero upon failure
 */
int video_test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f);


/**
 * @brief Tests retrieving VBE controller information  (VBE function 0x0)
 * 
 * Tests retrieving VBE controller information (VBE function 0x0) and its parsing.
 * Upon its invocation it should display the following pieces information, one per line:
 * 	- VbeVersion
 * 	- List of mode numbers (in hexadecimal) supported, using ':' as a separator
 * 	- Total memory in KB
 * 
 * @return 0 upon success, non-zero upon failure
 */
int test_controller();
		           
		           

/** @} end of lab5 */
 
 
#endif /* __TEST5_H */
