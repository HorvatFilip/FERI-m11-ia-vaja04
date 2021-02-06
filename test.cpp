#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iterator>

using namespace std;

int main()
{
    int *A = new int[5];
    for (int i = 0; i < 5; i++)
    {
        A[i] = i + 1;
    }

    for (int i = 0; i < 5; i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;

    int *n_A = new int[5];

    copy(A, A + 5, n_A);
    for (int i = 0; i < 5; i++)
    {
        cout << n_A[i] << " ";
    }
    cout << endl;

    cout << endl
         << endl;

    return 0;
}
