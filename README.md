# IT45_project
Projet IT45 - Algorithme génétique
Ce programme est compilable sur toutes les plateformes, il est compilé en -03 et -flto donc la compilation peut être plus longue que d'ordinaire.

## Compilation
Vous pouvez le compiler en effectuant 
```
./build.sh
```
ou
```
./make
```
Il est également possible de clear (supprimer les .o) et clean (supprimer l'éxécutable généré et clear)
```
./make clear|clean
```

## Lancement
Après compilation, il faut le lancer avec
```
./run.sh
```
ou
```
./main
```

## Paramétrage
Par défaut, le programme se lance avec les paramètres suivants:
```
temps_max = 20
taille_population = 100
taux_croisement = 0.75
taux_mutation = 0.3
taille_chromosome = 100
folderInstance = "instances/100-10"
```
Il est également possible de lancer le programme avec ses propres instances et paramètres
Pour cela, il faudra veiller à nommer ses instances "Missions.csv", "Intervenants.csv" et "Distances.csv".
On peut lancer le programme avec des paramètres comme ceci :
```
./main 25 150 0.45 0.05 96 instances/96-6
```
ce qui signifie :
```
temps_max = 25
taille_population = 150
taux_croisement = 0.45
taux_mutation = 0.05
taille_chromosome = 96
folderInstance = "instances/96-6"
```
