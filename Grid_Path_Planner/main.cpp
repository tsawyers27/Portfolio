#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string file_name;
    std::cout << "Enter grid file name: ";
    std::cin >> file_name;

    std::cout << "\n";
    std::cout << "Selected Grid: \n";
    std::cout << "\n";

    std::fstream file(file_name);

    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }

    std::cout << "\n";

    file.close();
    return 0;
}