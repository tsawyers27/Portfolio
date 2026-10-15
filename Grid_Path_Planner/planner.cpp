#include "planner.h"
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

int moves[4][2] = {{0,1}, {1, 0}, {0, -1}, {-1, 0}};

pair<int, int> get_start(const vector<vector<char>>& grid, char start)
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

pair<int, int> get_goal(const vector<vector<char>>& grid, char goal)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for(int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == goal)
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

vector<pair<int, int>> build_path(const vector<vector<pair<int, int>>>& parents,
                                      pair<int, int> start, pair<int, int> goal)
{
    // Initialize path for tracking
    vector<pair<int, int>> path;

    //Begin at goal coordinates
    pair<int, int> curr = goal;
    
    while (curr != start)
    {
        path.push_back(curr);
        pair<int, int> prev = parents[curr.first][curr.second];
        curr = prev;
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<pair<int, int>>& path)
{
    cout << "Shortest Path:\n";
    for (const auto& point : path)
    {
        cout << "(" << point.first << "," << point.second << ")\n";
    }
}

void display_path_length(const vector<pair<int, int>>& path)
{
    cout << "Path Length: " << path.size() << "\n";
}

vector<vector<pair<int, int>>> bfs(const vector<vector<char>>& grid)
{
    int rows {grid.size()};
    int cols {grid[0].size()};
    
    pair<int, int> start = get_start(grid, 'S');
    pair<int, int> goal = get_goal(grid, 'G');

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    queue<pair<int, int>> q;
    visited[start.first][start.second] = true;
    q.push(start);
    int explored {1};

    bool found = false;

    while (!q.empty()) 
    {
        pair<int, int> curr = q.front();
        q.pop();

        if (curr == goal)
        {
            found = true;
            break;
        }

        for (const auto& move : moves)
        {
            int nr = curr.first + move[0];
            int nc = curr.second + move[1];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && 
                grid[nr][nc] != '#' && !visited[nr][nc])
                {
                    visited[nr][nc] = true;
                    explored += 1;
                    parent[nr][nc] = curr;
                    q.push({nr, nc});
                }
        }
    }

    if (found)
    {
        cout << "Shortest path found!\n";
        cout << "Nodes Explored: " << explored << "\n";
        return parent;
    }
    else
    {
        cout << "No path exists.\n";
        cout << "Nodes Explored: " << explored << "\n";
        return parent;
    }

}