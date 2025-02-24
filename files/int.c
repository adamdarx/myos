#include "bootpack.h"

void init_pic(void)
{
    // 禁止主副PIC中断
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);
    
    io_out8(PIC0_ICW1, 0x11);       // 边沿触发模式
    io_out8(PIC0_ICW2, 0x20);       // IRQ0~7对应到INT20~27
    io_out8(PIC0_ICW3, 1 << 2);     // PIC1由IRQ2连接，0b001 --> 0b100
    io_out8(PIC0_ICW4, 0x01);       // 无缓冲区模式

    io_out8(PIC1_ICW1, 0x11);       // 边沿触发模式
    io_out8(PIC1_ICW2, 0x28);       // IRQ8~15对应到INT28~2f
    io_out8(PIC1_ICW3, 2);          // PIC1由IRQ2连接
    io_out8(PIC1_ICW4, 0x01);

    // 除PIC1以外(即IRQ2)的中断禁止
    io_out8(PIC0_IMR, 0xfb);       // 0x11111011
    io_out8(PIC1_IMR, 0xff);        // 把PIC1的中断禁止
}
