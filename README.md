# Malloc project

## Présentation 
Projet Malloc pour ISART 2022.

L'objectif était de recoder les fonctions malloc, free, calloc et realloc.

___
## Fonctions:

## my_malloc : 
    void* my_malloc(size_t size);

Fonction d'allocation d'espace mémoire.
Parcours la pile à la recherche d'espace mémoire suffisant pour écrire la donnée.

Si elle en trouve, la fonction crée une metaData qu'elle ajoute à la liste chainée des metaDatas (qui donnent un aperçu de l'état de la pile).

Si l'espace trouvé est encore suffisamment grand après l'allocation, la fonction crée une nouvelle zone mémoire marquée comme libre après la zone allouée avec l'espace restant. Elle ajoute également une metaData décrivant cet espace à la liste chainée.

Si elle n'en trouve pas elle repousse le break et étend la pile, ajoutant une nouvelle metaData en bout de liste chainée.

### Paramètre:
- size : la taille de l'espace mémoire que l'utilisateur veut allouer. (type size_t)

### Type de retour:
- le pointeur sur la première case mémoire de la zone mémoire allouée par la fonction. (type void*)

### Exemple d'utilisation:
Pour allouer un tableau de 5 ints.

    int* a = my_malloc(5*sizeof(int));

### Dépendance:
Aucune
___


## my_free : 
    void my_free(void* ptr);

Fonction de libération d'espace mémoire.
Parcours la liste chainée des metaDatas et change le paramètre blocIsFree de false à true, permettant aux futurs allocations de se servir de cet espace.

Si le bloc de données à libérer se trouve au bout de la pile, la fonction déplace le break, supprimant ainsi la donnée et la metaData associée de pile.

### Paramètre:
- ptr : le pointeur sur la plage de donnée devant être libérée. (type void*)

### Type de retour:
- rien. (type void)

### Exemple d'utilisation:
Pour libérer un tableau de 5 ints.

    int* a = my_malloc(5*sizeof(int));
    my_free(a);

### Dépendance:
Aucune
___

## my_calloc : 
    void* my_calloc(size_t nb, size_t size);

Fonction d'allocation d'espace mémoire avec initialisation de chaque case mémoire à 0.

### Paramètres:
- size : la taille du type devant être stocké dans l'espace mémoire. (type size_t)
- nb : quantité de variables de taille "size" devant être stockées.

### Type de retour:
- le pointeur sur la première case mémoire de la zone mémoire allouée par la fonction. (type void*)

### Exemple d'utilisation:
Pour un tableau de 5 ints.

    int* a = my_calloc(5,sizeof(int));

### Dépendance:
- my_malloc

___


## my_realloc : 
    void* my_realloc(void* ptr, size_t size);

Fonction de réallocation de l'espace mémoire.

Permet par exemple d'agrandir un tableau ou au contraire de le rétrécir.

Dans le cas d'une réduction de taille, la fonction fait appel à my_free. Cette opération peut éventuellement créer une nouvelle plage de données et sa metaData si l'espace libéré est suffisament grand.

Dans le cas d'une augmentation de taille, la fonction commence par vérifier s'il y a de l'espace libre dans la pile à côté de la variable. S'il y en a assez elle ajoute simplement cet espace à l'espace mémoire de la variable. 

Sinon, la fonction repousse le break et copie le contenu de la variable dans le nouveau bloc ainsi créé. Elle rend ensuite libre le bloc dans lequel était contenu la variable.

### Paramètres:
- ptr : pointeur sur la donnée donc l'espace mémoire doit être réalloué (type void*)
- size : la taille de l'espace mémoire que l'utilisateur veut allouer. Ex: Pour un tableau de 5 int on écrirait : 5 * sizeof(int)  (type size_t)

### Type de retour:
- le pointeur sur la première case mémoire de la zone mémoire allouée par la fonction. (type void*)


### Exemple d'utilisation:
Pour un tableau de 5 ints réalloué en un tableau de 2 ints.

    int* a = my_malloc(5*sizeof(int));
    a = my_realloc(a,2*sizeof(int));

### Dépendances:
- my_free
- my_malloc

___

## Structures:

## metaData:

    typedef struct metaData
    {
        size_t allocatedMemory;
        void* ptrData;
        bool blocIsFree; 
        struct metaData* next;
    } metaData;

Structure liée à un bloc de données contenant diverses informations sur ce dernier.

## Paramètres:

- allocatedMemory : taille de l'espace mémoire alloué à ce bloc de données. (type : size_t)
- ptrData : pointeur sur la première case mémoire du bloc de données. (type : void*)
- blocIsFree : booléen indiquant si le bloc contient des données importantes devant être conservées (false) ou anciennes pouvant être écrasée (true). (type : bool)
- next : pointeur sur la metaData suivante dans la liste chainée des metaDatas. (type : metaData*)
___
## Liste chainée de metaDatas

Les metaDatas décrivant la pile de my_allocator.c sont structurées dans une liste chainée dont le premier noeud est une variable globale.
___

## Librairie:

## colors.h:

Librairie simplifiant l'usage des changements de couleur dans les printf.

___