# Play Together

## Présentation
**Play Together** est un jeu de plateforme 2D collaboratif en ligne, développé en C++ avec la bibliothèque Simple DirectMedia Layer (SDL). Ce projet a été réalisé dans le cadre de notre cursus universitaire en licence informatique. Toutefois, l'idée du projet était entièrement notre propre initiative, et nous avons bénéficié de très peu d'encadrement de la part de nos responsables.

L'objectif principal de ce projet était de permettre à des étudiants en licence informatique de comprendre les mécanismes de création d'un jeu vidéo à partir de zéro, sans utiliser de moteur de jeu. Le processus détaillé et les insights sont documentés dans le rapport inclus dans ce dépôt [RAPPORT_PLAY_TOGETHER.pdf](./RAPPORT_PLAY_TOGETHER.pdf). De plus, une vidéo de démonstration est disponible sur YouTube : [Voir la Démo](https://www.youtube.com/watch?v=J_9TZDAv3z8).

## Caractéristiques
- **Moteur de jeu personnalisé :** Développé de A à Z en utilisant SDL.
- **Détection de collision :** Implémentée en utilisant les algorithmes AABB (Axis-Aligned Bounding Box) et SAT (Separating Axis Theorem).
- **Performance optimisée :** Utilisation des techniques de phase large (broad phase) et de phase étroite (narrow phase) pour gérer efficacement la détection des collisions et les différentes mécaniques de jeu.
- **Gestion des fps :** Utilisation du Delta Time pour garantir une expérience fluide et cohérente sur une variété de configurations matérielles.

## Techniques de détection de collision
### AABB (Axis-Aligned Bounding Box)
AABB est une méthode de détection de collision qui utilise des formes géométriques dont les côtés sont parallèles aux axes X ou Y. Cela signifie que les côtés de la forme sont toujours alignés avec les axes de coordonnées, ce qui simplifie les calculs. Nous avons utilisé AABB pour gérer les collisions dans diverses mécaniques du jeu, telles que la collecte d'items, l'inversion de gravité, et la collecte de pièces.

### SAT (Separating Axis Theorem)
SAT est un algorithme utilisé pour une détection de collision plus précise, applicable aux polygones convexes. Il fonctionne en projetant les sommets des polygones sur divers axes et en vérifiant les chevauchements. S'il n'y a pas de chevauchement sur un axe quelconque, il n'y a pas de collision. Nous avons utilisé SAT pour gérer les collisions avec le sol, car nous voulions inclure des pentes dans le jeu, nécessitant ainsi la gestion des collisions entre polygones.

## Optimisation des performances
### Phase Large (Broad Phase)
La phase large réduit le nombre de collisions potentielles en utilisant des algorithmes simples et rapides comme AABB pour identifier les paires d'objets qui pourraient entrer en collision.

### Phase Étendue (Narrow Phase)
Dans la phase étroite, des algorithmes détaillés et coûteux en calcul comme SAT sont appliqués aux paires identifiées dans la phase large pour déterminer avec précision les collisions.

## Documentation
Pour une explication approfondie du processus de développement du projet, des algorithmes utilisés et d'autres détails techniques, référez-vous au rapport complet inclus dans ce dépôt : [RAPPORT_PLAY_TOGETHER.pdf](./RAPPORT_PLAY_TOGETHER.pdf).

## Licence
Ce projet est sous licence GNU General Public License (GPL), version 3 ou ultérieure. Voir le fichier LICENSE pour plus de détails.