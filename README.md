# Projet Mini_Teams ESIEE-IT
 
### Fichier : 
- client.c : Script du client qui envoie les messages
- server.c : Script du server qui reçoit et affiches les messages
- Makefile : Makefile du projet `clean` `fclean` `re` `.PHONY`
- Make_run.sh : ouvre un terminal est éxécute la commande make qui compile les fichiers client & server
- Make.PHONY_run.sh : ouvre un terminal est éxécute la commande make .PHONY qui supprime les fichiers server client logServer.txt 
- run.sh : Ouvre 2 terminaux puis éxécute server et client 

### Utilisation : 
- server : `./server` 
- client : `./client <PID cible> <Message de moins de 1024 charactères>`

### Logs : 
- LogServer.txt : Une fois que le server à reçu un message il écrit un log du message reçu dans un fichier logServer.txt.  
