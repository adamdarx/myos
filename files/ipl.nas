; hello-os
; TAB=4
CYLS	EQU		10				; 读取柱面的数量
		ORG		0x7c00			; 指明程序装载地址

; 标准FAT12格式软盘专用的代码 Stand FAT12 format floppy code

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; 启动扇区名称（8字节）
		DW		512				; 每个扇区（sector）大小（必须512字节）
		DB		1				; 簇（cluster）大小（必须为1个扇区）
		DW		1				; FAT起始位置（一般为第一个扇区）
		DB		2				; FAT个数（必须为2）
		DW		224				; 根目录大小（一般为224项）
		DW		2880			; 该磁盘大小（必须为2880扇区1440*1024/512）
		DB		0xf0			; 磁盘类型（必须为0xf0）
		DW		9				; FAT的长度（必??9扇区）
		DW		18				; 一个磁道（track）有几个扇区（必须为18）
		DW		2				; 磁头数（必??2）
		DD		0				; 不使用分区，必须是0
		DD		2880			; 重写一次磁盘大小
		DB		0,0,0x29		; 意义不明（固定）
		DD		0xffffffff		; （可能是）卷标号码
		DB		"HELLO-OS   "	; 磁盘的名称（必须为11字?，不足填空格）
		DB		"FAT12   "		; 磁盘格式名称（必??8字?，不足填空格）
		RESB	18				; 先空出18字节

; 程序主体

entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		AX, 0x0820
		MOV		ES, AX
		MOV		CH, 0  ; 0号柱面
		MOV		DH, 0  ; 0号磁头（正面）
		MOV		CL, 2  ; 2号扇区
readloop:
		MOV		SI, 0  ; 记录失败次数的寄存器（本来应该用CX，但是CL和CH已经被占用了）
retry:
		MOV		AH, 0x02
		MOV		AL, 1
		MOV		BX, 0
		MOV 	DL, 0x00
		INT  	0x13
		JNC  	next
		ADD  	SI, 1  ; jnc没有执行，则说明有错误发生了！
		CMP  	SI, 5
		JAE  	error  ;  如果si中的错误次数大于等于5，则跳转到error代码段
		MOV  	AH, 0x00  ; 重置
		MOV  	DL, 0x00  ; 重置
		INT  	0x13
		JMP 	retry  ; 循环
next:
		MOV		AX,ES			; 把内存地址后移0x200（512/16十六进制转换）
		ADD		AX,0x0020
		MOV		ES,AX			; ADD ES,0x020因为没有ADD ES，只能通过AX进行
		ADD		CL, 1
		CMP		CL, 18
		JBE		readloop
        MOV     CL, 1
        ADD     DH, 1
        CMP     DH, 2
        JB      readloop
	    MOV     DH, 0
        ADD     CH, 1
        CMP     CH, CYLS
        JBE     readloop
        JMP     ok
fin:
		HLT						; 让CPU停止，等待指令
		MOV		[0x0ff0], CH	; 储存读取柱面数
		JNC		0xc200			; 若无异常则跳转到系统起始位置
		JMP		fin				; 无限循环

ok:
	MOV SI, ok_msg	
	JMP putloop

error:
	MOV SI, err_msg
	JMP putloop

putloop:
		MOV AL, [SI]
		ADD SI, 1
		CMP AL, 0
		JE fin
		MOV AH, 0x0e
		MOV BX, 15
		INT 0x10
		JMP putloop

ok_msg:
		DB "Reading 10 cylinders finished! Congratulations!", 0x0d, 0x0a, 0

err_msg:
		DB "An error happened! Please check your code.", 0x0d, 0x0a, 0