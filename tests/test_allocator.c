#include <stdlib.h>
#include <stdio.h>
 
#include <my_allocator.h>

 
void testAlloc()
{
   // MALLOC
   int* a = malloc(sizeof(int));
   *a = 10;
   printf("(malloc) a = %d\n", *a);
 
   afficherMetaData();
}
 
int main()
{
   registerAllocatorHook();
 
   printf("testAlloc()...\n");
   testAlloc();
 
   return 0;
}
