#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <string.h>
#include <map>

void printUsage()
{
   std::cerr << "Usage: cat hashes_to_sort.md5 | ./hashsort [-f|n|h|u] < hashes_to_sort.md5 | tee sorted.md5" << std::endl;
   std::cerr << "  -f : sort hashes by filename" << std::endl;
   std::cerr << "  -h : sort hashes by hash" << std::endl;
   std::cerr << "  -n : print out non-uniques (will sort by hash too)" << std::endl;
   std::cerr << "  -u : print out uniques only (will sort by hash too)" << std::endl;
   std::cerr << std::endl;
   std::cerr << "This application intended for sorting files created by md5sum.  It reads the hash file on stdin" << std::endl;
   std::cerr << "and writes the output on stdout" << std::endl;
}

bool sortByFilename(std::string const & a, std::string const & b)
{
   size_t endOfHashA = a.find_first_of(' ');
   size_t endOfHashB = b.find_first_of(' ');

   if ( (endOfHashA == std::string::npos) || (endOfHashB == std::string::npos))
   {
      std::cerr << "Couldn't find space between hash and filename:" << std::endl;
      std::cerr << "A: " << a << std::endl;
      std::cerr << "B: " << b << std::endl;
      throw std::exception();
   }

   return (a.substr(endOfHashA + 1) < b.substr(endOfHashB + 1));
}

void performMultiMapSort(std::vector<std::string> const & lines, std::string sortType)
{
   bool uniquesOnly = false;
   if (sortType == "-u")
   {
      uniquesOnly = true;
   }
   else
   {
      uniquesOnly = false;
   }

   std::multimap<std::string, std::string> bigMap;
   for(auto singleLine : lines)
   {
      size_t endOfHash = singleLine.find(' ');
      bigMap.insert(std::make_pair(singleLine.substr(0, endOfHash), singleLine.substr(endOfHash + 2)));
   }

   std::string lastHash = "";
   for(auto record : bigMap)
   {
      std::string thisHash = record.first;

      if (bigMap.count(thisHash) == 1)
      {
         // This hash is unique
         if (uniquesOnly)
         {
            std::cout << thisHash << "\t" << record.second << std::endl;
         }
      }
      else
      {
         if (!uniquesOnly)
         {
            // This hash isn't unique
            if (thisHash == lastHash)
            {
               // Print it as a dupe
               std::cout << "DUPE\t" << thisHash << "\t\"" << record.second << "\"" << std::endl;
            }
            else
            {
               std::cout << "UNIQ\t" << thisHash << "\t\"" << record.second << "\"" << std::endl;
            }

            lastHash = thisHash;
         }
      }
   }
}

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      printUsage();
      return 0;
   }

   bool useFilenameSort = true;
   bool useMultiMapSort = false;

   if (strcmp("-f", argv[1]) == 0)
   {
      useFilenameSort = true;
   }
   else if (strcmp("-h", argv[1]) == 0)
   {
      useFilenameSort = false;
   }
   else if (strcmp("-n", argv[1]) == 0)
   {
      useMultiMapSort = true;
   }
   else if (strcmp("-u", argv[1]) == 0)
   {
      useMultiMapSort = true;
   }
   else
   {
      printUsage();
      return 0;
   }

   std::string singleLine;
   std::vector<std::string> linesOfText;
	while(std::getline(std::cin, singleLine))
	{
      //std::cout << "Hey! " << singleLine << std::endl;
      linesOfText.push_back(singleLine);
	}

   if (useMultiMapSort)
   {
      performMultiMapSort(linesOfText, argv[1]);
      return 0;
   }

   if (!useFilenameSort)
   {
      // My c++ lambda viriginity was taken below...
      std::sort(linesOfText.begin(), linesOfText.end(),
                [](std::string const & a, std::string const & b){ return a < b; } );
   }
   else
   {
      std::sort(linesOfText.begin(), linesOfText.end(), sortByFilename);
   }

   // Output sorted text
   for(auto outline : linesOfText)
   {
      std::cout << outline << std::endl;
   }


   return 0;

}
