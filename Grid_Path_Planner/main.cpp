#include "grid.h"
#include "planner.h"
#include <iostream>
using namespace std;

int main()
{    
    vector<vector<char>> grid = read_grid();
    display_grid(grid);

    pair<int, int> start = get_start(grid, 'S');
    pair<int, int> goal = get_goal(grid, 'G');

    cout << "Starting Point: ";
    cout << "(" << start.first << ", " << start.second << ")\n";
    cout << "Goal: ";
    cout << "(" << goal.first << ", " << goal.second << ")\n";

    string input;
    cout << "Select search algorith (BFS or A*): ";
    cin >> input;
    if (input == "BFS")
    {
        cout << "BFS Results: \n";
        vector<vector<pair<int, int>>> parents = bfs(grid);
        vector<pair<int, int>> path = build_path(parents, start, goal);
        display_path_length(path);
        print_path(path);
        cout << "Visual Path:\n";
        display_solved_grid(grid, path);
    }
    
    else
    {
        cout << "A* Results: \n";
        vector<vector<pair<int, int>>> parents = a_star(grid);
        vector<pair<int, int>> path = build_path(parents, start, goal);
        display_path_length(path);
        print_path(path);
        cout << "Visual Path:\n";
        display_solved_grid(grid, path);
    }

    return 0;
}