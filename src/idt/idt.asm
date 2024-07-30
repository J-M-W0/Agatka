SECTION .asm

extern int21h_handler
extern no_interrupt_handler

global load_idt
load_idt:
    push ebp
    mov ebp, esp

    mov ebx, dword [ebp + 8]
    lidt [ebx]

    pop ebp
    ret

global int21h
int21h:
	cli
	pushad
	call int21h_handler
	popad
	sti
	iret

global no_interrupt
no_interrupt:
	cli
	pushad
	call no_interrupt_handler
	popad
	sti
	iret

global enable_interrupt
enable_interrupt:
	sti
	ret

global disable_interrupt
disable_interrupt:
	cli
	ret
