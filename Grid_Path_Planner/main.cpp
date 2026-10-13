#include "grid.h"

int main()
{
    std::vector<std::vector<char>> grid = read_grid();
    display_grid(grid);
    return 0;
}