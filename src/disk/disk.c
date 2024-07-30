#include "disk.h"
#include "../config.h"
#include "../status.h"
#include "../io/io.h"
#include "../memory/memory.h"

struct disk disk;

int disk_read_sector(int lba, int total, void *buf)
{
    out_b(0x01f6, (lba >> 24) | 0xe0);
    out_b(0x01f2, total);
    out_b(0x01f3, (unsigned char) (lba & 0xff));
    out_b(0x01f4, (unsigned char) (lba >> 8));
    out_b(0x01f5, (unsigned char) (lba >> 16));
    out_b(0x01f7, 0x20);

    unsigned short * ptr = (unsigned short *) buf;
    for (int b = 0; b < total; b++)
    {
        // wait for the buffer to be ready.
        char c = in_sb(0x01f7);
        while ( !(c & 0x08) ) 
        {
            c = in_sb(0x01f7);
        }

        // copy from hard disk to memory.
        for (int i = 0; i < 256; i++)
        {
            *ptr = in_sw(0x01f0);
            ptr++;
        }
    }

    return 0;
}

void disk_search_and_init() 
{
    mem_set(&disk, 0, sizeof(disk));
    disk.type = DISK_TYPE_REAL;
    disk.sector_size = SECTOR_SIZE;
}

struct disk* disk_get(int index)
{
    if ( index != 0 )
    {
        return 0;
    }
    return &disk;
}

int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buf)
{
    if ( idisk != &disk )
    {
        return -EIO;
    }
    return disk_read_sector(lba, total, buf);
}
