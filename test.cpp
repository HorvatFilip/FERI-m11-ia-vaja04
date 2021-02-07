#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    int *A = new int[5];
    for (int i = 0; i < 5; i++)
    {
        A[i] = i + 1;
    }

    vector<int> B;
    B = vector<int>(5);

    copy(A + 0, A + 5, B.begin());

    for (int i = 0; i < 5; i++)
    {
        cout << B.at(i) << " ";
    }

    cout << endl
         << endl;

    return 0;
}
