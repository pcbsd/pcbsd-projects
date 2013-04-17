#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   int **my_pointers;
   int index = 0, count;
   int max_allocate = 2048;

   if (argc > 1)
      sscanf(argv[1], "%d", &max_allocate);

   my_pointers = (int **) calloc(2048, sizeof(void *));
   while (index < max_allocate)
   {
      my_pointers[index] = (int *) calloc(1024 * 1024, sizeof(int));
      for (count = 0; count < (1024 * 1024); count++)
         my_pointers[index][count] = rand();
      index++;
      printf("Allocated %d blocks.\n", index);
      if (! (index % 10) )
         sleep(1);
   }

   while (index > 0)
   {
      index--;
      free(my_pointers[index]);
      printf("%d blocks remaining.\n", index);
      if (! (index % 10) )
         sleep(1);
   }
   free(my_pointers);
   return 0;
}

