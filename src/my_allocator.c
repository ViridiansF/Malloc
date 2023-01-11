#include "my_allocator.h"

static metaData* firstNode = NULL;


void printMetaData(metaData* node, int i)
{
        printf(BLUB "Metadata %d Adresse : %p Data adress : %p Next : %p (Size : %zu)" reset ,i,node,node->ptrData, node->next, sizeof(metaData));
        printf("\n");
        if(node->blocIsFree)
            printf(GRNB "DATA %d (Size : %zu)\n" reset,i,node->allocatedMemory);
        else
            printf(REDB "DATA %d (Size : %zu)\n" reset,i,node->allocatedMemory);
}

void afficherMetaData()
{

    printf("\nListe chainée des metaDatas :\n\n");
    metaData* node = firstNode;
    int i=0;
    if(firstNode==NULL)
        printf(BLUB "La liste chainée de metaData est vide\n" reset);
    else
    {
        while(node->next!=NULL)
        {
            printMetaData(node, i);
            node=node->next;
            i++;
        }
        printMetaData(node,i);
        printf(reset);
        printf("\n\n");
    }
}


void* my_malloc(size_t size)
{
    while(size%8!=0)
        size++;

    if(firstNode==NULL) // On initialise le premier jeu de metaData
    {
        //printf("Initialisation du premier jeu de metaData\n");
        firstNode = sbrk(sizeof(metaData));
        firstNode->allocatedMemory=size; 
        firstNode->blocIsFree=true ; 
        firstNode->next=NULL; 
        firstNode->ptrData=firstNode+sizeof(metaData);
    }

    metaData* node = firstNode;

    if(size>0)
    {
        void* ptrAllocatedMemory;
    
        // On cherche un bloc libre et assez grand
        while(node!=NULL && (node->blocIsFree==false || node->allocatedMemory<size)) 
        { 
            node = node->next;
        }

        if(node==NULL) // Si on en trouve pas alors on repousse le BREAK
        {
            //printf("Aucun node n'est disponible\nYOU NEED TO PUSH BREAK !\n");

            metaData* newMetaData = sbrk(sizeof(metaData));
            newMetaData->allocatedMemory=size;
            newMetaData->next=NULL;
            newMetaData->blocIsFree=false;

            node=firstNode;
            while(node->next!=NULL)
                node = node->next;

            node->next = newMetaData;
            
            ptrAllocatedMemory = sbrk(size);

            newMetaData->ptrData=ptrAllocatedMemory;


        }
        else // On a trouvé un bloc dans lequel on peut écrire
        {
            //printf("YOU CAN WRITE HERE !\n");

            // On crée une nouvelle metadata pour décrire l'espace restant après l'allocation
            // S'il reste suffisemment de place libre entre cette metadata et la suivante (ou le breakpoint)
            if(node->allocatedMemory >= sizeof(metaData)+1) 
            {

                //printf("Il y a split\n");

                // La taille est égale à l'espace libre total moins celui sur lequel on va écrire - la taille du metadata à écrire
                size_t dataSize = node->allocatedMemory - size - sizeof(metaData); 
                metaData* newMetaData = node->ptrData+size;
                newMetaData->allocatedMemory=dataSize;
                newMetaData->next=node->next;
                newMetaData->blocIsFree=true;
                newMetaData->ptrData=newMetaData+sizeof(metaData);

                node->next = newMetaData;

            }

            //Dans tous les cas le premier bloc de donnée n'est plus disponible en écriture et sa taille a changé
            node->blocIsFree=false;
            node->allocatedMemory = size;
            ptrAllocatedMemory=node+sizeof(metaData);
       
        }

        return ptrAllocatedMemory;
    }
    return NULL;
}

