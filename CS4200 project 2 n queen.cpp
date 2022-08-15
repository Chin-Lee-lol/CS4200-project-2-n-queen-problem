#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

vector<int> generatePuzzle(int n)
{
    vector<int> result;
    for (int i = 0; i < n; i++)
        result.push_back(rand()%n);
    random_shuffle(result.begin(), result.end());
    return result;
}

void print(vector<vector<char>> input, int n)
{
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << input.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

int conflict(vector<vector<char>> input, int n) // check queen attacking
{ // i = row, j = col row first then col lol so confusing
    int fight = 0, count = 0, temp = 0;
    for (int row = 0; row < n; row++)
        for (int col = 0; col < n; col++)
            if (input.at(row).at(col) == 'Q')            
                for (int i = col+1; i < n; i++)
                    if (input.at(row).at(i) == 'Q') // check conflict in same row basically
                        fight++;                          
    for (int row = 1; row < n; row++)
    {
        int i = row;
        for (int col = 0; col <= n; col++) // count Q in diagonal
        {
            if (input.at(i).at(col) == 'Q')
                count++;
            i--;
            if (i == -1)
                break;
        }
        if (count > 1)
            for (int j = 1; j < count; j++) // calculate conflict according to Q in diagonal
                fight += j;
        count = 0;
    }
    for (int col = 1; col < n; col++) // check the remain diagonal
    {
        int i = col;
        for (int row = n-1; row >= 0; row--)
        {
            if (input.at(row).at(i) == 'Q')
                count++;           
            i++;
            if (i == n)
                break;
        }
        if (count > 1)
            for (int j = 1; j < count; j++)
                fight += j;
        count = 0;
    }

    for (int col = 1; col < n; col++)
    {
        int i = col;
        for (int row = n-1; row >= 0; row--) // count Q in othogonal
        {
            if (input.at(row).at(i) == 'Q')
                count++;
            i--;
            if (i == -1)
                break;
        }
        if (count > 1)
            for (int j = 1; j < count; j++) // calculate conflict according to Q in othogonal
                fight += j;
        count = 0;
    }
    for (int row = n-2; row >= 0; row--)
    {
        int i = row;
        for (int col = n - 1; col >= 0; col--)
        {
            if (input.at(i).at(col) == 'Q')
                count++;
            i--;
            if (i == -1)
                break;
        }
        if (count > 1)
            for (int j = 1; j < count; j++)
                fight += j;
        count = 0;
    } // this part really confusing to implement lol
    return fight;
}

vector<vector<char>> hillClimb(vector<vector<char>> input, int n, int lowest)
{
    vector<vector<char>> next, check;
    int temp = 0, begin = lowest;
    for (int row = 0; row < n; row++)
        for (int col = 0; col < n; col++)
            if (input.at(row).at(col) == 'Q')            
                for (int i = 0; i < n; i++)
                {
                    check = input;
                    if (i != row) // no need swap same position
                    {
                        check.at(i).at(col) = 'Q';
                        check.at(row).at(col) = '-';
                        temp = conflict(check, n);
                        if (temp < lowest)
                        {
                            next = check;
                            lowest = temp;
                            //print(next, n);
                        }
                    }
                }       
    if (lowest == begin)
        return input;
    return hillClimb(next,n,lowest);
}

int hasConflict(vector<vector<char>>input, int n, int Qcol)
{ // check conflict for Q in that selected column, if there's conflict return Q's row if not return -1
    int Qrow = -1;
    for (int row = 0; row < n; row++)
        if (input.at(row).at(Qcol) == 'Q')
            Qrow = row; // locate Q position
    for (int col = 0; col < n; col++)
        if (col != Qcol)
            if (input.at(Qrow).at(col) == 'Q') // check same row for Q
                return Qrow;
    int row = Qrow + 1, col = Qcol + 1; // check down right for Q
    while (row != n && col != n)
    {
        if (input.at(row).at(col) == 'Q')
            return Qrow;
        row++; col++;
    }
    row = Qrow + 1, col = Qcol - 1; // check down left for Q
    while (row != n && col != -1)
    {
        if (input.at(row).at(col) == 'Q')
            return Qrow;
        row++; col--;
    }
    row = Qrow - 1, col = Qcol + 1; // check up right for Q
    while (row != -1 && col != n)
    {
        if (input.at(row).at(col) == 'Q')
            return Qrow;
        row--; col++;
    }
    row = Qrow - 1, col = Qcol - 1; // check up left for Q
    while (row != -1 && col != -1)
    {
        if (input.at(row).at(col) == 'Q')
            return Qrow;
        row--; col--;
    }
    
    return -1;
}

int QConflict(vector<vector<char>>input, int n, int Qcol, int Qrow)
{ // check conflict for Q in that selected column, if there's conflict return Q's row if not return -1
    int fight = 0;

    for (int col = 0; col < n; col++)
        if (col != Qcol)
            if (input.at(Qrow).at(col) == 'Q')
                fight++;
    int row = Qrow + 1, col = Qcol + 1; // check down right for Q
    while (row != n && col != n)
    {
        if (input.at(row).at(col) == 'Q')
            fight++;
        row++; col++;
    }
    row = Qrow + 1, col = Qcol - 1; // check down left for Q
    while (row != n && col != -1)
    {
        if (input.at(row).at(col) == 'Q')
            fight++;
        row++; col--;
    }
    row = Qrow - 1, col = Qcol + 1; // check up right for Q
    while (row != -1 && col != n)
    {
        if (input.at(row).at(col) == 'Q')
            fight++;
        row--; col++;
    }
    row = Qrow - 1, col = Qcol - 1; // check up left for Q
    while (row != -1 && col != -1)
    {
        if (input.at(row).at(col) == 'Q')
            fight++;
        row--; col--;
    }

    return fight;
}

vector<vector<char>> minConflict(vector<vector<char>> input, int n,int tries)
{
    int Qcol, Qrow, temp, lowest = INT_MAX;
    vector<vector<char>> cur = input, check, next ;
    for (int i = 0; i < tries; i++)
    {
        if (conflict(cur, n) == 0)
            return cur;
        while (true)
        {
            Qcol = rand() % n; 
            Qrow = hasConflict(cur, n, Qcol);
            if (Qrow != -1) // it has conflict we continue
                break;
        }
        for (int row = 0; row < n; row++)
        {            
            if (row != Qrow) // choose lowest conflict one to become next cur
            {
                check = cur;
                check.at(row).at(Qcol) = 'Q';
                check.at(Qrow).at(Qcol) = '-';
                temp = QConflict(check, n, Qcol, row);
                if (temp < lowest)
                {
                    lowest = temp;
                    next = check;
                }
            }           
        }
        lowest = INT_MAX; // reset lowest value
        cur = next;
    }
    return {}; // failed to find the solution
}

using namespace std::chrono;

int main()
{
    srand(time(0));
    vector<vector<char>> puzzle,output;
    cout << "Welcome to Chin's n-queen solver!\nPlease input size of the board: ";
    int n, fight, run = 1, count = 0;
    cin >> n;
    auto start = high_resolution_clock::now();
    for (int k = 0; k < run; k++)
    {
        vector<int> input = generatePuzzle(n);
        //input = { 1,3,3,1 };
        puzzle.resize(n);
        for (int i = 0; i < n; i++)
            puzzle.at(i).resize(n); // initialize the board
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (input.at(j) == i)
                    puzzle.at(i).at(j) = 'Q';
                else
                    puzzle.at(i).at(j) = '-';
        cout << "I have generated:";
        print(puzzle, n);
        fight = conflict(puzzle, n);
        cout << "\nThere are " << fight << " pairs of queen attacking each other\n\nHill-Climbing method\n";
        output = hillClimb(puzzle, n, fight);
        fight = conflict(output, n);
        print(output, n);
        cout << "\n\nThere are " << fight << " pairs of queen attacking each other\n\nMin-Conflict method\n";
        output = minConflict(puzzle, n, 5000);
        if (output.empty())
        {
            cout << "failed to find the solution\n";
            continue;
        }
        print(output, n);
        cout << "\n\nThere are 0 pairs of queen attacking each other\n";
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nduration: " << ((float)duration.count()) / 1000000 << endl;
    cout << endl;
    system("pause");
    return 0;
}
