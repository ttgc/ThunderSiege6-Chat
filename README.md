# Thunder Siege 6 - Chat library

 Networking chat for Thunder Siege 6 project

## Quel protocole transport utilisez-vous pour ce genre de messages ?

Nous avons choisi d'utiliser, pour ce genre de messages, le protocole **TCP**. 

En effet, le protocole TCP assure l'arrivée des paquets dans l'ordre tout en traçant leur origine, de ce fait, il est possible de savoir qui a envoyé quoi à quel moment grâce à ce protocole. Ces avantages sont primordiaux pour la gestion d'un chat réseau, a fortiori celui d'un jeu multijoueur compétitif tel que ThunderSiege 6, puisqu'il est capital de savoir qui envoie quel message à quel moment pour pouvoir ensuite l'afficher aux autres utilisateurs.

Les paquets transmis sont vérifiables, traçables, et limite la corruption et la perte de données. Finalement c'est l'ensemble de ces points qui ont orienté notre choix sur le protocole TCP.
