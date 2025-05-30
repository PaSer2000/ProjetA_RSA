# ProjetA_RSA
Groupe 5 : 
CHALOT Mael
DA SILVA Lucas
SERRATOS Paul
VIALA Timothé

#PHASE 1
La valeur de MAXPRIME n'est pas modifiable étant donné qu'on obtient sinon un dépassement de capacité pour les fonctions.
Pour créer l'éxecutable il faut effectuer la commande make pahse2_exec, puis ./phase1_exec pour le lancer.

#PHASE 2
Nécessité d'installer GMP.
On a pu augmenté MAXPRIME jusqu'à 10^18 avant d'avoir un dépassement de capacité.
On a décidé d'encoder la clé publique en hexadécimal plutôt qu'en base64 pour différentes raisons:
Pas de souci avec les caractères spéciaux mal interprétés (pas de /, +, =…).
Possible de le mettre dans un shell, une URL,...(par exemple en paramètre de shell ce ne sera pas mal interprété).
Pour créer l'éxecutable il faut effectuer la commande make phase2_exec, puis ./phase2_exec pour le lancer.

#PHASE 3
Nous avons réussi à implémenter l'ensemble des fonctionnalités demandées.
Pour créer l'éxecutable il faut effectuer la commande make pahse3_exec, puis ./phase3_exec pour le lancer.

#PHASE 4
Nous avons implémenter la totalité malgré que le code ne soit pas otpimisé.
Etant donné que c'est la suite de la phase 3, ce sont les mêmes commandes qu'il faut effectuer.

#PHASE 5
Temps manquant, nous n'avons pas pu la faire.

#CONCLUSION 
Le code répond aux fonctionnalités demandées jusqu'à la phase 5 que nous n'avons pas eu le temps d'implémenter.
Il est optimisé jusqu'à la phase 3 inclus mais pas dans la phase 4 par manque de temps.


