#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "video_test.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p)
{
	lm_init();

	struct reg86u r;

	mmap_t buf;

	unsigned long size = sizeof(vbe_mode_info_t);

	lm_alloc(size,&buf);					/* use liblm.a to initialize buf */

	r.u.w.ax = VBE_MODE_INFO;          /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf.phys);    /* set a segment base */
	r.u.w.di = PB2OFF(buf.phys);     /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK )
	{ /* call BIOS */
		printf("vbe_get_mode_info: sys_int86() failed \n");
		lm_free(&buf);
		return 1;
	}

	*vmi_p = * (vbe_mode_info_t*) buf.virtual;
	lm_free(&buf);

	return 0;
}

int vbe_get_info(vbe_config_info *config)
{
	lm_init();

	struct reg86u r;

	mmap_t buf;

	unsigned long size = sizeof(vbe_config_info);

	lm_alloc(size,&buf);					/* use liblm.a to initialize buf */

	r.u.w.ax = VBE_INFO;          /* VBE get info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf.phys);    /* set a segment base */
	r.u.w.di = PB2OFF(buf.phys);     /* set the offset accordingly */
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK )
	{ /* call BIOS */
		printf("vbe_get_mode_info: sys_int86() failed \n");
		lm_free(&buf);
		return 1;
	}

	*config = * (vbe_config_info*) buf.virtual;
	lm_free(&buf);

	return 0;
}


