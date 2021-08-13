//
// Created by wmespindula on 12/08/2021.
//

#ifndef SISOP_STRING_H
#define SISOP_STRING_H

#include <string.h>
#include <malloc.h>
#include <stddef.h>
#include "bytearray.h"

#define STRING_FIELD_COUNT 2

typedef struct String {
    char *string;
    size_t size;
} String;

String newString(const char *string);
void deleteString(String string);

void stringSerialize(const byte_t *serializedData, void *inputString,
                     DecodMetadata metadata[STRING_FIELD_COUNT]);
void *stringDeserialize(const byte_t *serializedData,
                        DecodMetadata metadata[STRING_FIELD_COUNT]);
size_t stringLength(const void *inputString);

#endif //SISOP_STRING_H
