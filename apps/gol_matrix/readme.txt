Fait par Théo Plockyn & Rémy Debue

Notre projet de base ( communication entre mbed par UDP ) était trop difficile pour nous, nous n'avancions pas.
Nous avons donc choisi de nous diriger vers un autre projet, qui est une appli web pour lancer un jeu de la vie de Conway sur le mbed, affiché sur la matrice de leds.

La principale difficulté de ce projet a été le premier contact avec des technologies du web, ce qui a ralenti le développement du projet. Nous avons 
- un jeu de la vie qui marche ( et qui s'arrête lors de boules infinies dans la simulation )
- son affichage
- une page web où l'on peut dessiner sur la grille et l'envoyer sur la mbed
- le bouton pour envoyer les données et lancer le jeu
mais la communication entre la page web et le jeu de la vie ne se fait pas bien, au niveau du tableau de caractères, et l'absence de retours des erreurs inhérente au développement embarqué nous a bloqué.

Nous voulions rajouter à ce projet :
- Une gestion de la vitesse des générations avec l'un des potentiomètres du mbed
- L'affichage en temps réel ( avec comet et ajax ) de la matrice de leds une fois le jeu lancé
- Le choix du nombre de générations par l'utilisateur
