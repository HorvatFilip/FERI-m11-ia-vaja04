#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

bool ReadSimplexFile(string fileName, int &n, int &m, float **(&A), float *(&b), float *(&c))
{

    ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        return false;
    }
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
    return true;
}

void PrintSimplexData(int n, int m, float **A, float *b, float *c, int *N, int *B)
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
    cout << endl
         << endl;

    cout << "N:" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << N[i] << " ";
    }
    cout << endl
         << endl;

    cout << "B:" << endl;

    for (int i = 0; i < m; i++)
    {
        cout << B[i] << " ";
    }
    cout << endl;

    cout << endl
         << endl;
}

bool InitializeSimplex(int n, int m, float *b, int *(&N), int *(&B), float &v)
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
            e = N[k] - 1;
            return true;
        }
    }
    return false;
}

void Find_l(int &l, float *delta, int m)
{
    l = 0;
    for (int i = 1; i < m; i++)
    {
        if (delta[l] > delta[i])
            l = i;
    }
}

float **n_A = NULL;
float *n_b = NULL;
float *n_c = NULL;
vector<int> n_N;
vector<int> n_B;
float n_v;

void InitWorkingBuffers(int n, int m)
{
    n_A = new float *[n + m];
    for (int i = 0; i < (n + m); i++)
    {
        n_A[i] = new float[n + m];
    }
    n_b = new float[n + m];
    n_c = new float[n + m];
    n_N = vector<int>(n);
    n_B = vector<int>(m);
    n_v = 0;
}

void FreeWorkingBuffers(int n, int m)
{
    for (int i = 0; i < (n + m); i++)
    {
        delete[] n_A[i];
        n_A[i] = NULL;
    }
    delete[] n_A;
    n_A = NULL;
    delete[] n_b;
    n_b = NULL;
    delete[] n_c;
    n_c = NULL;
}

void bubbleSort(vector<int> &arr, int n)
{
    int i, j;
    int tmp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
    }
}

void Pivot(int n, int m, float **(&A), float *(&b), float *(&c), int *(&N), int *(&B), float &v, int l, int e)
{
    for (int i = 0; i < (n + m); i++)
    {
        memset(n_A[i], 0, (n + m) * sizeof(float));
    }
    memset(n_b, 0, (n + m) * sizeof(float));
    memset(n_c, 0, (n + m) * sizeof(float));
    n_N = vector<int>(n);
    n_B = vector<int>(m);
    copy(N + 0, N + n, n_N.begin());
    copy(B + 0, B + m, n_B.begin());

    //PrintSimplexData(n, m, A, b, c, N, B);

    //n_be := bl/ale;
    n_b[e] = b[l] / A[l][e];

    //for vsak j v N - {e}
    for (int j = 0; j < n; j++)
    {
        if (N[j] - 1 == e)
            continue;

        //n_aej := alj/ale;
        n_A[e][N[j] - 1] = A[l][N[j] - 1] / A[l][e];
    }
    //n_ael := 1/ale;
    n_A[e][l] = 1 / A[l][e];

    //for vsak i v B - {l}
    for (int i = 0; i < m; i++)
    {
        if (B[i] - 1 == l)
            continue;

        //n_bi := bi - aie*n_be
        n_b[B[i] - 1] = b[B[i] - 1] - A[B[i] - 1][e] * n_b[e];

        //for vsak j v N - {e}
        for (int j = 0; j < n; j++)
        {
            if (N[j] - 1 == e)
                continue;

            //n_aij := aij - aie*n_aej
            n_A[B[i] - 1][N[j] - 1] = A[B[i] - 1][N[j] - 1] - A[B[i] - 1][e] * n_A[e][N[j] - 1];
        }
        //n_ail := -aie*n_ael
        n_A[B[i] - 1][l] = -A[B[i] - 1][e] * n_A[e][l];
    }

    //n_v := v + ce*n_be
    n_v = v + c[e] * n_b[e];

    //for vsak j v N - {e}
    for (int j = 0; j < n; j++)
    {
        if (N[j] - 1 == e)
            continue;

        //n_cj := cj - ce*n_aej
        n_c[N[j] - 1] = c[N[j] - 1] - c[e] * n_A[e][N[j] - 1];
    }

    //n_cl := -ce*n_ael
    n_c[l] = -c[e] * n_A[e][l];

    //n_N := N-{e} U {l}
    for (int i = 0; i < n; i++)
    {
        if (n_N[i] == e + 1)
            n_N.erase(n_N.begin() + i);
    }
    n_N.push_back(l + 1);
    //bubbleSort(n_N, n);

    //n_B := B-{l} U {e}
    for (int i = 0; i < m; i++)
    {
        if (n_B[i] == l + 1)
            n_B.erase(n_B.begin() + i);
    }
    n_B.insert(n_B.begin(), 1, e + 1);
    //bubbleSort(n_B, m);

    for (int i = 0; i < (m + n); i++)
        copy(n_A[i] + 0, n_A[i] + (n + m) * sizeof(float), A[i]);

    copy(n_b, n_b + (n + m) * sizeof(float), b);
    copy(n_c, n_c + (n + m) * sizeof(float), c);

    copy(n_N.begin(), n_N.end(), N);
    copy(n_B.begin(), n_B.end(), B);

    v = n_v;
}

