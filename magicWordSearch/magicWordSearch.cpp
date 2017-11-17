#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <vector>

#include "../hexdump/hexdump.h"

void dumpFileData(int fd, long offset, long prefixLength, long dumpLength, long fileLength)
{
   long seekAddress = offset - prefixLength;
   if (offset < prefixLength)
   {
      // Dump from beginning of the file
      printf("Prefix of dump at %lx (0x%lx) is cut off by the start of the file\n", offset, offset);
      seekAddress = 0;

      prefixLength = offset;
   }

   if (seekAddress != lseek(fd, seekAddress, SEEK_SET))
   {
      printf("Seek failed for dump at %ld (0x%lx)\n", offset, offset);
      return;
   }

   if ( (offset + dumpLength) > fileLength)
   {
      printf("Dump at %lx (0x%lx) cut off by the end of the file\n", offset, offset);
      dumpLength = fileLength - offset;
   }

   long bufSize = prefixLength + dumpLength;
   char* buf = (char*) malloc(bufSize);

   long bytesRead = 0;
   while(bytesRead < bufSize)
   {
      int readLen = read(fd, buf + bytesRead, bufSize - bytesRead);

      if (readLen <= 0)
      {
         printf("Error reading the file at offset %ld (0x%lx)\n", offset, offset);
         free(buf);
         return;
      }

      bytesRead += readLen;
   }

   printf("\nMagic word at %ld (0x%lx):\n", offset, offset);
   hexDump( (unsigned char*) buf, bufSize);

   free(buf);
}