void my_free(void* ptr)
{

    if(firstNode==NULL)
        printf("Error : La liste de metaData est vide");
    else
    {
        metaData* node=firstNode;

        while(node->ptrData!=ptr && node->next!=NULL)
        {
            node=node->next;
        }

        // S'il s'agit du dernier bloc de donnée on peut reculer le break sans risque
        // On modifie supprime également la première metaData
        if(node->next==NULL) 
        {           
            //printf("On recule le break car il s'agit du dernier bloc de donnée\n");
            brk(ptr-sizeof(metaData));

            //  S'il s'agit du seul bloc de données on supprime tout
            if(node==firstNode)
            {
                firstNode=NULL;
                brk(ptr-sizeof(metaData));
            }
            //  Sinon on doit reculer le break et modifier la metaData précédente
            else
            {
                metaData* meta = firstNode;

                while(meta->next!=NULL && meta->next!=node)
                    meta=meta->next;
                meta->next=NULL;
            }
            brk(ptr-sizeof(metaData));
        }

        // Sinon free rend juste éditable l'ancien bloc de données
        else                            
        {
            //printf("Le bloc est au milieu de la liste de metaData, on le rend donc éditable");

            //Si le bloc précédent est aussi libre on le fusionne avec le bloc et éventuellement avec le suivant s'il est libre
            metaData* previousMetaData = firstNode;
            
            while(previousMetaData->next!=node && previousMetaData->next!=NULL)
                previousMetaData = previousMetaData->next;
            
            metaData* nextMetaData = node->next;

            if(nextMetaData->blocIsFree)
            {
                //printf("il y a fusion de %p et le bloc précédent %p",node->next,previousMetaData);

                node->allocatedMemory = node->allocatedMemory + nextMetaData->allocatedMemory + sizeof(metaData);
                node->next=nextMetaData->next;
            }

            if(previousMetaData->blocIsFree)
            {
                //printf("il y a fusion de %p et le bloc suivant %p",node->next,nextMetaData);

                // Le node libre précédent récupère l'espace du node appelant ainsi que l'espace de sa data 
                previousMetaData->allocatedMemory= previousMetaData->allocatedMemory + node->allocatedMemory + sizeof(metaData);
                // Le node libre précédent récupère comme node suivant le node qui suit le node appelant
                previousMetaData->next=previousMetaData->next->next;
            }
            
            //printf("On libère le bloc\n");
            node->blocIsFree=true;
        }
    }
}

void* my_calloc(size_t nb, size_t size)
{   
    // On appelle malloc
    int* ptr = my_malloc(nb*size);
    int* oldPtr=ptr;

    // On initialise l'espace à 0
    for(size_t i=0;i<nb;i++)
        ptr[i]=0;
    
    
    return oldPtr;

}


void* my_realloc(void* ptr, size_t size)
{
    
    // La metaData de la portion de la Data qu'on veut reallouer
    metaData* firstMetaData = ptr-sizeof(metaData);

    metaData* node = firstMetaData;
    metaData* newMetaData;


    // Si le bloc de Data suivant est libre on récupère cet espace et on passe à la metaData suivante
    while(node->next!=NULL && node->next->blocIsFree)
    {
        node = node->next;
        firstMetaData->allocatedMemory+=sizeof(metaData) + node->allocatedMemory;
    }


    // Si on a récupéré suffisemment de place on peut écrire dedans la data
    if(firstMetaData->allocatedMemory>=size)
    {
        //printf("Place requise trouvée !\n");

        // Si on a la place d'écrire une nouvelle metaData dans la mémoire après la réallocation on le fait
        if(firstMetaData->allocatedMemory-size > sizeof(metaData)+1)
        {
            newMetaData = firstMetaData + sizeof(metaData)+size;
            newMetaData->next = firstMetaData->next;
            newMetaData->blocIsFree=true;
            newMetaData->allocatedMemory=firstMetaData->allocatedMemory-size-sizeof(metaData);
            newMetaData->ptrData+=sizeof(metaData);
            firstMetaData->next = newMetaData;
        }

        // Sinon tant pis
        firstMetaData->allocatedMemory=size;
        return firstMetaData->ptrData;
    }

    // Dans le cas contraire il est nécessaire d'étendre la pile
    else
    {
        //printf("Place requise introuvable, extension de la pile\n");

        // On copie alors la totalité de la plage de Data au bout de la pile en ajoutant l'espace pour que la réallocation soit un succès
        char* allocatedMemory = my_malloc(size);
        char* ptrEnChar = ptr;
        for(size_t i=0;i<size;i++)
            allocatedMemory[i]=ptrEnChar[i];   
        

        // Ensuite on libère le bloc de Data qu'on vient de copier
        my_free(ptr);


        return allocatedMemory;
    }
}

#include<malloc.h>
void* malloc_hook(size_t size, const void* caller)            { return my_malloc(size); }
void* realloc_hook(void* ptr, size_t size, const void* caller){ return my_realloc(ptr, size); }
void  free_hook(void* ptr, const void* caller)                { return my_free(ptr); }


void registerAllocatorHook(void)
{
    __malloc_hook = malloc_hook;
    __realloc_hook = realloc_hook;
    __free_hook = free_hook;
}