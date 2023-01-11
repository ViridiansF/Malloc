#include <stdlib.h>
#include <stdio.h>
#include "../include/my_allocator.h"
 
#define USE_MY_ALLOCATOR
 
#ifdef USE_MY_ALLOCATOR
#define ALLOC my_malloc
#define REALLOC my_realloc
#define CALLOC my_calloc
#define FREE my_free
#else
#define ALLOC malloc
#define REALLOC realloc
#define CALLOC calloc
#define FREE free
#endif
 


// Les tests suivants emplois un affichage de la liste chainée des metaDatas avec un code couleur

// Bleu : indique une metaData
// Rouge : indique une plage de données occupée
// Vert : indique une plage de données libre



void testSplit()
{

   printf("==============================================================================================================\n");
   printf("Split\n");
   
   int* a = ALLOC(sizeof(int));
   *a=10;
   int* b = ALLOC(100*sizeof(int));
   b[0]=10;
   int* c = ALLOC(sizeof(int));
   *c=10;
   int* d = ALLOC(sizeof(int));
   *d=10;

   FREE(b);

   afficherMetaData();

   int* e = ALLOC(sizeof(int)); 
   *e=10;
   printf("\n(malloc) e = %d\n", *e);

   afficherMetaData();
   printf("==============================================================================================================\n");

   FREE(a);
   FREE(b);
   FREE(c);
   FREE(d);
   FREE(e);

}
 
void testFusion()
{
 
   int* a = ALLOC(sizeof(int));   
   int* b = ALLOC(sizeof(int));
   int* c = ALLOC(sizeof(int));
   int* d = ALLOC(sizeof(int));
   int* e = ALLOC(sizeof(int));

   printf("==============================================================================================================\n");
   printf("Test Fusion\n");
   afficherMetaData();


   printf("Free metaData 1\n");
   printf("Free metaData 3\n");
   FREE(b);
   FREE(d);

   afficherMetaData();

   printf("Free metaData 2\n");
   FREE(c);

   // Voici l'ensemble de la taille libérée par le free
   int x = 3*(32+4);

   // On conserve cependant une metaData;
   x-=32;

   printf("\nLa taille du bloc libéré devrait être égale à 3*32 + 3*sizeof(int) - 32 = %d\n",x);

   afficherMetaData();
   printf("==============================================================================================================\n");

   printf("a=%p\n",a);
   printf("b=%p\n",b);
   printf("c=%p\n",c);
   printf("d=%p\n",d);
   printf("e=%p\n",e);


   FREE(a);
   FREE(b);
   FREE(c);
   FREE(d);
   FREE(e);

}

void testAlloc()
{

   printf("==============================================================================================================\n");
   printf("Allocation du premier bloc de données\n");
   
   int* a = ALLOC(sizeof(int));
   *a = 10;
   printf("(malloc) a = %d\n", *a);

   afficherMetaData();

   printf("\n==============================================================================================================\n");
   printf("Allocation d'un bloc de donnés autre que le premier\n");

   int* b = ALLOC(100 * sizeof(int));
   b[78] = 20;
   printf("(malloc) b[78] = %d\n", b[78]);

   afficherMetaData();


   FREE(a);
   FREE(b);

}


void testCalloc()
{
   printf("\n==============================================================================================================\n");
   printf("Allocation de deux blocs de données\n");


   int* a = ALLOC(5*sizeof(int));

   int* b = ALLOC(5*sizeof(int));


   for(int i=0;i<5;i++)
   {
      a[i]=456;
      printf("a[%d] = %d\n",i,a[i]);
   }

   printf("\nFree metadata 0\n");


   FREE(a);

   afficherMetaData();

   printf("\nCalloc de la même taille, la fonction écrit dans la plage Data 0\n\n");

   a = CALLOC(5,sizeof(int));


   printf("On constate l'initialisation à 0\n");

   for(int i=0;i<5;i++)
   {
      printf("a[%d] = %d\n",i,a[i]);
   }

   afficherMetaData();
   printf("==============================================================================================================\n");


   FREE(a);
   FREE(b);


}

void testRealloc()
{

   printf("==============================================================================================================\n");
   printf("Réallocation d'une grande variable en une petite variable\n");
   

   int* a = ALLOC(2 * sizeof(int));
   printf("a[0]=456\n");
   a[0]=456;
   int* b = ALLOC(100 * sizeof(int));
   printf("b[0]=125\n");
   b[0] = 125;

   int* c = ALLOC(2 * sizeof(int));

   afficherMetaData();

   b = REALLOC(b, 5 * sizeof(int));
   printf("(realloc) de b : 100*int => 5*int\n");

   printf("Preuve que b est définie : b[0] = %d\n", b[0]);

   // x représente la taille théorique du bloc libre après la reallocation
   int x = 100*sizeof(int); //taille initiale de b
   
   x-=5*sizeof(int); // on enlève la nouvelle taille de b
   
   x-=32; // on enlève la taille d'une metaData

   printf("\nLa taille du bloc libre devrait être de 100*sizeof(int) - 5*sizeof(int) - 32 = %d\n",x);
   afficherMetaData();



   printf("==============================================================================================================\n");
   printf("Réallocation d'une variable grande variable avec extension de la pile\n");

   a = REALLOC(a, 200 * sizeof(int));
   printf("(realloc) de a : 2*int => 200*int\n");

   printf("a[100]=789\n");
   a[100]=789;

   afficherMetaData();

   printf("Preuve que a a été copié a[0] = %d\n", a[0]);

   printf("Preuve que a a été aggrandie a[100] = %d\n", a[100]);

   printf("==============================================================================================================\n");

   
   FREE(a);
   FREE(b);
   FREE(c);
}

void testFree()
{
   printf("\n==============================================================================================================\n");

   printf("Free dernier bloc d'une liste avec une seule metaData\n");

   int* a = ALLOC(2 * sizeof(int));
   
   afficherMetaData();

   printf("\nmy_free(metaData 0)\n");
   my_free(a);
   afficherMetaData();


   printf("\n==============================================================================================================\n");
   printf("Free bloc au milieu de la pile\n");


   a = ALLOC(2 * sizeof(int));

   int* b = ALLOC(2 * sizeof(int));
   int* c = ALLOC(2 * sizeof(int));
   afficherMetaData();
   printf("\nmy_free(metaData 0)\n");
   FREE(a);
   afficherMetaData();

   printf("\n==============================================================================================================\n");

   printf("Free dernier bloc d'une liste avec plusieurs metaDatas\n");

   afficherMetaData();

   printf("\nmy_free(metaData 2)\n");
   my_free(c);
   afficherMetaData();


   FREE(a);
   FREE(b);
   FREE(c);
}


int main()
{
   //registerAllocatorHook();
   printf("ALLOCATION\n");
   testAlloc();
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\nFREE\n");
   testFree();
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\nSPLIT\n");
   testSplit();
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\nFUSION\n");
   testFusion();
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\nCALLOCATION\n");
   testCalloc();
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\nREALLOCATION\n");
   testRealloc();

   printf("\n");

   return 0;
}


