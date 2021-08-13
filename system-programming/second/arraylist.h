//
// Created by wmespindula on 12/08/2021.
//

#ifndef SISOP_ARRAYLIST_H
#define SISOP_ARRAYLIST_H

#include <stddef.h>
#include <bits/stdint-uintn.h>

typedef struct ArrayList {
    void *list;
    int length;
    size_t representationSize;
} ArrayList;

ArrayList emptyArrayList(size_t representationSize);
ArrayList initArrayList(void *list, int length, size_t representationSize);
void add(ArrayList *list, void *element);
void *get(ArrayList *list, int num);


#endif //SISOP_ARRAYLIST_H
