#ifndef __PPARSER_H
#define __PPARSER_H

struct path_root
{
    int drive_nr;
    struct path_part * first;
};

struct path_part
{
    const char       * part;
    struct path_part * next;
};

struct path_root* pathparser_parse(const char * path, const char * current_directory_path);
void pathparser_free(struct path_root * root);

#endif // __PPARSER_H
