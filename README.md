# ProjetRayTracing
## Projet d'IN204 :Object Oriented Ray Tracing 

### Auteurs: AZOUZ SALIMA -MARIEM DOGGAZ

## Paramètres de la scène

### Paramètres d'image

   largeur_img

   hauteur_img

   nombre echantillons_par_pixel

   profondeur_max de récursion pour limiter le nombre de rebonds des rayons réfléchies et réfractés et eviter les boucles infinies

 ### Paramètres de la caméra

   viewerPosition

   lowerLeft

   lensDiameter

   startTime, endTime
   gazeAt

verticalFieldOfView, aspectRatio, apertureSize, focalDistance


### Paramètres des objets de  la scène:

Il existe deux catégories d'objets dans notre système : les sphères statiques et les sphères en mouvement. Les deux catégories partagent des caractéristiques communes telles que le rayon et la position. Cependant, les sphères en mouvement possèdent des attributs supplémentaires : deux positions et deux temps, un pour chaque position.

En ce qui concerne les matériaux disponibles pour les objets, nous disposons de trois options :
Lambertien
Diélectrique
Métal
## Options

Enter : Cette option démarre le rendu de la scène. Si aucune scène n'est chargée ou créée, le programme exécute une scène par défaut.

c : Cette option lance le tutoriel interactif où l'utilisateur peut définir les paramètres et les éléments présents dans la scène.

r : Cette option permet à l'utilisateur de charger un fichier XML contenant les paramètres et les éléments de la scène. 

p : Cette option charge une scène prédéfinie dans le programme.

s : Cette option permet de sauvegarder la scène créée ou chargée au format XML, ce qui permet de la charger ultérieurement.

i : Cette option sauvegarde l'image générée lors du rendu de la scène.

q : Cette option permet de quitter le programme.

## Instalation dans linux
Pour installer les dépendances requises par le projet sous Linux, veuillez exécuter les commandes suivantes :
sudo apt-get update
sudo apt-get install make
make install





