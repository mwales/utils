# utils

Small utilities that I (or others) may find helpful.

EasyRegex:  Wrapper for the C POXIX regular expression library.  Takes care of
managing the memory used by the library, and then uses std::strings for patterns
and matches.

hashsort:  I sometimes will run the sort application on a md5 hash file which
then sorts them by hash.  This application will resort them back into order
based on filename.