bool IsinB(int *B, int m, int i, int &indx)
{
    for (int j = 0; j < m; j++)
    {
        if (i == B[j])
        {
            indx = j;
            return true;
        }
    }
    return false;
}

bool Simplex(int n, int m, float **A, float *b, float *c, float *(&x), float(&z))
{
    int *N, *B;
    int e, l;
    float *delta = new float[m];
    float inf = numeric_limits<float>::infinity();
    float v;
    x = new float[n + m];
    int state = 0, indx;

    if (InitializeSimplex(n, m, b, N, B, v))
    {
        while (Run(c, N, n, e))
        {
            delta = new float[m];
            for (int j = 0; j < m; j++)
            {
                if (A[B[j] - 1][e] > 0)
                    delta[j] = b[B[j] - 1] / A[B[j] - 1][e];
                else
                    delta[j] = inf;
            }
            Find_l(l, delta, m);

            if (delta[l] == inf)
            {
                cout << "Error - inf program" << endl;
                return false;
            }
            else
            {
                if (state++ == 0)
                    InitWorkingBuffers(n, m);

                l = B[l] - 1;
                Pivot(n, m, A, b, c, N, B, v, l, e);
            }
        }
        //PrintSimplexData(n, m, A, b, c, N, B);

        for (int i = 0; i < (n + m); i++)
        {
            if (IsinB(B, m, i + 1, indx))
            {
                x[i] = b[B[indx] - 1];
            }
            else
                x[i] = 0;
        }
        z = v;
        FreeWorkingBuffers(n, m);
        //delete[] N;
        N = NULL;
        delete[] B;
        B = NULL;
        delete[] delta;
        delta = NULL;

        return true;
    }
    else
    {
        cout << "Error - negative val in b vector" << endl;
        return false;
    }
}

void PrintSiplexResult(int n, int m, float *x, float z)
{
    for (int i = 0; i < (n + m); i++)
    {
        cout << "x" << i + 1 << ": " << x[i] << endl;
    }
    cout << endl;
    cout << "z: " << z << endl
         << endl;
}

void MySimplex()
{

    cout << "Simplex- file data" << endl
         << endl;

    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;

    int n, m;
    float **A = NULL;
    float *b = NULL, *c = NULL;

    if (!ReadSimplexFile(fileName, n, m, A, b, c))
    {
        cout << "File not found!" << endl;
    }

    float *x = NULL;
    float z;

    Simplex(m, m, A, b, c, x, z);

    PrintSiplexResult(n, m, x, z);

    for (int i = 0; i < (n + m); i++)
    {
        delete[] A[i];
    }
    delete[] A;
    delete[] b;
    delete[] c;
    delete[] x;
}

void GenerateSimplexData(int n, int m, float **(&A), float *(&b), float *(&c))
{
    srand(time(NULL));

    A = new float *[n + m];
    for (int i = 0; i < (n + m); i++)
    {
        A[i] = new float[n + m];
        memset(A[i], 0, (n + m) * sizeof(float));
    }

    for (int i = n; i < (n + m); i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = rand() % 1000 + 1;
        }
    }

    b = new float[n + m];
    for (int i = n; i < (n + m); i++)
    {
        b[i] = rand() % 1000 + 1;
    }

    c = new float[n + m];
    for (int i = 0; i < n; i++)
    {
        c[i] = rand() % 1000 + 1;
    }
}

void TestSimplex()
{
    int n, m;
    float **A = NULL;
    float *b = NULL, *c = NULL;

    float *x = NULL;
    float z;

    m = n = 6;

    GenerateSimplexData(n, m, A, b, c);

    Simplex(m, m, A, b, c, x, z);

    for (int i = 0; i < (n + m); i++)
    {
        delete[] A[i];
    }
    delete[] A;
    delete[] b;
    delete[] c;
    delete[] x;
}

int main()
{
    TestSimplex();

    return 0;
}
