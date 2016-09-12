#include "simple_patcher.h"

#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <string.h>

void printSectionInfo(FILE* f,
                       struct Elf32_Ehdr const & elfHeader,
                       int i,
                       std::map<uint32_t, std::string> const & sectionNames)
{
   if (i > elfHeader.e_shnum)
   {
      printf("Section number %d is invalid for this ELF\n", i);
      return;
   }

   if (sizeof(struct Elf32_SectionHeader) != elfHeader.e_shentsize)
   {
      if (sizeof(struct Elf32_SectionHeader) < elfHeader.e_shentsize)
      {
         printf("Error: Size of section header (%d bytes) is smaller than the struct we need to load it into (%zu bytes)!",
                elfHeader.e_shentsize, sizeof(struct Elf32_SectionHeader));
         return;
      }

      printf("Size of section header (%d bytes) is different than the struct we need to load it into (%zu bytes)!",
             elfHeader.e_shentsize, sizeof(struct Elf32_SectionHeader));
   }

   struct Elf32_SectionHeader sectionData;

   if (!readFile(f,
                 elfHeader.e_shoff + i * elfHeader.e_shentsize,
                 sizeof(struct Elf32_SectionHeader),
                 (uint8_t*) &sectionData))
   {
      printf("Error reading section %d header\n", i);
   }

   std::string secName;
   if (sectionNames.find(sectionData.sh_name) != sectionNames.end())
   {
      secName = sectionNames.at(sectionData.sh_name);
   }


   // Section, Name, VMA, Size, Offset
   printf("%2d %20s  0x%08x  0x%08x  0x%08x\n",
          i,
          secName.c_str(),
          sectionData.sh_addr,
          sectionData.sh_size,
          sectionData.sh_offset);

}

void printElfInfo(struct Elf32_Ehdr const & elfHeader, FILE* f)
{
   char const * elfType;
   switch(elfHeader.e_type)
   {
      case 0:
         elfType = "None";
         break;

      case 1:
         elfType = "Relocatable File";
         break;

      case 2:
         elfType = "Executable File";
         break;

      case 3:
         elfType = "Shared Object File";
         break;

      case 4:
         elfType = "Core File";
         break;

      default:
         elfType = "Unknown or Processor Specifiec";
   }

   printf("ELF Type = %s\n", elfType);

   std::map<int, char const *> machineMap;
   machineMap[3]   = "Intel 386";
   machineMap[20]  = "PowerPC";
   machineMap[21]  = "PowerPC 64";
   machineMap[40]  = "ARM";
   machineMap[42]  = "Hitachi SH";
   machineMap[50]  = "Intel IA-64";
   machineMap[62]  = "AMD x86-64";
   machineMap[83]  = "Atmel AVR";
   machineMap[105] = "TI MSP 430";
   machineMap[183] = "ARM AArch64";
   machineMap[190] = "NVidia CUDA";

   if (machineMap.find(elfHeader.e_machine) == machineMap.end())
   {
      printf("Unknown Machine Type (%d)\n", elfHeader.e_machine);
   }
   else
   {
      printf("Machine Type = %s\n", machineMap[elfHeader.e_machine]);
   }

   if (elfHeader.e_shnum <= 0)
   {
      printf("There are no sections in this elf file\n");
      return;
   }

   //printf("Section Strings = 0x%08x\n", elfHeader.e_shstrndx);

   std::map<uint32_t, std::string> secNames = getSectionNames(f, elfHeader);

   // Section, Name, VMA, Size, Offset
   printf("Sec# %18s    VirtAddr        Size  FileOffset\n",
          "Section Name");
   for(int i = 0; i < elfHeader.e_shnum; i++)
   {
      printSectionInfo(f, elfHeader, i, secNames);
   }
}

