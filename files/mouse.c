#include "bootpack.h"

struct FIFO8 mousefifo;

void inthandler2c(int *esp)
{
    unsigned char data;
    io_out8(PIC1_OCW2, 0x64);       // 通知PIC IRQ-12已经受理完毕
    io_out8(PIC0_OCW2, 0x62);       // 通知PIC IRQ-02已经受理完毕
    data = io_in8(PORT_KEYDAT);
    fifo8_put(&mousefifo, data);
    return;
}

void enable_mouse(void)
{
    // 激活鼠标
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return;
}
