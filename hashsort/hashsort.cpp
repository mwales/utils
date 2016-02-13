#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <string.h>

void printUsage()
{
   std::cerr << "Usage: hashsort [-f] [-h] < hashes_to_sort.md5" << std::endl;
   std::cerr << "  -f : sort hashes by filename" << std::endl;
   std::cerr << "  -h : sort hashes by hash" << std::endl;
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

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      printUsage();
      return 0;
   }

   bool useFilenameSort = true;

   if (strcmp("-f", argv[1]) == 0)
   {
      useFilenameSort = true;
   }
   else if (strcmp("-h", argv[1]) == 0)
   {
      useFilenameSort = false;
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

   if (useFilenameSort)
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
