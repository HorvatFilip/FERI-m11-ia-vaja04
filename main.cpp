#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void ReadSimplexFile(string fileName, int &n, int &m, float **(&A), float *(&b), float *(&c))
{

    ifstream inputFile(fileName);
    string line, numStr;
    istringstream *iss;
    int state = 0, indx = 0, i = 0;
    float tmp;

    while (getline(inputFile, line))
    {
        iss = new istringstream(line);
        if (state == 0)
        {
            while (*iss >> numStr)
            {
                if (state == 0)
                {
                    n = stoi(numStr);
                    state++;
                }
                else
                {
                    m = stoi(numStr);
                }
            }
            A = new float *[n + m];
            b = new float[n + m];
            c = new float[n + m];
        }
        else if (state == 1 || state == (n + m + 2) || state == (n + m + 4))
        {
            state++;
        }
        else if (state <= (n + m + 1))
        {
            A[indx] = new float[n + m];

            while (*iss >> numStr)
            {
                A[indx][i] = stof(numStr);
                i++;
            }

            i = 0;
            indx++;
            state++;
        }
        else if (state == (n + m + 3))
        {
            while (*iss >> numStr)
            {
                b[i] = stof(numStr);
                i++;
            }
            i = 0;
            state++;
        }
        else
        {
            while (*iss >> numStr)
            {
                c[i] = stof(numStr);
                i++;
            }
            break;
        }
    }
    delete iss;
}

void PrintSimplexData(int n, int m, float **A, float *b, float *c)
{
    cout << "n,m: " << n << ", " << m << endl
         << endl;
    cout << "A:" << endl;
    for (int i = 0; i < (n + m); i++)
    {
        for (int j = 0; j < (n + m); j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "b:" << endl;
    for (int i = 0; i < (n + m); i++)
    {
        cout << b[i] << " ";
    }
    cout << endl
         << endl;

    cout << "c:" << endl;

    for (int i = 0; i < (n + m); i++)
    {
        cout << c[i] << " ";
    }
    cout << endl;

    cout << endl
         << endl;
}

bool InitializeSimplex(int n, int m, float *b, int *(&N), int *(&B), int &v)
{
    for (int i = 0; i < (n + m); i++)
    {
        if (b[i] < 0)
            return false;
    }

    N = new int[n];
    for (int i = 0; i < n; i++)
        N[i] = i + 1;

    B = new int[m];
    for (int i = 0; i < m; i++)
        B[i] = n + i + 1;

    v = 0;

    return true;
}

bool Run(float *c, int *N, int n, int &e)
{
    for (int k = 0; k < n; k++)
    {
        if (c[N[k] - 1] > 0)
        {
            e = c[N[k] - 1];
            return true;
        }
    }
    return false;
}

void Simplex(int n, int m, float **A, float *b, float *c)
{
    int *N = NULL, *B = NULL;
    int v, e;

    if (InitializeSimplex(n, m, b, N, B, v))
    {
        while (Run(c, N, n, e))
        {
        }
    }
    else
    {
        cout << "Error - negative val in b vector" << endl;
        return;
    }
}

int main()
{

    int n, m;
    float **A = NULL;
    float *b = NULL, *c = NULL;

    string fileName = "./doc/testOsnovniSimplex.txt";

    ReadSimplexFile(fileName, n, m, A, b, c);

    PrintSimplexData(n, m, A, b, c);

    return 0;
}
