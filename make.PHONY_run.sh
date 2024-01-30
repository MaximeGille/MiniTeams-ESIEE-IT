#!/bin/bash

# Récupérer le répertoire où le script est situé
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Ouvrir un terminal GNOME dans ce répertoire et exécuter la commande 'make'
gnome-terminal --working-directory="$SCRIPT_DIR" -- bash -c "make .PHONY; exec bash"
