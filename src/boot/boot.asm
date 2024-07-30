ORG 0x7c00

[BITS 16]

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop
    times 33 db 0

start:
    jmp 0:step2

step2:
    cli;        clear interrupt
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti;        set interrupt

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

;; ==============================
;; GDT
;; ==============================
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0
;; offset 0x8
;; CS should be pointing to here.
gdt_code:
    dw 0xffff;      segment limit first 0-15 bits
    dw 0;           base first 0-15 bits
    db 0;           base 16-23 bits
    db 0x9a;        access byte
    db 11001111b;   high 4 bit flags and low 4 bit flags
    db 0;           base 24-31 bits
;; offset 0x10
;; DS, SS, ES, FS, GS
gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x00100000
    call ata_lba_read
    jmp CODE_SEG:0x00100000

ata_lba_read:
    mov dx, 0x01f6
    mov ebx, eax;   backup the LBA
    shr eax, 24
    or eax, 0xe0;   select the master drive.
    out dx, al
    ;; ^ finished sending the highest 8 bits of the LBA

    mov dx, 0x01f2
    mov eax, ecx
    out dx, al
    ;; ^ finished sending the total sectors to read

    mov dx, 0x01f3
    mov eax, ebx
    out dx, al
    ;; ^ finished sending more bits of the LBA

    mov dx, 0x01f4
    mov eax, ebx;   restore the backup LBA
    shr eax, 8
    out dx, al
    ;; ^ finished sending more bits of the LBA
    
    mov dx, 0x01f5
    mov eax, ebx;   restore the backup LBA
    shr eax, 16
    out dx, al
    ;; ^ finished sending more bits of the LBA

    mov dx, 0x01f7
    mov al, 0x20
    out dx, al
    
;; read all sectors into memory
.next_sector:
    push ecx
;; check if we need to read
.try_again:
    mov dx, 0x01f7
    in al, dx
    test al, 8
    jz .try_again

    ;; we need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x01f0
    rep insw
    pop ecx
    loop .next_sector
    ;; ^ end of reading sectors into memory
    ret
    


times 510-($-$$) db 0
dw 0xaa55
