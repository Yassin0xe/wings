# WiNgS Tool
## description
 this tools is for testing the shellcodes and run it to the memory
 ,is created by yassin0xe
## install
to install the wings tool ,basicaly use MakeFile
to install it
```sh
sudo make install
```
## usage
this tool take the shellcode binary file path as argument
to run it it in the memory

```sh
wings -h
```
the output similarly to this
```
Usage wings [-f FILE][-r][-x/-t]
       _______              _                   _______
       \------\   _      __(_)___  ____ ______ /------/
        \-----|  | | /| / / / __ \/ __ `/ ___/ |-----/
         \----|  | |/ |/ / / / / / /_/ (__  )  |----/
          \---|  |__/|__/_/_/ /_/\__, /____/   |---/
           \__/                 /____/         \__/
            |   _____________________________    |
            |__|                             |___|
               |    Created By Yassin0xe     |
               |_____________________________|

 Options

	--file      -f FILE     The directory to thr shellcode file
	--run       -r          Run the Shell Code
	--hexdump   -x          Show The shellCode As Hexdump
	--textdump  -t          Show the shellcode as Textump
	--help      -h          Show The HElp Menu

 Exemples

	wings -f shellcode.bin -r
	wings -f shellcode.bin -r -x
	wings -f shellcode.bin -r -t
	wings -f shellcode.bin -x
	wings -f shellcode.bin -t
```
## example
shellcode.asm file
```asm
BITS 64
mov rax,0x0a414141414141
push rax
push rsp
pop rsi
mov rdx,7
mov rdi,1
mov rax,1
syscall
mov rdi,0
mov rax,60
syscall
```
```sh
nasm test.asm -o test.bin
wings -f test.bin -r
```
the outputs is like this
```
[*] opening (shellcode.bin)
[+] seccessful read (42 bytes)
[+] the shellcode is at 0x7ffd804711c0
[*] start runing the shellcode
AAAAAA
```
