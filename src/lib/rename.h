
#ifndef RENAME_H
#define RENAME_H

#include <sys/stat.h>
#include <stdio.h>

#include "queue.h"
#include "files.h"

int rename_files(queue_t *file_queue, unsigned short overwrite);

#endif
