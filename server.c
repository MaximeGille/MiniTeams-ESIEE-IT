#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024


char binaryMessage[MAX_MESSAGE_LENGTH];
int binaryIndex = 0;
int messageComplete = 0;
pid_t clientPid = 0;

// Handler pour SIGUSR1
void handleSIGUSR1() {
    binaryMessage[binaryIndex++] = '1';
}

// Handler pour SIGUSR2
void handleSIGUSR2() {
    binaryMessage[binaryIndex++] = '0';
}

// Handler pour SIGALRM
void handleSIGALRM(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused))) {
    if (clientPid == 0) {
        // Stocker le PID du client
        clientPid = si->si_pid;
    } else {
        // Traitement de la fin du message
        messageComplete = 1;
    }
}
// Convertit un message binaire en texte
void binaryToText(char *binary, char *text) {
    while (*binary) {
        char byte = 0;
        for (int i = 0; i < 8 && *binary; ++i) {
            byte = (byte << 1) | (*binary++ - '0');
        }
        *text++ = byte;
    }
    *text = '\0';
}

void saveMessage(const char *message)
{
    FILE *file = fopen("LogsServer.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}


int main() {
    printf("PID du Serveur: %d\n", getpid());

    struct sigaction sa1, sa2, sa3;
    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_handler = handleSIGUSR1;
    sigaction(SIGUSR1, &sa1, NULL);

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = handleSIGUSR2;
    sigaction(SIGUSR2, &sa2, NULL);

    memset(&sa3, 0, sizeof(sa3));
    sa3.sa_flags = SA_SIGINFO;
    sa3.sa_sigaction = handleSIGALRM;
    sigaction(SIGALRM, &sa3, NULL);

    while (!messageComplete) {
        pause();

        if (messageComplete) {
            binaryMessage[binaryIndex] = '\0'; // Fin du message

            char textMessage[MAX_MESSAGE_LENGTH];
            binaryToText(binaryMessage, textMessage);

            time_t currentTime;
            time(&currentTime);
            struct tm *localTime = localtime(&currentTime);

	    char formattedMessage[2048];
            sprintf(formattedMessage, "[%02d:%02d:%02d] Message de %d: %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, clientPid, textMessage);

            saveMessage(formattedMessage);
            printf("%s", formattedMessage);

            memset(binaryMessage, 0, MAX_MESSAGE_LENGTH);
            binaryIndex = 0;
            messageComplete = 0;
            clientPid = 0; // Réinitialiser pour le prochain client
        }
    }

    return 0;
}
