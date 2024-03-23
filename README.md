# ProjetRayTracing
Projet d'IN204 :Object Oriented Ray Tracing 

Auteurs: AZOUZ SALIMA -MARIEM DOGGAZ
Paramètres de la scène:
Paramètres d'image:
largeur_img
hauteur_img
nombre echantillons_par_pixel;
profondeur_max de récursion pour limiter le nombre de rebonds des rayons réfléchies et réfractés et eviter les boucles infinies
Paramètres de la caméra:
viewerPosition
lowerLeft
lensDiameter
startTime, endTime
gazeAt
verticalFieldOfView, aspectRatio, apertureSize, focalDistance

Paramètres des objets de  la scène:
Il existe deux catégories d'objets dans notre système : les sphères statiques et les sphères en mouvement. Les deux catégories partagent des caractéristiques communes telles que le rayon et la position. Cependant, les sphères en mouvement possèdent des attributs supplémentaires : deux positions et deux temps, un pour chaque position.

En ce qui concerne les matériaux disponibles pour les objets, nous disposons de trois options :
Lambertien
Diélectrique
Métal


