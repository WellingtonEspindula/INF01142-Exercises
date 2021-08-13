//
// Created by wmespindula on 12/08/2021.
//

#ifndef SISOP_BYTEARRAY_H
#define SISOP_BYTEARRAY_H

typedef void byte_t;

#include <stddef.h>
#include <bits/stdint-uintn.h>

typedef struct DecodMetadata {
    uint8_t fieldId;
    uint8_t offset;
} DecodMetadata;

typedef struct ByteArray {
    byte_t *data;
    size_t length;
} ByteArray;


ByteArray serialize(const void *data, void (*serializeFunc)(const byte_t *, const void *, DecodMetadata metadata[]),
                    size_t (*lengthFunc)(const void *), int fieldCount);
void *deserialize(ByteArray byteArray, void *(*deserializeFunc)(const void *, DecodMetadata metadata[]),
                  int fieldCount);

#endif //SISOP_BYTEARRAY_H
