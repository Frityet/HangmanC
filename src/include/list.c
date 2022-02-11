//
// Created by Frityet on 2022-01-17.
//

#include "list.h"

#include <string.h>
#include <errno.h>
extern int errno;

static inline uint32_t round_to_2nd_power(uint32_t num)
{
    return num == 0 ? 0 : (num--, num |= num >> 1, num |= num >> 2, num |= num >> 4, num |= num >> 8, num |= num >> 16, ++num);
}

//    num--;
//    num |= num >> 1;
//    num |= num >> 2;
//    num |= num >> 4;
//    num |= num >> 8;
//    num |= num >> 16;
//    num++;
//
//    return num;


void *create_list(size_t type_size, size_t count)
{
    count = round_to_2nd_power(count);
    struct list_data *list = calloc(count, type_size);
    if (list == NULL) {
        fprintf(stderr, "Could not allocate list!\nReason: %s", strerror(errno));
    }
    list->used_spaces       = 0;
    list->total_spaces      = count;
    list->type_size         = type_size;
    list->total_size        = type_size * count;
    list->array             = list + 1;
    return list->array;
}

void grow_list(void *ptr)
{
    struct list_data *list = listinfo(ptr);

    if (list->array != ptr) {
        fprintf(stderr, "This ptr is not a list!\n%s:%d", __FILE__, __LINE__);
        return;
    }
    size_t new_size = list->type_size * (list->total_spaces *= 2);
    list->total_size = new_size;
    void *new_alloc = realloc(list, sizeof(struct list_data) + new_size);
    if (new_alloc == NULL)
        return;

//    free(ptr);
    list = new_alloc;
    list->array = list + 1;
}

void *remove_item(void *ptr, size_t index)
{
    struct list_data *info = listinfo(ptr);
    uint8_t *newlist = create_list(info->type_size, info->total_spaces - 1),
            *list_array = info->array;

    for (size_t i = 0; i < info->used_spaces; ++i) {
        newlist[i] = list_array[i];
    }

    return newlist;
}
