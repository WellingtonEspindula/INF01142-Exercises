//
// Created by wmespindula on 12/08/2021.
//

#include <malloc.h>
#include <string.h>
#include "bytearray.h"

ByteArray serialize(const void *data, void (*serializeFunc)(const byte_t *, const void *, DecodMetadata metadata[]),
                    size_t (*lengthFunc)(const void *), int fieldCount) {
    size_t metadataSize = sizeof(DecodMetadata) * fieldCount;
    size_t structureSize = lengthFunc(data);
    size_t totalSize = metadataSize + structureSize;

    byte_t *serializedData = (byte_t *) malloc(structureSize);
    DecodMetadata metadata[fieldCount];
    serializeFunc(serializedData, data, metadata);

    size_t offset = 0;
    byte_t *serializedMetadata = (byte_t *) malloc(metadataSize);
    memcpy(&(serializedMetadata[offset]), metadata, (sizeof(DecodMetadata) * fieldCount));

    byte_t *allSerializedData = (byte_t *) malloc(totalSize);
    memcpy(allSerializedData, serializedMetadata, metadataSize);
    memcpy(&(allSerializedData[metadataSize]), serializedData, structureSize);

    free(serializedData);
    free(serializedMetadata);

    ByteArray newByteArray = {allSerializedData, totalSize};
    return newByteArray;
}

void *deserialize(ByteArray byteArray, void *(*deserializeFunc)(const void *, DecodMetadata metadata[]),
                  int fieldCount) {
    size_t metadataSize = fieldCount * sizeof(DecodMetadata);
    DecodMetadata metadata[fieldCount];
    memcpy(metadata, byteArray.data, metadataSize);

    size_t structureSize = byteArray.length * sizeof(byte_t);
    byte_t *serializedData = (byte_t *) malloc(structureSize);
    memcpy(serializedData, &(byteArray.data[metadataSize]), structureSize);

    void *data = deserializeFunc(serializedData, metadata);

    free(serializedData);

    return data;
}
