#include <stdio.h>
#include <string.h>

#include "hexdump.h"

int main()
{
   const char* alphabet = "abcdefghijklmnopqrstuvwxyz";

   printf("Alphabet to stdout:\n");
   hexDump( (unsigned char*) alphabet, strlen(alphabet));

   printf("Numbers to stderr:\n");
   unsigned char nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   hexDump(nums, sizeof(nums) / sizeof(unsigned char), stderr);

   printf("Alphabet to stdout with ASCII:\n");
   hexDump( (unsigned char*) alphabet, strlen(alphabet), stdout, true);

   printf("Numbers to stderr with ASCII:\n");
   hexDump(nums, sizeof(nums) / sizeof(unsigned char), stderr, true);


   return 0;
}


