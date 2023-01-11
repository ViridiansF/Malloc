#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#include "../src/metadata.h"
#include "../src/colors.h"

#include <stddef.h>
 

void registerAllocatorHook(void);

void* my_malloc(size_t size);
void* my_realloc(void* ptr, size_t size);
void* my_calloc(size_t nb, size_t size);
void my_free(void* ptr);

void afficherMetaData();
void printMetaData(metaData* node, int i);
