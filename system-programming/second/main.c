#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include "bytearray.h"
#include "string.h"

int main() {
    int pipeChannel[2];
    pipe(pipeChannel);

    pid_t pid = fork();

    if (pid != 0) {
        printf("Pai: Eu vou mandar...\n");

        String message = newString("Weeeeeeeee");
        ByteArray serialization = serialize((void *) &message, stringSerialize, stringLength, STRING_FIELD_COUNT);

        printf("Should send %zu bytes\n", serialization.length);
        write(pipeChannel[1], &(serialization.length), sizeof(size_t));
        ssize_t bytesCount = write(pipeChannel[1], serialization.data, serialization.length);
        if (bytesCount == serialization.length) {
            printf("[OK] Sending %lu bytes to pipe\n", bytesCount);
        } else {
            printf("[ERROR] Bytes sent differs than it should be sent!");
        }
        deleteString(message);
        close(pipeChannel[1]);
    } else {
        ByteArray byteArray;

        size_t byteArrayLength = 0;
        read(pipeChannel[0], &byteArrayLength, sizeof(size_t));
//        printf("Should receive %zu bytes\n", byteArrayLength);

        byteArray.data = (byte_t *) malloc(byteArrayLength * sizeof(byte_t));
        byteArray.length = byteArrayLength;

        ssize_t bytes = read(pipeChannel[0], byteArray.data, byteArrayLength * sizeof(byte_t));
//        printf("Read %zd bytes from pipe\n", bytes);

        String *myStringBack = (String *) deserialize(byteArray, stringDeserialize, STRING_FIELD_COUNT);
        printf("Filho: %s", myStringBack->string);

        deleteString(*myStringBack);

        close(pipeChannel[0]);
    }
    return 0;
}
