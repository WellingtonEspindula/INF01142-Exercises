//
// Created by wmespindula on 18/08/2021.
//

#include "user.h"

enum FieldEnumerator {
    USER_ID = 0,
    FIRSTNAME = 1,
    LASTNAME = 2,
    AGE = 3,
    WEIGHT = 4,
};

User newUser(int userId, const char* firstName, const char* lastName, int age,
             float weight) {
    User newUser = {userId, newString(firstName), newString(lastName), age, weight};
    return newUser;
}

void userSerialize(const byte_t *serializedData, const void *inputUser,
                   DecodMetadata metadata[USER_FIELD_COUNT]) {
    User *user = (User *) inputUser;
    int offset = 0;
    memcpy((void *) &(serializedData[offset]), (const void *) &(user->userId), sizeof(int));

    DecodMetadata userIdMD = {USER_ID, offset};
    metadata[0] = userIdMD;
    offset += sizeof(int);

    ByteArray firstNameBytes = serialize((void *) &(user->firstName), stringSerialize, stringLength, STRING_FIELD_COUNT);
    size_t firstNameBytesSize = sizeof(byte_t) * firstNameBytes.length;
    memcpy((void *) &(serializedData[offset]), firstNameBytes.data, firstNameBytesSize);
    DecodMetadata firstNameMD = {FIRSTNAME, offset};
    metadata[1] = firstNameMD;
    offset += (int) firstNameBytesSize;

    ByteArray lastNameBytes = serialize((void *) &(user->lastName), stringSerialize, stringLength, STRING_FIELD_COUNT);
    size_t lastNameBytesSize = sizeof(byte_t) * lastNameBytes.length;
    memcpy((void *) &(serializedData[offset]), lastNameBytes.data, lastNameBytesSize);
    DecodMetadata lastNameMD = {LASTNAME, offset};
    metadata[2] = lastNameMD;
    offset += (int) lastNameBytesSize;

    memcpy((void *) &(serializedData[offset]), &(user->age), sizeof(int));
    DecodMetadata ageMD = {AGE, offset};
    metadata[3] = ageMD;
    offset += sizeof(int);

    memcpy((void *) &(serializedData[offset]), &(user->weight), sizeof(float));
    DecodMetadata weightMD = {WEIGHT, offset};
    metadata[4] = weightMD;
}

void *userDeserialize(const byte_t *serializedData,
                      DecodMetadata metadata[USER_FIELD_COUNT]) {

    int offset = 0;

    User *newUser = (User *) malloc(sizeof(User));

    for (int i = 0; i < USER_FIELD_COUNT; i++) {
        uint8_t fieldId = metadata[i].fieldId;
        offset = metadata[i].offset;

//        printf("fieldId=%d;offset=%d\n", fieldId, offset);

        if (fieldId == USER_ID) {
            memcpy(&(newUser->userId), &(serializedData[offset]), sizeof(int));
        } else if (fieldId == FIRSTNAME){
            ByteArray firstNameBA = {(byte_t *) &(serializedData[offset]), (metadata[i+1].offset - offset)};
            String *firstName = (String *) deserialize(firstNameBA, stringDeserialize, STRING_FIELD_COUNT);
            newUser->firstName = *(firstName);
        } else if (fieldId == LASTNAME) {
            ByteArray lastNameBA = {(byte_t *) &(serializedData[offset]), (metadata[i + 1].offset - offset)};
            String *lastName = (String *) deserialize(lastNameBA, stringDeserialize, STRING_FIELD_COUNT);
            newUser->lastName = *(lastName);
        } else if (fieldId == AGE) {
            memcpy(&(newUser->age), &(serializedData[offset]), sizeof(int));
        } else if (fieldId == WEIGHT) {
            memcpy(&(newUser->weight), &(serializedData[offset]), sizeof(float));
        }
    }

    return newUser;
}

size_t userLength(const void *inputUser) {
    User *user = (User*) inputUser;
    return ((sizeof(int) * 2) + sizeof(float) +
        stringLength(&(user->firstName)) + (sizeof(DecodMetadata)*STRING_FIELD_COUNT * 2) +
        stringLength(&(user->lastName)));
}


void printUser(User user) {
    printf("User=[userId=%d; firstName=%s; lastName=%s, age=%d; weight=%.2f]\n", user.userId, user.firstName.string,
           user.lastName.string, user.age, user.weight);
}

void deleteUser(User user) {
    deleteString(user.firstName);
    deleteString(user.lastName);
}
