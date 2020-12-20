# Projet d’apprentissage de la programmation C++

## Description du projet

Le projet a pour but de mettre en pratique les connaissances acquises lors des séances d’apprentissage de la programmation. Le sujet de cette année est de développer en C++ un programme d’encodage/décodage de message en code Morse.
Pour un peu de culture, le code Morse a été inventé en 1832 pour la télégraphie. Le principe est d’associer à chaque lettre et chiffre une combinaison unique de signaux intermittents.
Le but du projet est donc de développer un programme permettant : (i) de convertir un message depuis le francais vers le Morse ; (ii) convertir un message Morse vers le francais. Cependant ce n’est pas tout… (trop facile sinon). Il faut que le message Morse soit stocké au format Audio afin que l’on puisse écouter votre message, en utilisant VLC par exemple !

<br><br>

## Programme attendu

### Scénario

#### Francais vers Morse

1. Un utilisateur lance le programme
2. Il spécifie au programme qu’il veut encoder un message
3. Il donne le message (soit via un fichier texte lu par le programme, soit en le saisissant au clavier directement)
4. Le programme génère un fichier audio contenant le message en Morse


#### Morse vers Français

1. L’utilisateur lance le programme
2. Il spécifie au programme qu’il veut décoder un message
3. Il donne le chemin du fichier audio contenant le message morse.
4. Le programme affiche à l’écran ou écrit dans un fichier texte le contenu du message décodé

<br>

### Attentes

En plus d’un programme fonctionnel nous attendons de votre part :

Un programme lisible et bien construit
* Des instruction de compilation claires, l’idéal étant de fournir un Makefile
* Une documentation utilisateur
* Des tests


Le programme ainsi que tous les éléments additionels (doc, test, …) devra être versionné sous Git et mis à disposition sur Github. <b>Vous devez envoyer à V. Roy, P. Thamie ou B. Marchand (selon votre groupe) pour le 24 Janvier 2021 avant 23h59 le lien vers votre dépôt Git.</b>

<br>

### Quelques indications techniques

* Pour la partie encodage/décodage vous êtes fortement encouragé à regarder ce qui peut-être fait via l’utilisation de graphes.
* Pour la partie lecture/écriture du fichier audio. Vous êtes fortement encouragé à utiliser le format WAV car ce dernier est relativement simple. Cela vous permet d’écrire/lire un fichier WAV à la main sans passer par une librairie externe.