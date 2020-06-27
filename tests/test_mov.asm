    global start
    section .text

# Test moving data from a register to memory and back
# register, value, address
%macro  reg2mem2reg 3
    mov %1, %2
    mov [%3], %1
    xor %1, %1
    mov %1, [%3]
%endmacro

# Test moving data from register to another
# register register value
%macro reg2reg 3
    mov %1, %3
    mov %2, %1
    xor %1, %1
    mov %1, %2
%endmacro

start:
# Our first opcode, yes!
    nop

    reg2mem2reg ax, 0x1234, 0x9000
    reg2mem2reg bx, 0x2345, 0xA000
    reg2mem2reg cx, 0x3456, 0xB000
    reg2mem2reg dx, 0x4567, 0xC000

    reg2mem2reg al, 0x10, 0x9100
    reg2mem2reg bl, 0x20, 0xA100
    reg2mem2reg cl, 0x30, 0xB100
    reg2mem2reg dl, 0x40, 0xC100

    reg2mem2reg ah, 0x10, 0x9100
    reg2mem2reg bh, 0x20, 0xA100
    reg2mem2reg ch, 0x30, 0xB100
    reg2mem2reg dh, 0x40, 0xC100

    reg2reg ax, bx, 0x12
    reg2reg ax, cx, 0x34
    reg2reg ax, dx, 0x56

    reg2reg bx, cx, 0x78
    reg2reg bx, dx, 0x9A

    reg2reg cx, dx, 0xBC

    mov ax, ax
    mov bx, bx
    mov cx, cx
    mov dx, dx
