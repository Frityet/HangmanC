//
// Created by Frityet on 2022-01-17.
//

#ifndef UNIGS_LIST_
#define UNIGS_LIST_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define ADD_ITEM(_list, _item) ({if (listinfo(_list)->used_spaces == listinfo(_list)->total_spaces) \
                                    grow_list(_list);                                             \
                               (_list)[free_index(_list)] = _item;            \
                               listinfo(_list)->used_spaces++;})

struct list_data {
    size_t      type_size, total_size, used_spaces, total_spaces;
    void        *array;
};

static inline struct list_data *listinfo(void *ptr)
{
    struct list_data *list = (struct list_data *)(ptr - sizeof(struct list_data));
    if (list->array != ptr) {
        fprintf(stderr, "Could not get info of ptr %p! Are you sure this is a list?", ptr);
        return NULL;
    }
    return list;
}

void *create_list(size_t type_size, size_t count);
void grow_list(void *ptr);
void *remove_item(void *ptr, size_t index);

static inline unsigned int free_index(void *list)
{
    return listinfo(list)->used_spaces;
}


static inline size_t list_length(void *ptr)
{
    return listinfo(ptr)->used_spaces;
}

static inline size_t list_max(void *ptr)
{
    return listinfo(ptr)->total_spaces;
}

static inline void free_list(void *ptr)
{
    free(listinfo(ptr));
}

#endif //UNIGS_LIST_
