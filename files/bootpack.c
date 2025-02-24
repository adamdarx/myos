#include "bootpack.h"
#include <stdio.h>

extern struct FIFO8 keyfifo, mousefifo;

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my;

	init_gdtidt();
	init_pic();
	io_sti();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfont8_str(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	init_keyboard();
	enable_mouse();

	for (;;) {
		io_hlt();
		if(fifo8_status(&keyfifo) + fifo8_status(&mousefifo)  == 0) {
			io_stihlt();
		} else {
			if(fifo8_status(&keyfifo)) {
				int i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
				putfont8_str(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo)) {
				int i = fifo8_get(&mousefifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 47, 31);
				putfont8_str(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
			}
		}
	}
}
