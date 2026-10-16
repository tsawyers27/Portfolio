#include "grid.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

vector<vector<char>> read_grid()
{
    vector<vector<char>> grid;

    string file_name;
    cout << "Enter grid file name: ";
    cin >> file_name;

    fstream file(file_name);

    char ch;
    vector<char> row;
    while (file.get(ch))
    {
        if (ch != '\n')
        {
            row.push_back(ch);
        }
        else if (ch == '\n')
        {
            grid.push_back(row);
            row.clear();
        }
    }

    file.close();
    return grid;
}

void display_grid(const vector<vector<char>>& grid)
{
    for (const auto& row : grid)
    {
        for (char elem : row)
        {
            cout << elem << " ";
        }
        cout << endl;
    }
}

void display_solved_grid(const vector<vector<char>>& grid, const vector<pair<int, int>>& path)
{
    vector<vector<char>> solved = grid;
    for (const auto& point : path)
    {
        if (solved[point.first][point.second] != 'S' && solved[point.first][point.second] != 'G')
        {
            solved[point.first][point.second] = '*';
        }
    }

    for (const auto& row : solved)
    {
        for (char elem : row)
        {
            cout << elem << " ";
        }
        cout << endl;
    }

}