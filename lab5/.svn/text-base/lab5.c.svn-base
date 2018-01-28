#include "test5.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <stdlib.h>
#include "video_test.h"

char *pic1[] = {
		"32 13 4",
		". 0",
		"x 2",
		"o 14",
		"+ 4",
		"................................",
		"..............xxx...............",
		"............xxxxxxx.............",
		".........xxxxxx+xxxxxx..........",
		"......xxxxxxx+++++xxxxxxx.......",
		"....xxxxxxx+++++++++xxxxxxx.....",
		"....xxxxxxx+++++++++xxxxxxx.....",
		"......xxxxxxx+++++xxxxxxx.......",
		".........xxxxxx+xxxxxx..........",
		"..........ooxxxxxxxoo...........",
		".......ooo...........ooo........",
		".....ooo...............ooo......",
		"...ooo...................ooo...."
};

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);
static unsigned long parse_long(char *str, int base);


int main(int argc, char **argv)
{

	/* DO NOT FORGET TO initialize service (DONE)*/
	sef_startup();

	if (argc == 1) {						/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init <hexadecimal no.- mode> <decimal no. - delay>\"\n"
			"\t service run %s -args \"square <decimal no.- x> <decimal no. - y> <decimal no. - size> <decimal no. - color>\"\n"
			"\t service run %s -args \"line <decimal no.- xi> <decimal no. - yi> <decimal no. - xf> <decimal no. - yf> <decimal no. - color>\"\n"
			"\t service run %s -args \"xpm <decimal no. - xi> <decimal no. - yi>\"\n"
			"\t service run %s -args \"move <decimal no.- xi> <decimal no. - yi> <decimal no. - xf> <decimal no. - yf> <decimal no. - speed> <decimal no. - frame_rate>\"\n"
			"\t service run %s -args \"controller\"\n",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long mode, delay, x, y, size, color, xi, yi, xf, yf, f;
	long s;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("video: wrong no. of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);						/* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;
		delay = parse_ulong(argv[3],10);
		if (delay == ULONG_MAX)
			return 1;
		printf("video::video_test_init(%lu,%lu)\n", mode, delay);
		return video_test_init(mode,delay);
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("video: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
		x = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (x == ULONG_MAX)
			return 1;
		y = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (y == ULONG_MAX)
			return 1;
		size = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (size == ULONG_MAX)
			return 1;
		color = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		if (color == ULONG_MAX)
			return 1;
		printf("video::video_test_square(%lu, %lu, %lu, %lu)\n",x,y,size,color);
		return video_test_square(x,y,size,color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("video: wrong no of arguments for video_test_line()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		xf = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (xf == ULONG_MAX)
			return 1;
		yf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		if (yf == ULONG_MAX)
			return 1;
		color = parse_ulong(argv[6], 10);						/* Parses string to unsigned long */
		if (color == ULONG_MAX)
			return 1;
		printf("video::video_test_line(%lu,%lu,%lu,%lu,%lu)\n", xi,yi,xf,yf,color);
		return video_test_line(xi,yi,xf,yf,color);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 4) {
			printf("video: wrong no of arguments for video_test_xpm()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		printf("video::video_test_xpm(%lu,%lu)\n", xi,yi);
		return video_test_xpm(pic1,xi,yi);
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0){
		if (argc != 8){
			printf ("video: wrong no of arguments for video_test_move()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;
		yi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;
		xf = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (xf == ULONG_MAX)
			return 1;
		yf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		if (yf == ULONG_MAX)
			return 1;
		s = parse_long(argv[6], 10);						/* Parses string to long */
		if (s == LONG_MAX)
			return 1;
		f = parse_ulong(argv[7], 10);						/* Parses string to unsigned long */
		if (f == ULONG_MAX)
			return 1;
		printf ("video::video_test_move (%lu,%lu,%lu,%lu,%lu,%lu)\n",xi,yi,xf,yf,s,f);
		return video_test_move(pic1,xi,yi,xf,yf,s,f);
	}
	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("video: wrong no of arguments for test_config()\n");
			return 1;
		}
		printf("video::test_controller\n");
		return test_controller();
	}
	else {
		printf("video: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("video: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}


static unsigned long parse_long(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtol(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (errno == ERANGE && val == LONG_MIN)
	{
		perror("strtol");
		return LONG_MIN;
	}

	if (endptr == str) {
		printf("mouse: parse_long: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
