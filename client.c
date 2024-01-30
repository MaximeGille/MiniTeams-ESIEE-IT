#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


// Fonction pour envoyer les signaux en fonction du message binaire
void sendSignals(pid_t pid, char c) {
    for (int i = 7; i >= 0; --i) {
        int bit = (c >> i) & 1; //Transformation du charactère en binaire
        if (bit == 1) {
            kill(pid, SIGUSR1); // Si 1 alors envoie d'un SIGUSR1 au PID en première argument
        } else {
            kill(pid, SIGUSR2); // Si 0 alors envoie d'un SIGUSR2 au PID en première argument
        }
        usleep(200); // Pause pour assurer la synchronisation
    }
}
// Fonction pour vérifier chaque charactère du PID 
int isNumeric(char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    // Message d'erreur si les arguments ne sont pas correcte"  
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <Message>\n", argv[0]);
        return 1;
    }
    
        if (!isNumeric(argv[1]))
    {
        fprintf(stderr, "Erreur : l'argument <PID cible> doit être numérique.\n");
        return 1;
    }
    
    // Sauvegarde le PID dans sa variable 
    pid_t pid = atoi(argv[1]);

    // Sauvegarde le message dans sa variable 
    char *message = argv[2];
    
    // Message d'erreur si le message dépasse 1024 charactères 
    if (strlen(message) > 1024)
    {
        fprintf(stderr, "Votre message doit faire moins de 1024 charactères\n");
        return EXIT_FAILURE;
    }

    // Envoie du message
    for (int i = 0; message[i] != '\0'; ++i) {
        sendSignals(pid, message[i]);
    }

    // Envoie du caractère \0 pour indiquer la fin du message
    sendSignals(pid, '\0');

    return 0;
}

