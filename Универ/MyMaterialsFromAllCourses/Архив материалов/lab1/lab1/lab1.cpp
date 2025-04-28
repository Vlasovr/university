#include "String.h"

int main()
{
    char ss[80];
    cout << "Vvod str" << endl;
    cin.getline(ss, 80);
    String originalString(ss);
    cout << "1= " << endl;
    originalString.show();
    system("pause");
    String copiedString(originalString);
    cout << "2= " << endl;
    copiedString.show();
    system("pause");
    String newString = originalString + copiedString;
    system("pause");
    newString.show();
    cout << endl;
    return 0;
}
