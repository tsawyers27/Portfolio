#include "grid.h"
#include <iostream>
#include <fstream>

std::vector<std::vector<char>> read_grid()
{
    std::vector<std::vector<char>> grid;

    std::string file_name;
    std::cout << "Enter grid file name: ";
    std::cin >> file_name;

    std::fstream file(file_name);

    char ch;
    std::vector<char> row;
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

void display_grid(const std::vector<std::vector<char>>& grid)
{
        for (const auto& row : grid)
    {
        for (char elem : row)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}