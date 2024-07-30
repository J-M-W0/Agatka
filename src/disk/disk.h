#ifndef __DISK_H
#define __DISK_H

typedef unsigned int DISK_TYPE;

// represents a real physical hard disk.
#define DISK_TYPE_REAL 0

struct disk 
{
    DISK_TYPE type;
    int       sector_size;
};

void disk_search_and_init();
struct disk* disk_get(int disk);

int disk_read_sector(int lba, int total, void* buf);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif // __DISK_H
