#include "streamer.h"
#include "../config.h"
#include "../memory/heap/kheap.h"

struct disk_stream * diskstream_new(int disk_id)
{
    struct disk * disk = disk_get(disk_id);
    if ( !disk )
    {
        return 0;
    }

    struct disk_stream * streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos  = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstream_seek(struct disk_stream * stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstream_read(struct disk_stream * stream, void * out, int total)
{
    int sector = stream->pos / SECTOR_SIZE;
    int offset = stream->pos % SECTOR_SIZE;
    char buf[SECTOR_SIZE];

    int result = disk_read_block(stream->disk, sector, 1, buf);
    if ( result < 0 )
    {
        goto out;
    }

    int total_to_read = (total > SECTOR_SIZE) ? SECTOR_SIZE : total;
    for (int i = 0; i < total_to_read; i++)
    {
        * (char*) out++ = buf[offset + i];
    }

    // adjust the stream
    stream->pos += total_to_read;
    if (total > SECTOR_SIZE)
    {
        result = diskstream_read(stream, out, total - SECTOR_SIZE);
    }

out:
    return result;
}

void diskstream_close(struct disk_stream * stream)
{
    kfree(stream);
}
