#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>

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
    for (int i = 0; i < n + m; i++)
    {
        for (int j = 0; j < n + m; j++)
        {
            n_A[i][j] = 0;
        }
        n_b[i] = 0;
        n_c[i] = 0;
    }

    n_N = vector<int>(n);
    n_B = vector<int>(m);
    copy(N + 0, N + n, n_N.begin());
    copy(B + 0, B + m, n_B.begin());

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

    for (int i = 0; i < n + m; i++)
    {
        for (int j = 0; j < n + m; j++)
        {
            A[i][j] = n_A[i][j];
        }
        b[i] = n_b[i];
        c[i] = n_c[i];
    }

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
    float *delta = NULL;
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
            delete[] delta;
        }
        delta = NULL;
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
        delete[] N;
        N = NULL;
        delete[] B;
        B = NULL;

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

void SimplexFromFile()
{

    cout << "Simplex- file data" << endl
         << endl;

    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;
    fileName = "./doc/" + fileName + ".txt";

    int n, m;
    float **A = NULL;
    float *b = NULL, *c = NULL;

    if (!ReadSimplexFile(fileName, n, m, A, b, c))
    {
        cout << "File not found!" << endl;
        return;
    }

    float *x = NULL;
    float z;

    if (!Simplex(m, m, A, b, c, x, z))
    {
        return;
    }

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
    }

    for (int i = 0; i < (n + m); i++)
    {
        for (int j = 0; j < (n + m); j++)
        {
            if (i >= n && j < n)
            {
                A[i][j] = rand() % 1000 + 1;
            }
            else
            {
                A[i][j] = 0;
            }
        }
    }

    b = new float[n + m];
    for (int i = 0; i < (n + m); i++)
    {
        if (i >= n)
            b[i] = rand() % 1000 + 1;
        else
            b[i] = 0;
    }

    c = new float[n + m];
    for (int i = 0; i < (n + m); i++)
    {
        if (i < n)
            c[i] = rand() % 1000 + 1;
        else
            c[i] = 0;
    }
}

void ProcessSimplexTestData(vector<int> data)
{

    int min, max, Q1, Q3, sum = 0;
    double mean, median, Q2, tmp;
    int dataSize = data.size();

    bubbleSort(data, dataSize);

    min = data[0];
    max = data[dataSize - 1];

    for (int i = 0; i < dataSize; i++)
        sum += data[i];
    mean = sum / dataSize;

    if (dataSize % 2 == 0)
    {
        int x1, x2;
        x2 = dataSize / 2;
        x1 = x2 - 1;
        median = (data[x1] + data[x2]) / 2;

        x1 = dataSize / 4;
        Q1 = data[x1];

        x2 = x1 + dataSize / 2;
        Q3 = data[x2];
    }
    else
    {
    }

    cout << "min: " << min << " max: " << max << " Q1: " << Q1 << " meadian: " << median << " Q3: " << Q3 << endl;
}

void TestSimplex()
{
    int n, m;
    float **A = NULL;
    float *b = NULL, *c = NULL;

    float *x = NULL;
    float z;

    int maxDataSize = 50;
    int testPerData = 100;

    chrono::high_resolution_clock::time_point t1;
    chrono::high_resolution_clock::time_point t2;
    chrono::microseconds time_span;
    int sum;
    vector<int> testTimes(testPerData);

    for (n = 2; n <= maxDataSize; n++)
    {
        testTimes.clear();
        for (int j = 0; j < testPerData; j++)
        {
            GenerateSimplexData(n, n, A, b, c);

            t1 = chrono::high_resolution_clock::now();
            Simplex(n, n, A, b, c, x, z);
            t2 = chrono::high_resolution_clock::now();
            time_span = chrono::duration_cast<chrono::microseconds>(t2 - t1);

            testTimes.push_back(time_span.count());

            //PrintSiplexResult(n, n, x, z);

            for (int i = 0; i < (n + n); i++)
            {
                delete[] A[i];
            }
            delete[] A;
            delete[] b;
            delete[] c;
            delete[] x;
        }
        bubbleSort(testTimes, testTimes.size());
        cout << "Test Times for matrix size: " << n << endl;
        for (int i = 0; i < testTimes.size(); i++)
        {
            cout << testTimes[i] << endl;
        }
        //ProcessSimplexTestData(testTimes);
    }
}

int main()
{

    cout << "==== Simplex alg ====" << endl;

    int choice = 0;
    while (true)
    {
        cout << "Simplex from file - 1" << endl;
        cout << "Generate simplex matrix(test) - 2" << endl;
        cin >> choice;

        if (choice == 1)
            SimplexFromFile();
        else if (choice == 2)
            TestSimplex();
        else
            break;
    }

    cout << endl
         << endl;

    return 0;
}
