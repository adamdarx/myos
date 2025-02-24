TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
FONT	 = $(TOOLPATH)makefont.exe		# 制作字体（txt->bin）
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) img

# 镜像文件生成

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

# 先编译成bin文件
font.bin : font.txt Makefile
	$(FONT) font.txt font.bin

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

# 再编译成obj文件
font.obj: font.bin Makefile
	$(BIN2OBJ) font.bin font.obj _font

%.obj : %.nas Makefile
	$(NASK) $*.nas  $*.obj $*.lst

bootpack.bim : bootpack.obj naskfunc.obj graphic.obj dsctbl.obj int.obj fifo.obj keyboard.obj mouse.obj font.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj graphic.obj dsctbl.obj int.obj fifo.obj keyboard.obj mouse.obj font.obj
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

os.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb os.sys

os.img : ipl.bin os.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:os.sys to:@: \
		imgout:os.img

# 其他指令

img :
	$(MAKE) os.img

run :
	$(MAKE) img
	$(COPY) os.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: os.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) *.sys

src_only :
	$(MAKE) clean
	-$(DEL) os.img
