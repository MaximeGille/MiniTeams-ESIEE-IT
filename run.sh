#!/bin/bash

# Récupérer le répertoire où le script est situé
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Ouvrir un terminal pour exécuter './server'
gnome-terminal --working-directory="$SCRIPT_DIR" -- bash -c "./server; exec bash" &

# Ouvrir un autre terminal pour exécuter './client'
gnome-terminal --working-directory="$SCRIPT_DIR" -- bash -c "./client; exec bash" &

