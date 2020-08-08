#include "util.h"

typedef struct RadixMemoryBlob {
    unsigned long long length;
    void *ptr;
    bool heap;
} RadixMemoryBlob;

RadixMemoryBlob RadixAbstract_ConstructPointerBlob(
    void *dest,
    unsigned long long length)
{
    RadixMemoryBlob final = {length, dest, false}; return final;
}

RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length)
{
    RadixMemoryBlob temp = RadixAbstract_ConstructPointerBlob(
        malloc(length), length);
    temp.heap = true;
    return temp;
}

RadixMemoryBlob RadixAbstract_SliceBlob(
    RadixMemoryBlob *blob,
    unsigned long long offset,
    unsigned long long length)
{
    RadixMemoryBlob temp = RadixAbstract_ConstructPointerBlob(
        (blob->ptr)+offset, length);
    temp.heap = blob->heap;
    return temp;
}

void RadixAbstract_InsertBlob(
    RadixMemoryBlob *source,
    RadixMemoryBlob *target,
    unsigned long long target_offset)
{
    memcpy((target->ptr)+target_offset, source->ptr, source->length);
}

unsigned long long RadixAbstract_GetBlobLength(RadixMemoryBlob *blob)
{
    return blob->length;
}

void * RadixAbstract_GetBlobPointer(RadixMemoryBlob *blob)
{
    return blob->ptr;
}

void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob)
{
    if (blob->heap) {
        free(blob->ptr);
    }
}