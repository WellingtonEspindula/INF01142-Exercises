//
// Created by wmespindula on 12/08/2021.
//

#include "string.h"

enum FieldEnumerate {
    SIZE = 0,
    STRING = 1
};

String newString(const char *string) {
    String newString;
    size_t newStringSize = strlen(string);

    newString.string = (char *) malloc(sizeof(char) * newStringSize);
    newString.size = newStringSize;
    strncpy(newString.string, string, newStringSize);

    return newString;
}

void deleteString(const String string) {
    free(string.string);
}

void stringSerialize(const byte_t *serializedData, void *inputString,
                     DecodMetadata metadata[STRING_FIELD_COUNT]) {
    String *string = ((String *) inputString);
    int offset = 0;
    memcpy((void *) &(serializedData[0]), (const void *) &(string->size), sizeof(size_t));

    DecodMetadata sizeField = {SIZE, offset};
    metadata[0] = sizeField;

    offset += sizeof(size_t);
    strncpy((char *) &(serializedData[offset]), string->string, string->size);

    DecodMetadata stringField = {STRING, offset};
    metadata[1] = stringField;
}

void *stringDeserialize(const byte_t *serializedData,
                        DecodMetadata metadata[STRING_FIELD_COUNT]) {
    size_t size = -1;
    char *string = NULL;
    int offset = 0;

    for (int i = 0; i < STRING_FIELD_COUNT; i++) {
        if (metadata[i].fieldId == SIZE) {
            offset = metadata[i].offset;
            memcpy(&(size), &(serializedData[offset]), sizeof(size_t));
//            printf("New String size defined: %zu\n", size);
        } else if (metadata[i].fieldId == STRING) {
            offset = metadata[i].offset;
            size_t stringSize = size * sizeof(char);
            string = (char *) malloc(stringSize);
            strncpy(string, &(serializedData[offset]), stringSize);
//            printf("New String defined: %s\n", string);
        }
    }

    String *newString = (String *) malloc(sizeof(size_t)+ (sizeof(char) * size));
    newString->size = size;
    newString->string = string;
    return newString;
}

size_t stringLength(const void *inputString) {
    String *string = (String *) inputString;
    return sizeof(size_t) + (sizeof(char) * string->size);
}
