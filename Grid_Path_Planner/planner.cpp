#include "planner.h"

char start {'S'};

std::pair<int, int> getStart(const std::vector<std::vector<char>>& grid, char start)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for(int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == start)
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}