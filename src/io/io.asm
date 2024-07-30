SECTION .asm

global in_sb
global in_sw
global out_b
global out_w

in_sb:
    push    ebp
    mov     ebp, esp

    xor     eax, eax
    mov     edx, dword [ebp + 8]
    in      al, dx

    pop     ebp
    ret

in_sw:
    push    ebp
    mov     ebp, esp

    xor     eax, eax
    mov     edx, dword [ebp + 8]
    in      ax, dx

    pop     ebp
    ret

out_b:
    push    ebp
    mov     ebp, esp

    mov     eax, dword [ebp + 12]
    mov     edx, dword [ebp + 8]
    out     dx, al

    pop     ebp
    ret

out_w:
    push    ebp
    mov     ebp, esp

    mov     eax, dword [ebp + 12]
    mov     edx, dword [ebp + 8]
    out     dx, ax

    pop     ebp
    ret