int main(int argc, char** argv)
{
   if ( (argc < 4) || (argc > 6) )
   {
      printf("Usage error, no filename to walk provided\n");
      printf("  Usage: %s filename.bin [-h] magicBytes numBytesToDump [prefixLength]\n\n", argv[0]);
      printf("  filename.bin is the file to search in\n");
      printf("  -h indicates that the magic_bytes are hexadecimal (ex: -h 89abcd\n");
      printf("  magicBytes is the string to search for (or hex if -h specified)\n");
      printf("  numBytesToDump is the number of bytes after beginning of magic word to dump\n");
      printf("  prefixLength (optional) number of bytes before the magic word to dump\n");
      return 1;
   }

   // Parse and verify the arguements

   char* filename = argv[1];

   char* magicString = argv[2];
   bool optionalHexArgPresent = false;
   int magicStringLength;
   if(strcmp(argv[2], "-h") == 0)
   {
      optionalHexArgPresent = true;

      // Make sure there is an even number of characters in the hex string
      if (strlen(argv[3]) % 2 == 1)
      {
         printf("Hex string for the magic bytes must have an even number of characters!: %s has %lu characters\n",
                argv[3], strlen(argv[3]));
         return 1;
      }

      magicStringLength = strlen(argv[3]) / 2;

      magicString = (char*) malloc(magicStringLength + 1);
      memset(magicString, 0, magicStringLength + 1);
      for(int i = 0; i < magicStringLength; i++)
      {
         char smallBuf[3];
         smallBuf[0] = argv[3][i * 2];
         smallBuf[1] = argv[3][i * 2 + 1];
         smallBuf[2] = 0;

         long int val = strtol(smallBuf, 0, 16);
         magicString[i] = (char) val;
      }

   }
   else
   {
      // This is a vanilla ASCII magic string
      magicStringLength = strlen(magicString);
   }

   int dumpLenArgIndex = 3;
   int prefixArgIndex = 4;
   if (optionalHexArgPresent)
   {
      dumpLenArgIndex++;
      prefixArgIndex++;
   }

   if (dumpLenArgIndex >= argc)
   {
      printf("Dump length arguement missing!\n");
      return 1;
   }
   int dumpLength = atoi(argv[dumpLenArgIndex]);

   int prefixLength = 0;
   if (prefixArgIndex < argc)
   {
      prefixLength = atoi(argv[prefixArgIndex]);
   }

   // Print out all the variables / options enabled

   printf("Filename  : %s\n", filename);
   printf("Hex mode  : %s\n", optionalHexArgPresent ? "Yes" : "No");

   if (!optionalHexArgPresent)
   {
      printf("Magic    : %s\n", magicString);
   }

   printf("Dump Len  : %d\n", dumpLength);
   printf("Prefix Len: %d\n", prefixLength);

   // Now we will actually do stuff

   int fd = open(argv[1], O_RDONLY);

   if (fd <= 0)
   {
      printf("Failed to open file %s\n", argv[1]);
      return 1;
   }

   off_t fileSize = lseek(fd, 0, SEEK_END);

   if (fileSize == -1)
   {
      printf("Error seeking to the end of the file to determine file size\n");
      close(fd);
      return 1;
   }

   if (lseek(fd, 0, SEEK_SET) == -1)
   {
      printf("Error seeking back to the beginning of the file\n");
      close(fd);
      return 1;
   }

   long int bytesLeftToSearch = fileSize;

   printf("File size : %ld\n", bytesLeftToSearch);

   const int BUF_SIZE = 8192;
   char buf[BUF_SIZE];
   memset(buf, 0, BUF_SIZE);

   std::vector<long int> offsetsWithMagicWords;
   long int currentOffset = 0;
   int bufferOffset = 0;
   while(bytesLeftToSearch > 0)
   {
      long int bytesToRead = bytesLeftToSearch;
      if (bytesToRead > BUF_SIZE)
      {
         bytesToRead = BUF_SIZE;
      }

      bytesToRead -= bufferOffset;

      ssize_t bytesRead = 0;
      bytesRead = read(fd, buf + bufferOffset, bytesToRead);

      //printf("Bytes read = %ld while trying to read %ld bytes at offset 0x%lx, buffer offset = %d\n",
      //       bytesRead, bytesToRead, currentOffset, bufferOffset);

      //hexDump( (unsigned char*) buf, 16);

      if (bytesRead == -1)
      {
         printf("Error reading data from file: %s\n", strerror(errno));
         break;
      }

      if (bytesRead == 0)
      {
         printf("Bytes read = 0, end of file\n");
         break;
      }

      // Search for the magic string in the buffer (note, the string isn't necessarily null-terminated)
      for(int i = 0; i < BUF_SIZE - magicStringLength; i++)
      {
         bool matchFound = true;
         for(int magicByteOffset = 0; magicByteOffset < magicStringLength; magicByteOffset++)
         {
            if (buf[i + magicByteOffset] != magicString[magicByteOffset])
            {
               matchFound = false;
               break;
            }
         }

         if ( matchFound )
         {
            printf("Found a magic word at offset %ld (0x%lx)\n", i + currentOffset, i + currentOffset);
            offsetsWithMagicWords.push_back(i + currentOffset);

            // if (i < BUF_SIZE - 0xc0)
            // {
            //    printf("Hex preview:\n");
            //    hexDump((unsigned char*) (buf + i), 0xc0);
            // }
         }
      }


      int endOfBuffer = bufferOffset + bytesRead;
      if (bytesRead > magicStringLength)
      {
         // Copy the last bytes to the front of the buffer incase the match is right on the boundary
         memcpy(buf, buf + endOfBuffer - magicStringLength, magicStringLength);
         currentOffset += endOfBuffer - magicStringLength;
         bytesLeftToSearch -= endOfBuffer - magicStringLength;
         memset(buf + magicStringLength, 0, BUF_SIZE - magicStringLength);
         bufferOffset = magicStringLength;
      }
      else
      {
         printf("Weird case where we read less than 4 bytes at offset %ld\n", currentOffset);
         currentOffset += endOfBuffer;
         bytesLeftToSearch -= endOfBuffer;
         memset(buf, 0, BUF_SIZE);
         bufferOffset = 0;
      }
   }

   // Display all the matches

   for(std::vector<long int>::const_iterator it = offsetsWithMagicWords.begin();
       it != offsetsWithMagicWords.end();
       it++)
   {
      dumpFileData(fd, *it, prefixLength, dumpLength, fileSize);
   }

   close(fd);

   if (optionalHexArgPresent)
   {
      free(magicString);
   }

}
