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
    cout << "(" << start.first << ", " << start.second << ")\n";
    cout << "(" << goal.first << ", " << goal.second << ")\n";
    vector<vector<pair<int, int>>> parents = bfs(grid);
    vector<pair<int, int>> path = build_path(parents, start, goal);
    display_path_length(path);
    print_path(path);
    return 0;
}