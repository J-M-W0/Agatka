#ifndef __IO_H
#define __IO_H

extern unsigned char  in_sb(unsigned short port);
extern unsigned short in_sw(unsigned short port);

extern void out_b(unsigned short port, unsigned char  val);
extern void out_w(unsigned short port, unsigned short val);

#endif // __IO_H
