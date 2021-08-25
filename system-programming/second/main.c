#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/wait.h>
#include "bytearray.h"
#include "string.h"
#include "user.h"

int main() {
    int pipeChannel[2];
    pipe(pipeChannel);

    pid_t pid = fork();

    if (pid != 0) {
        User user = newUser(1, "Wellington", "Espindula", 22, 78.2);
        printUser(user);
        ByteArray serialization = serialize((void *) &user, userSerialize, userLength, USER_FIELD_COUNT);
//        printf("%s", serializedUser);
//        ByteArray serialization = serialize((void *) &message, stringSerialize, stringLength, STRING_FIELD_COUNT);

        printf("Should send %zu bytes\n", serialization.length);
        write(pipeChannel[1], &(serialization.length), sizeof(size_t));
        ssize_t bytesCount = write(pipeChannel[1], serialization.data, serialization.length);
        if (bytesCount == serialization.length) {
            printf("[OK] Sending %lu bytes to pipe\n", bytesCount);
        } else {
            printf("[ERROR] Bytes sent differs than it should be sent!");
        }
        close(pipeChannel[1]);

        int status = -1;
        waitpid(pid, &status, 0);
    } else {
        ByteArray byteArray;

        size_t byteArrayLength = 0;
        read(pipeChannel[0], &byteArrayLength, sizeof(size_t));

        byteArray.data = (byte_t *) malloc(byteArrayLength * sizeof(byte_t));
        byteArray.length = byteArrayLength;

        ssize_t bytes = read(pipeChannel[0], byteArray.data, byteArrayLength * sizeof(byte_t));
        printf("Read %zd bytes from pipe\n", bytes);

        User *myUserBack = (User *) deserialize(byteArray, userDeserialize, USER_FIELD_COUNT);
        printUser(*myUserBack);

        deleteUser(*myUserBack);

        close(pipeChannel[0]);
    }
    return 0;
}
