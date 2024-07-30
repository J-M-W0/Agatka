#ifndef __KERNEL_H
#define __KERNEL_H

#define VGA_WIDTH  80
#define VGA_HEIGHT 20

#define MAX_PATH   108

void kernel_main();
void print_str(const char *str);

#endif // __KERNEL_H
