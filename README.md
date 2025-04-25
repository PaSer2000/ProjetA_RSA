# ProjetA_RSA

#PHASE 1
La valeur de MAXPRIME n'est pas modifiable étant donné qu'on obtient sinon un dépassement de capacité pour les fonctions.

#PHASE 2
On a pu augmenté MAXPRIME jusqu'à 10^18 avant d'avoir un dépassement de capacité.
On a décidé d'encoder la clé publique en hexadécimal plutôt qu'en base64 pour différentes raisons:
Pas de souci avec les caractères spéciaux mal interprétés (pas de /, +, =…).
Possible de le mettre dans un shell, une URL,...(par exemple en paramètre de shell ce ne sera pas mal interprété).
