#include <stdio.h>
#include <string.h>

void hexDump(unsigned char const * const buffer, unsigned int bufferLen, FILE* fd = stdout, bool asciiToo = false)
{
  char asciiDump[0x11];
  char const * const ASCII_BLANK = "                ";
  strcpy(asciiDump, ASCII_BLANK);

   unsigned int i;
  for(i = 0; i < bufferLen; i++)
  {
    if ( (buffer[i] >= ' ') && (buffer[i] <= '~') )
    {
       // Printable in ASCII
       asciiDump[i % 16] = buffer[i];
    }
    else
    {
       // Not printable ASCII
       asciiDump[i % 16] = '.';
    }

     fprintf(fd, "%02x", buffer[i]);
    if (i % 16 == 15)
    {
      if (asciiToo)
      {
         fprintf(fd, "  |%s|\n", asciiDump);
         strcpy(asciiDump, ASCII_BLANK);
      }
      else
      {
         fprintf(fd, "\n");
      }
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
     if (asciiToo)
     {
        // Need to print out padding before ascii
        for (int paddingByte = ( i % 16); paddingByte < 16; paddingByte++)
        {
           fprintf(fd, "   ");
        }

        fprintf(fd, " |%s|\n", asciiDump);
        strcpy(asciiDump, ASCII_BLANK);
     }
     else
     {
        fprintf(fd, "\n");
     }
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

  printf("Alphabet to stdout with ASCII:\n");
  hexDump( (unsigned char*) alphabet, strlen(alphabet), stdout, true);

  printf("Numbers to stderr with ASCII:\n");
  hexDump(nums, sizeof(nums) / sizeof(unsigned char), stderr, true);


  return 0;
}


