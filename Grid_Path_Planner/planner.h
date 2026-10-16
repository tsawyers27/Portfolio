#ifndef PLANNER_H
#define PLANNER_H

#include <utility>
#include <vector>

std::pair<int, int> get_start(const std::vector<std::vector<char>>& grid, char start);
std::pair<int, int> get_goal(const std::vector<std::vector<char>>& grid, char goal);

double heuristic(const std::pair<int, int>& one, const std::pair<int, int>& two);

std::vector<std::pair<int, int>> build_path(const std::vector<std::vector<std::pair<int, int>>>& parents, 
                                            std::pair<int, int> start, std::pair<int, int> goal);
void print_path(const std::vector<std::pair<int, int>>& path);
void display_path_length(const std::vector<std::pair<int, int>>& path);

std::vector<std::vector<std::pair<int, int>>> bfs(const std::vector<std::vector<char>>& grid);
std::vector<std::vector<std::pair<int, int>>> a_star(const std::vector<std::vector<char>>& grid);

#endif