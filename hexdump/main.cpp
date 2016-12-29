#include <stdio.h>
#include <string.h>

void hexDump(unsigned char const * const buffer, unsigned int bufferLen, FILE* fd = stdout)
{
  unsigned int i;
  for(i = 0; i < bufferLen; i++)
  {
    fprintf(fd, "%02x", buffer[i]);
    if (i % 16 == 15)
    {
      fprintf(fd, "\n");
    }
    else if (i % 16 == 7)
    {
      fprintf(fd, "  ");
    }
    else
    {
      fprintf(fd, " ");
    }
  }

  if (i % 16 != 0)
  {
    fprintf(fd, "\n");
  }
}



int main()
{
  const char* alphabet = "abcdefghijklmnopqrstuvwxyz";

  printf("Alphabet to stdout:\n");
  hexDump( (unsigned char*) alphabet, strlen(alphabet));

  printf("Numbers to stderr:\n");
  unsigned char nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  hexDump(nums, sizeof(nums) / sizeof(unsigned char), stderr);
 
  return 0;
}


