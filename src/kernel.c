#include "kernel.h"
#include "idt/idt.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include <stddef.h>
#include <stdint.h>

static uint16_t * video_mem    = 0;
static uint16_t   terminal_row = 0;
static uint16_t   terminal_col = 0;

uint16_t terminal_make_char(char c, char color) 
{
    return (color << 8) | c;
}

void terminal_put_char(int x, int y, char c, char color) 
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

void terminal_write_char(char c, char color) 
{
    if (c == '\n') 
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_put_char(terminal_col, terminal_row, c, color);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH) 
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_init() 
{
    video_mem = (uint16_t *) (0xb8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int row = 0; row < VGA_HEIGHT; row++) 
    {
        for (int col = 0; col < VGA_WIDTH; col++) 
        {
            terminal_put_char(col, row, ' ', 0);
        }
    }
}

void print_str(const char *str) 
{
    size_t len = str_len(str);
    for (int i = 0; i < len; i++) 
    {
        terminal_write_char(str[i], 15);
    }
}

static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main() 
{
    // initialize the terminal.
    terminal_init();
    print_str("hello world!! \n test");
    
    // initialize the heap.
    kheap_init();

    // search and initialize the disks
    disk_search_and_init();

    // initialize the interrupt descriptor table (IDT).
    init_idt();

    // setup paging
    kernel_chunk = 
        paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    // switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    char* ptr = kzalloc(4096);
    paging_set(
        paging_4gb_chunk_get_directory(kernel_chunk), 
        (void*) 0x1000,
        (uint32_t) ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITABLE
    );

    // enable paging
    enable_paging();

    // enable the system interrupts
    enable_interrupt();

    struct path_root * root_path = pathparser_parse("0:/bin/shell.exe", NULL);
    if ( root_path )
    {
        // TODO
    }
}
