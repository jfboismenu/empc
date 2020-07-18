; MIT License
;
; Copyright (c) 2020 Jean-François Boismenu
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.

; BIOS is loaded at F0000 and processors is in 16 bits mode on startup
org 0xf0000
bits 16

; Test moving data from a register to memory and back
; register, value, address
%macro  reg2mem2reg 3
    mov %1, %2
    mov [%3], %1
    mov %1, 0
    mov %1, [%3]
    mov %1, 0
%endmacro

; Test moving data from register to another
; register register value
%macro reg2reg 3
    mov %1, %3
    mov %2, %1
    mov %1, 0
    mov %1, %2
    mov %1, 0
    mov %2, 0
%endmacro

; This is the test entry point. It will be written from 0xf0000
test:
    reg2mem2reg ax, 0x1234, 0x9000
    reg2mem2reg bx, 0x2345, 0xA000
    reg2mem2reg cx, 0x3456, 0xB000
    reg2mem2reg dx, 0x4567, 0xC000
    reg2mem2reg sp, 0x4567, 0xD000
    reg2mem2reg bp, 0x5678, 0xE000
    reg2mem2reg si, 0x6789, 0xF000
    reg2mem2reg di, 0x7890, 0x1000

    reg2mem2reg al, 0x10, 0x91
    reg2mem2reg bl, 0x20, 0xA1
    reg2mem2reg cl, 0x30, 0xB1
    reg2mem2reg dl, 0x40, 0xC1

    reg2mem2reg ah, 0x10, 0x92
    reg2mem2reg bh, 0x20, 0xA2
    reg2mem2reg ch, 0x30, 0xB2
    reg2mem2reg dh, 0x40, 0xC2

    reg2reg ax, bx, 0x1234
    reg2reg ax, cx, 0x3456
    reg2reg ax, dx, 0x5678
    reg2reg ax, sp, 0x6789
    reg2reg ax, bp, 0x789A
    reg2reg ax, si, 0x89AB
    reg2reg ax, di, 0x9ABC

    reg2reg bx, cx, 0xABCD
    reg2reg bx, dx, 0xBCDE
    reg2reg bx, sp, 0xCDEF
    reg2reg bx, bp, 0xDEF0
    reg2reg bx, si, 0xEF01
    reg2reg bx, di, 0xF012

    reg2reg cx, dx, 0x0123
    reg2reg cx, sp, 0x1234
    reg2reg cx, bp, 0x2345
    reg2reg cx, si, 0x3456
    reg2reg cx, di, 0x4567

    reg2reg dx, sp, 0x5678
    reg2reg dx, bp, 0x6789
    reg2reg dx, si, 0x789A
    reg2reg dx, di, 0x89AB

    reg2reg sp, bp, 0x9ABC
    reg2reg sp, si, 0xABCD
    reg2reg sp, di, 0xBCDE

    reg2reg bp, si, 0xCDEF
    reg2reg bp, di, 0xDEF0

    reg2reg si, di, 0xEF01

    mov ax, 0xF012
    mov ax, ax

    mov bx, 0x0123
    mov bx, bx

    mov cx, 0x1234
    mov cx, cx

    mov dx, 0x2345
    mov dx, dx

    mov sp, 0x3456
    mov sp, sp

    mov bp, 0x4567
    mov bp, bp

    mov si, 0x5678
    mov si, si

    mov di, 0x6789
    mov di, di

    hlt

; PC jumps to FFFF0 on startup, so let's jump to the beginning of the test.
times   0xfff0-($-$$) DB 0x90
    jmp 0xf000:0000
times   0x10000-($-$$) DB 0x90
