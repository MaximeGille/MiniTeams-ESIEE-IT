#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024
#define BYTE 8
// Déclaration des Variables
char binaryChar[BYTE];
int binaryIndex = 0;
char message[MAX_MESSAGE_LENGTH];
int messageIndex = 0;
pid_t clientPid = 0;

// Handler pour SIGUSR1
void handleSIGUSR1(int sig __attribute__((unused)), siginfo_t *si, void *context __attribute__((unused))) {
	if (clientPid == 0)
		clientPid = si->si_pid;    
	if (binaryIndex < BYTE) {
        binaryChar[binaryIndex++] = '1';
    }
}

// Handler pour SIGUSR2
void handleSIGUSR2() {
    if (binaryIndex < BYTE) {
        binaryChar[binaryIndex++] = '0';
    }
}

// Convertit un caractère binaire en caractère ASCII
char binaryToASCII(char *binary) {
    char result = 0;
    for (int i = 0; i < BYTE; ++i) {
        result = (result << 1) | (binary[i] - '0');
    }
    return result;
}
// Créer le fichier logs puis imprime le contenu de message à l'intèrieur
void saveMessage(const char *message) {
    FILE *file = fopen("LogServer.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

int main() {
// Affiche le PID du serveur
    printf("PID du Serveur: %d\n", getpid());

    struct sigaction sa1, sa2;
    memset(&sa1, 0, sizeof(sa1));
    sa1.sa_sigaction = handleSIGUSR1;
    sa1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa1, NULL);

    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = handleSIGUSR2;
    sigaction(SIGUSR2, &sa2, NULL);

    while (1) {
        pause();
// Vérification CHAR est complet
        if (binaryIndex == BYTE) {
            char asciiChar = binaryToASCII(binaryChar);

            if (asciiChar == '\0') {
                // Fin du message
                message[messageIndex] = '\0';

                // Récupération heure de la machine
            	time_t currentTime;
            	time(&currentTime);
            	struct tm *localTime = localtime(&currentTime);

                // Traitement du message reçu
                char formattedMessage[MAX_MESSAGE_LENGTH + 50];
                sprintf(formattedMessage, "[%02d:%02d:%02d] Message de %d: %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, clientPid, message);
                saveMessage(formattedMessage);
                printf("%s", formattedMessage);

                // Réinitialiser pour le prochain message
                memset(message, 0, MAX_MESSAGE_LENGTH);
                messageIndex = 0;
                clientPid = 0;
            } else {
                // Ajout du caractère au message
                if (messageIndex < MAX_MESSAGE_LENGTH) {
                    message[messageIndex++] = asciiChar;
                }
            }

            // Réinitialiser pour le prochain caractère
            memset(binaryChar, 0, BYTE);
            binaryIndex = 0;
        }
    }

    return 0;
}

