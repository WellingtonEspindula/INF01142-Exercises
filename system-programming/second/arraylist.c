//
// Created by wmespindula on 12/08/2021.
//

#include <malloc.h>
#include <string.h>
#include "arraylist.h"

ArrayList emptyArrayList(size_t representationSize) {
    ArrayList arrayList = {NULL, 0, representationSize};
    return arrayList;
}

ArrayList initArrayList(void *list, int length, size_t representationSize) {
    ArrayList arrayList = {list, length, representationSize};
    return arrayList;
}

void add(ArrayList *list, void *element) {
    void *newList = malloc((list->length + 1) * list->representationSize);
    memcpy(newList, list->list, (list->length * list->representationSize));
    free(list->list);
    list->list = newList;
    list->length++;
}

void *get(ArrayList *list, int num) {
    if (num < list->length) {
        void *newList = malloc(list->representationSize);
        int offset = ((int) list->representationSize)*num;
        memcpy(newList, &(list->list[offset]), list->representationSize);
        return (void *) newList;
    } else {
        return NULL;
    }
}

//void remove(ArrayList *list, int num) {
//    if (num < list->length) {
//        return list->list[num];
//    } else {
//        return NULL;
//    }
//}