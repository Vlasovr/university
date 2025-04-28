

//#include <iostream>
#include "String.h"

int main()
{
    char ss[80];
    cout << "Vvod str" << endl;
    cin.getline(ss, 80);
    String originalString(ss);
    system("pause");
    String copiedString(originalString);
    system("pause");
    String newString = originalString + copiedString;
    system("pause");
    cout << newString.getString();
    cout << endl;
    return 0;
}

