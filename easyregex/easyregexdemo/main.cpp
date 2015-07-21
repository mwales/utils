
#include <iostream>
#include "EasyRegex.h"

using namespace std;

int main()
{
   // A typical line of text from ip addr show
   string address = "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n";

   // Regular expression that will get us the IP address and net mask via subcaptures
   EasyRegex re(".*inet ([0-9\\.]*)/([0-9]*) .*");

   bool matchSuccess = re.match(address);

   cout << "Match success = " << matchSuccess << endl;
   cout << "Errors = " << re.isError() << endl;

   if (re.isError())
   {
      cout << "Error Msg = " << re.getErrorMessage() << endl;
   }
   else if (!matchSuccess)
   {
      cout << "Couldn't find the IP address or net mask in the string" << endl;
   }
   else
   {
      cout << "IP Address = " << re.getCapture(0) << endl;
      cout << "Net Mask   = " << re.getCapture(1) << endl;
   }

   return 0;
}

