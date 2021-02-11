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
    float **A = new float *[5];
    for (int i = 0; i < 5; i++)
    {
        A[i] = new float[5];
        for (int j = 0; j < 5; j++)
        {
            A[i][j] = 0;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    float **B = new float *[5];
    for (int i = 0; i < 5; i++)
    {
        B[i] = new float[5];
        for (int j = 0; j < 5; j++)
        {
            B[i][j] = 1;
        }
    }

    int n = 5;

    copy(B + 0, B + (n + n) * (n + n), A + 0);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl
         << endl;

    return 0;
}
