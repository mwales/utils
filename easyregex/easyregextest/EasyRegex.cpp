#include <sys/types.h>
#include <regex.h>
#include <sstream>

#include "EasyRegex.h"

const int EasyRegex::ERROR_MSG_MAX_LEN = 255;

EasyRegex::EasyRegex(std::string pattern):
   thePattern(pattern),
   theErrorFlag(true),
   theErrorMsg("Need to call match first"),
   theRegexFlags( REG_EXTENDED | REG_NEWLINE),
   theMaxMatches(20)
{
   // Don't do anything here, do everything in match
}


bool EasyRegex::match(std::string haystack, bool* error)
{
   // Reset members for new match
   theCapturedTexts.clear();
   theEntireMatch.clear();
   theErrorFlag = false;
   theErrorMsg = "";

   regex_t regexPatternBuffer;
   int compilationStatus = regcomp(&regexPatternBuffer, thePattern.c_str(), theRegexFlags);

   if (compilationStatus)
   {
      // Error compiling the regular expression
      char errBuf[ERROR_MSG_MAX_LEN];
      regerror(compilationStatus, &regexPatternBuffer, errBuf, ERROR_MSG_MAX_LEN);

      theErrorMsg = "Error during regex compilation: ";
      theErrorMsg += errBuf;

      theErrorFlag = true;

      // If user specified an error return variable, update it
      if (error)
      {
         *error = true;
      }

      regfree(&regexPatternBuffer);
      return false;
   }
   regmatch_t* matchPositions = new regmatch_t[theMaxMatches];

   int matchStatus = regexec(&regexPatternBuffer, haystack.c_str(), theMaxMatches, matchPositions, 0);

   bool retVal = (matchStatus == 0);

   // If match worked, process all the matched texts
   if (retVal)
   {
      for(int i = 0; i < theMaxMatches; i++)
      {
         if ( (matchPositions[i].rm_so == -1) ||
              (matchPositions[i].rm_eo == -1) )
         {
            break;
         }

         int numChars = matchPositions[i].rm_eo - matchPositions[i].rm_so;

         if (i == 0)
         {
            // Special case, the first match
            theEntireMatch = haystack.substr(matchPositions[i].rm_so, numChars);
         }
         else
         {
            theCapturedTexts.push_back(haystack.substr(matchPositions[i].rm_so, numChars));
         }
      }
   }

   if (error)
   {
      *error = false;
   }

   delete[] matchPositions;
   return retVal;
}

int EasyRegex::captureCount()
{
   return theCapturedTexts.size();
}

std::string EasyRegex::getCapture(unsigned int index)
{
   if ( index >= theCapturedTexts.size() )
   {
      return "";
   }

   return theCapturedTexts.at(index);
}

std::vector<std::string> EasyRegex::getAllCaptures()
{
   return theCapturedTexts;
}

bool EasyRegex::isError()
{
   return theErrorFlag;
}

std::string EasyRegex::getErrorMessage()
{
   return theErrorMsg;
}

std::string EasyRegex::getEntireMatch()
{
   return theEntireMatch;
}

void EasyRegex::setMaxNumberOfMatches(int max)
{
   theMaxMatches = max;
}

void EasyRegex::setPattern(std::string pattern)
{
   thePattern = pattern;
}
