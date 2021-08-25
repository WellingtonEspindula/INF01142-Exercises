//
// Created by wmespindula on 18/08/2021.
//

#ifndef SISOP_USER_H
#define SISOP_USER_H

#include "string.h"
#include "bytearray.h"

#define USER_FIELD_COUNT 5

typedef struct User {
    int userId;
    String firstName;
    String lastName;
    int age;
    float weight;
} User;

User newUser(int userId, const char* firstName, const char* lastNae, int age,
             float weight);
void userSerialize(const byte_t *serializedData, const void *inputUser,
                     DecodMetadata metadata[USER_FIELD_COUNT]);
void *userDeserialize(const byte_t *serializedData,
                        DecodMetadata metadata[USER_FIELD_COUNT]);
size_t userLength(const void *inputUser);
void printUser(User user);
void deleteUser(User user);

#endif //SISOP_USER_H
