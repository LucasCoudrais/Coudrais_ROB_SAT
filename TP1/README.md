# Controle les phares
0x123 [2] 00 01 => Cligno gauche
0x123 [2] 00 02 => cligno droit
0x123 [2] 01 00 => phare
0x123 [2] 02 00 => plein phare
0x123 [2] 00 00 => eteindre les phares

# Pour le controle des pédales et volant
0x321 [3] 64 00 09 => On avance vitesse de 64 et on tourne à 09
0x321 [3] 10 00 02 => On ralentit donc on avance a 10 et on tourne à 02
0x321 [3] 10 00 00 
0x321 [3] 00 20 00 => On freine à 20
On envoie à l'id 321 avec une taille de 3 bytes.
Le premier représente la force d'accélération
Le second la force de freinage
Le 3eme l'angle de virage

# Choix du masque / intervalle de filtrage 
on choisit ici 0xF00 comme masque pour que ça complète la condition donnée <received_can_id> & mask == can_id & mask
ici on choisit ce masque un peu comme les masque qu'on établit en réseau
par exemple : avec notre masque   1111 1111  0000 0000  0000 0000
avec qqch invalide comme 0x300 => 0000 0011  0000 0000  0000 0000
application du & logique =>       0000 0011  0000 0000  0000 0000
borne moins (0x100) avec et logique  masque : 0000 0001  0000 0000  0000 0000 => condition borne min <= valeur ok 
borne max (0x1FF) avec et logique masque : 0000 0001  0000 0000  0000 0000 => condition borne max >= valeur pas ok
en fait il faut choisir un masque qui fera, par un ou logique avec la valeur, toujours une valeur comprise entre nos bornes
	