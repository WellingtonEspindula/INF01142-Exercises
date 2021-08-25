#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

pid_t recursive_fork();

void send_message_pipe(pid_t pid, const char *message);

void receive_message_pipe(char message[100]);

int main() {
    int m = 0;
    printf("Inform processes numbers: ");
    scanf("%d", &m);

    pid_t pid = recursive_fork(1, m);
    return 0;
}

pid_t recursive_fork(int procCount, int maxProcesses) {
    int pipeChannel[2];
    pipe(pipeChannel);

    pid_t pid = fork();

    if (pid != 0) {
        char message[] = "Luke, I'm your father!";
        size_t messageSize = strlen(message);
        write(pipeChannel[1], &(messageSize), sizeof(size_t));

        ssize_t bytesCount = write(pipeChannel[1], message, messageSize * sizeof(char));
        printf("[Father; P=%d; PID=%d] Sent %zd bytes to pipe\n", procCount, getpid(), bytesCount);
        printf("[Father; P=%d; PID=%d] Message Sent: %s\n", procCount, getpid(), message);
        close(pipeChannel[1]);

        int status = -1;
        waitpid(pid, &status, 0);

        return pid;
    } else {
        procCount++;

        size_t messageSize = 0;
        read(pipeChannel[0], &messageSize, sizeof(size_t));

        char message[messageSize];
        ssize_t bytes = read(pipeChannel[0], message, (messageSize * sizeof(char)));
        printf("[Son; P%d; PID=%d] Read %zd bytes from pipe\n", procCount, getpid(), bytes);
        printf("[Son; P%d; PID=%d] Message Read: %s\n", procCount, getpid(), message);

        close(pipeChannel[0]);

        if (procCount < maxProcesses) {
            recursive_fork(procCount, maxProcesses);
        }
        return 0;
    }
}
