
#include <iostream>
#include "EasyRegex.h"

using namespace std;

int main()
{
   string address = "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n";

   EasyRegex re(".*inet (.*)");

   bool matchSuccess = re.match(address);

   cout << "Match success = " << matchSuccess << endl;
   cout << "Errors = " << re.isError() << endl;

   if (re.isError())
   {
      cout << "Error Msg = " << re.getErrorMessage() << endl;
   }

   return 0;
}

