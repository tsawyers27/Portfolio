#include "grid.h"
#include "planner.h"
#include <iostream>

int main()
{
    std::vector<std::vector<char>> grid = read_grid();
    display_grid(grid);
    std::pair<int, int> start = getStart(grid, 'S');
    std::cout << "(" << start.first << ", " << start.second << ")";
    return 0;
}