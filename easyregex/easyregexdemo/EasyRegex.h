#ifndef EASYREGEX_H
#define EASYREGEX_H

#include <iostream>
#include <vector>

class EasyRegex
{
public:
   EasyRegex(std::string pattern);

   /**
    * Performs the regex operation
    *
    * @param haystack String that will be search / matched against
    * @param error Optional parameter that will be set to true if their was an error
    * @return True if there was a successful match
    */
   bool match(std::string haystack, bool* error = NULL);

   int captureCount();

   std::string getCapture(int index);

   std::vector<std::string> getAllCaptures();

   bool isError();

   std::string getErrorMessage();

   std::string getEntireMatch();

   void setMaxNumberOfMatches(int max);

protected:

   static const int ERROR_MSG_MAX_LEN;

   std::string thePattern;

   std::vector<std::string> theCapturedTexts;

   bool theErrorFlag;

   std::string theErrorMsg;

   int theRegexFlags;

   int theMaxMatches;

   std::string theEntireMatch;
};

#endif // EASYREGEX_H
