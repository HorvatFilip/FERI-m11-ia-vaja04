#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main()
{

    string s = "10 20 50 10 5.30 0.00001 1";

    istringstream iss(s);
    string sub;
    while (iss >> sub)
    {
        cout << sub << endl;
    }

    cout << endl
         << endl;

    return 0;
}
