
https://cours.polymtl.ca/inf2610/documentation/notes/chap6.pdf

Objet critique : Objet qui ne peut être accédé simultanément. Comme par
exemple, les imprimantes, la mémoire, les fichiers, les variables etc.

Section critique : Ensemble de suites d’instructions qui opèrent sur un ou
plusieurs objets critiques et qui peuvent produire des résultats impré-
visibles lorsqu’elles sont exécutées simultanément par des processus
différents.

En général, les processus qui exécutent
des sections critiques sont structurés comme suit :
– Section non critique.
– Demande d’entrée en section critique.
– Section critique.
– Demande de sortie de la section critique.
– Section non critique.

Quatre conditions sont nécessaires pour réaliser correctement une ex-
clusion mutuelle :
1. Deux processus ne peuvent être en même temps en section critique.
2. Aucune hypothèse ne doit être faite sur les vitesses relatives des pro-
cessus et sur le nombre de processeurs.
3. Aucun processus suspendu en dehors d’une section critique ne doit
bloquer les autres processus.
4. Aucun processus ne doit attendre trop longtemps avant d’entrer en
section critique.

https://sites.uclouvain.be/SystInfo/notes/Theorie/Threads/threads.html

https://sites.uclouvain.be/SystInfo/notes/Theorie/Threads/spinlocks.html

## Créer un thread

On peut créer un nouveau thread depuis n’importe quel thread du programme avec la fonction `pthread_create`, dont le prototype est le suivant :

```c
int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);
```

Examinons chaque argument qu’on doit lui fournir :

- **thread** : un pointeur vers une variable de type `pthread_t` pour stocker l’identifiant du thread qu’on va créer.
- **attr** : un argument qui permet de changer les attributs par défaut du nouveau thread lors de sa création. Ceci va au-delà de la portée de cet article, et en général il suffira d’indiquer `NULL` ici.
- **start_routine** : la fonction par laquelle le thread commence son exécution. 

Cette fonction doit avoir pour prototype :
  
  ```c
  void *nom_de_fonction_au_choix(void *arg);
  ```
  
  Lorsque le thread arrive à la fin de cette fonction, il aura terminé toutes ses tâches.
- **arg** : le pointeur vers un argument à transmettre à la fonction `start_routine` du thread. Si l’on souhaite passer plusieurs paramètres à cette fonction, il n’y a pas d’autre choix que de lui renseigner ici un pointeur vers une structure de données.