int main(int argc, char** argv)
{
   if (argc != 4)
   {
      printf("Usage: %s elf_file VMA_Start NumBytes\n", argv[0]);
      printf(" Then write in the patch bytes to stdin\n");
      printf(" All bytes after the patch will be NOPed until NumBytes reached\n");
      return 1;
   }

   FILE* fileToPatch = fopen(argv[1], "rw");

   if (fileToPatch == NULL)
   {
      printf("Couldn't open file %s to patch it\n", argv[1]);
      return 1;
   }

   struct Elf32_Ehdr elfHeader;

   fread(&elfHeader, sizeof(Elf32_Ehdr), 1, fileToPatch);

   printElfInfo(elfHeader, fileToPatch);

   /* // ELF Header Debug
   printf("Type                             = 0x%08x\n", elfHeader.e_type);
   printf("Machine                          = 0x%08x\n", elfHeader.e_machine);
   printf("Version                          = 0x%08x\n", elfHeader.e_version);
   printf("Entry                            = 0x%08x\n", elfHeader.e_entry);
   printf("Program Header Offset            = 0x%08x\n", elfHeader.e_phoff);
   printf("Section Header Offset            = 0x%08x\n", elfHeader.e_shoff);
   printf("Flags                            = 0x%08x\n", elfHeader.e_flags);
   printf("ELF Header Size                  = 0x%08x\n", elfHeader.e_ehsize);
   printf("Program Header Table Entry Size  = 0x%08x\n", elfHeader.e_phentsize);
   printf("Number of ProgHdr Table Entries  = 0x%08x\n", elfHeader.e_phnum);
   printf("Section Header Size              = 0x%08x\n", elfHeader.e_shentsize);
   printf("Number of Section Header Entries = 0x%08x\n", elfHeader.e_shnum);
   printf("String Index                     = 0x%08x\n", elfHeader.e_shstrndx);
   */



   fclose(fileToPatch);

   return 0;
}

bool readFile(FILE* f,
              uint32_t offset,
              uint32_t length,
              uint8_t* buffer)
{
   if (0 != fseek(f, offset, SEEK_SET))
   {
      printf("Error seeking in the file to offset 0x%08x\n", offset);
      return false;
   }

   if ( 1 != fread(buffer, length, 1, f))
   {
      printf("Error reading data into file.  Offset=0x%08x, Length=%d\n", offset, length);
      return false;
   }

   return true;
}

bool writeFile(FILE* f,
               uint32_t offset,
               uint32_t length,
               uint8_t* buffer)
{
   return false;
}

std::string readSectionName(FILE* f,
                            struct Elf32_Ehdr const & elfHeader,
                            int i)
{
   if (0 != fseek(f, elfHeader.e_shstrndx + i, SEEK_SET))
   {
      printf("Error seeking in the file to offset 0x%08x (string index)\n", elfHeader.e_shstrndx);
      return "";
   }

   std::string retVal;
   char data = ' ';
   while(data != 0)
   {
      if (1 != fread(&data, 1, 1, f))
      {
         printf("Error reading section name for section index %d\n", i);
         return retVal;
      }

      if (data != 0)
         retVal += (char) data;
   }

   return retVal;
}

std::map<uint32_t, std::string> getSectionNames(FILE* f,
                                         struct Elf32_Ehdr const & elfHeader)
{
   struct Elf32_SectionHeader stringSectionHdr;

   uint32_t stringSectionLocation = elfHeader.e_shoff + elfHeader.e_shstrndx * elfHeader.e_shentsize;
   // printf("String section @ 0x%08x\n", stringSectionLocation);

   if (!readFile(f, stringSectionLocation, sizeof(struct Elf32_SectionHeader), (uint8_t*) &stringSectionHdr))
   {
      // printf("Failed to read the header names section of the ELF\n");
      return std::map<uint32_t, std::string>();
   }

   int numSections = elfHeader.e_shnum;

   uint32_t nameDataLocation = stringSectionHdr.sh_offset;
   uint32_t nameDataSize     = stringSectionHdr.sh_size;

   char* nameData = new char[stringSectionHdr.sh_size];
   uint32_t strIndexOffset = 0;
   readFile(f, nameDataLocation, nameDataSize, (uint8_t*) nameData);

   // printf("Going to read section strings at 0x%08x\n", nameDataLocation);

   std::map<uint32_t, std::string> sectionNames;
   for(int i = 0; i < numSections; i++)
   {
      std::string secName(nameData + strIndexOffset);
      sectionNames[strIndexOffset]=secName;
      strIndexOffset += secName.length() + 1;

      //printf("Just added section name %s to the list of sections\n", secName.c_str());
   }

   delete[] nameData;
   return sectionNames;
}
