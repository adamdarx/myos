#include "bootpack.h"

void set_palette(int start, int end, unsigned char * rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for(i = start; i <= end; i++)
    {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0x00, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84
    };
    set_palette(0, 15, table_rgb);
    return;
}

void boxfill8(unsigned char * vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int i, j;
    for(i = x0; i <= x1; i++)
        for(j = y0; j <= y1; j++)
            vram[i + xsize * j] = c;
    return;
}

void init_screen8(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}


void putfont8(char *vram, int xsize, int x, int y, char c, char * font)
{
    int i;
    char d;
    for(i = 0; i < 16; i++) {
        d = font[i];
        if((d & 0x80) != 0) vram[(y + i) * xsize + x + 0] = c;      // 0x80 = 0b10000000
        if((d & 0x40) != 0) vram[(y + i) * xsize + x + 1] = c;      // 0x40 = 0b01000000
        if((d & 0x20) != 0) vram[(y + i) * xsize + x + 2] = c;      // 0x20 = 0b00100000
        if((d & 0x10) != 0) vram[(y + i) * xsize + x + 3] = c;      // 0x10 = 0b00010000
        if((d & 0x08) != 0) vram[(y + i) * xsize + x + 4] = c;      // 0x08 = 0b00001000
        if((d & 0x04) != 0) vram[(y + i) * xsize + x + 5] = c;      // 0x04 = 0b00000100
        if((d & 0x02) != 0) vram[(y + i) * xsize + x + 6] = c;      // 0x02 = 0b00000010
        if((d & 0x01) != 0) vram[(y + i) * xsize + x + 7] = c;      // 0x01 = 0b00000001
    }
}

void putfont8_str(char *vram, int xsize, int x, int y, char c, unsigned char * str)
{
    for(;*str != 0x00; str++) {
        putfont8(vram, xsize, x, y, c, font + *str * 16);
        x += 8;
        if(x >= xsize) {    // 换行
            x %= 320;
            y += 16;
        }
    }
}

void init_mouse_cursor8(char *mouse, char bc)
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}


void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}
