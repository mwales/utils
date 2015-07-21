#ifndef EASYREGEX_H
#define EASYREGEX_H

#include <iostream>
#include <vector>
#include <regex.h>

/**
 * Class to simplify usage of POSIX regular expressions.  The regular expression library is configured with the
 * following POSIX configuration options:
 *
 * @li REG_EXTENDED: POSIX extended regular expressions
 * @li REG_NEWLINE:  Don't match newlines
 */
class EasyRegex
{
public:
   /**
    * Creates a regex pattern to match with (POSIX extended regular expressions)
    * @param pattern Regular expression to match with
    */
   EasyRegex(std::string pattern);

   /// Destructor cleans up memory used by matching offsets array
   ~EasyRegex();

   /**
    * Performs the regex operation
    *
    * @param haystack String that will be search / matched against
    * @param error Optional parameter that will be set to true if their was an error
    * @return True if there was a successful match
    */
   bool match(std::string haystack, bool* error = NULL);

   /// Number of subcaptures captured
   int captureCount();

   /**
    * Gets a subcapture
    * @param index
    * @return Subcapture, or an empty string if no subcapture for the index specified
    */
   std::string getCapture(unsigned int index);

   /// Returns all the subcaptures found (or an empty list if none)
   std::vector<std::string> getAllCaptures();

   /// Returns true if the last match generated an error
   bool isError();

   /// Error message returned by the last match
   std::string getErrorMessage();

   /**
    * Returns the entire match (the first returned match by the POSIX regex library)
    * @return
    */
   std::string getEntireMatch();

   /**
    * Sets the maximum number of matches that we can possibly match.  The higher this is set, the more memory is
    * consumed by the object
    *
    * @param max Maximum number of matches to return
    */
   void setMaxNumberOfMatches(int max);

   /**
    * Sets the pattern for the regular expression.  The regex will not be compiled until the match method is called
    *
    * @param pattern New regular expression pattern to use
    */
   void setPattern(std::string pattern);

protected:

   static const int ERROR_MSG_MAX_LEN;

   std::string thePattern;

   std::vector<std::string> theCapturedTexts;

   bool theErrorFlag;

   std::string theErrorMsg;

   int theRegexFlags;

   int theMaxMatches;

   std::string theEntireMatch;

   regmatch_t* theMatchPositions;
};

#endif // EASYREGEX_H
