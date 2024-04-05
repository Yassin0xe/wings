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
