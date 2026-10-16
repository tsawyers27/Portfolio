#include "planner.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

int moves[4][2] = {{0,1}, {1, 0}, {0, -1}, {-1, 0}};

struct Node
{
    pair<int, int> coords;
    double g, h;
    double f() const { return g + h; }
};

struct CompareNode
{
    bool operator()(const Node& a, const Node& b)
    {
        return a.f() > b.f();
    }
};

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

double heuristic(const pair<int, int>& one, const pair<int, int>& two)
{
    double x1 = static_cast<double>(one.first);
    double x2 = static_cast<double>(two.first);
    double y1 = static_cast<double>(one.second);
    double y2 = static_cast<double>(two.second);

    return abs(x1 - x2) + abs(y1 - y2);
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
        if (prev.first == -1 && prev.second == -1)
        {
            path.clear();
            return path;
        }
        else
        {
            curr = prev;
        }
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
        cout << "(" << point.first << "," << point.second << ") ";
    }
}

void display_path_length(const vector<pair<int, int>>& path)
{
    cout << "Path Length: " << path.size() << "\n";
}

vector<vector<pair<int, int>>> bfs(const vector<vector<char>>& grid)
{
    size_t rows {grid.size()};
    size_t cols {grid[0].size()};
    
    pair<int, int> start = get_start(grid, 'S');
    pair<int, int> goal = get_goal(grid, 'G');

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    queue<pair<int, int>> q;
    visited[start.first][start.second] = true;
    q.push(start);
    int explored {0};

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
        explored += 1;

        for (const auto& move : moves)
        {
            int nr = curr.first + move[0];
            int nc = curr.second + move[1];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && 
                grid[nr][nc] != '#' && !visited[nr][nc])
                {
                    visited[nr][nc] = true;
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

vector<vector<pair<int, int>>> a_star(const vector<vector<char>>& grid)
{
    // Instantiate indices
    size_t rows {grid.size()};
    size_t cols {grid[0].size()};
    
    // Get start and goal coordinates
    pair<int, int> start = get_start(grid, 'S');
    pair<int, int> goal = get_goal(grid, 'G');

    // Instantiate parent map and g-score tracking for path construction
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    vector<vector<double>> lowest_g(rows, vector<double>(cols, 1000.0));

    // Instantiate open and closed sets for frontier tracking and logic
    priority_queue<Node, vector<Node>, CompareNode> open;
    set<pair<int, int>> closed;

    // Add start to open list
    double cost = heuristic(start, goal);
    open.push({start, 0, cost});
    // Instantiate node exploration accumulator
    int explored {0};

    // Instantiate var to identify if goal is reached
    bool found = false;

    while (!open.empty()) 
    {
        // Pick node with lowest f score from open list
        Node curr = open.top();
        open.pop();

        // Check if current node is the goal
        if (curr.coords == goal)
        {
            found = true;
            break;
        }
        
        // Check if node already visited
        if (closed.find(curr.coords) != closed.end())
        {
            continue;
        }
        else
        {
            closed.insert(curr.coords);
            explored += 1;
        }

        // For each neighbor calculate cost to that neighbor
        for (const auto& move : moves)
        {
            int nr = curr.coords.first + move[0];
            int nc = curr.coords.second + move[1];
            double g = curr.g + 1;
            double h = heuristic({nr, nc}, goal);

            // If path is cheaper, update neighbor cost and add to open list
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != '#' &&
            closed.find({nr, nc}) == closed.end() && g < lowest_g[nr][nc])
                {
                    parent[nr][nc] = curr.coords;
                    lowest_g[nr][nc] = g;
                    open.push({{nr, nc}, g, h});
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