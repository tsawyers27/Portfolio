# Grid Path Planner — BFS and A*

A C++ command-line application that finds and visualizes shortest paths through text-based mazes using **breadth-first search (BFS)** or **A-Star search**. The project compares an uninformed search strategy against a heuristic-guided strategy using the same grid, movement rules, and path-reconstruction interface.

This was developed as a small, independently implemented portfolio project to practice C++ data structures, graph-search algorithms, algorithm design, debugging, and evaluating search efficiency. It is deliberately a focused path-planning exercise rather than a general-purpose robotics navigation stack.

## Features

- Loads a maze from a text file supplied through an interactive prompt.
- Supports **BFS** and **A***, selectable at runtime.
- Finds shortest paths on a grid with four-directional, unit-cost movement.
- Prints the starting position, goal position, search result, number of nodes explored, and the reconstructed path.
- Prints a copy of the maze with the chosen route marked by `*`, preserving `S` and `G`.
- Includes three test mazes with increasing size and complexity.
- Detects when a goal cannot be reached and avoids visualizing an invalid path.

## Requirements

- A C++ compiler with **C++17** support (for example, GCC or Clang)
- Standard C++ library; **no third-party libraries required**
- Optional: **CMake** for configuring and building the project if your repository includes a `CMakeLists.txt`

## Project Files

| File | Purpose |
| --- | --- |
| `main.cpp` | Interactive entry point; selects BFS or A* and displays results |
| `grid.cpp`, `grid.h` | Reads maze data and displays original/solved grids |
| `planner.cpp`, `planner.h` | BFS, A*, Manhattan heuristic, parent-based path reconstruction, and path reporting |
| `grid_1.txt` | Small/easy test maze (9 × 13) |
| `grid_2.txt` | Medium test maze (17 × 25) |
| `grid_3.txt` | Larger/harder test maze (25 × 39) |

## Setup and Build

Clone the repository (or place the source files and maze files in one directory), then open a terminal in the project directory.

### Build directly with a compiler

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp grid.cpp planner.cpp -o grid_planner
```

Run on Linux or macOS:

```bash
./grid_planner
```

On Windows, compile using a suitable installed C++ toolchain and run the resulting executable (for example, `grid_planner.exe`).

### Build with CMake (if `CMakeLists.txt` is included)

```bash
cmake -S . -B build
cmake --build build
```

Launch the executable produced in `build/` (its exact location can vary by generator and platform). The project was developed with CMake tooling in VS Code; the direct compiler command above also works without a CMake configuration.

**Working directory:** Run the program from a directory where the grid text files can be resolved, or enter a relative/absolute path to the desired grid file. The program reads the path from standard input rather than accepting command-line flags.

## Usage

After launching, enter a maze file and select an algorithm:

```text
Enter grid file name: grid_1.txt
Select search algorith (BFS or A*): A*
```

Enter `BFS` exactly to select breadth-first search; the current menu uses the A* branch for any other input.

The application reports whether a route was found, its sequence of `(row, column)` coordinates, the number of explored nodes, and an ASCII visualization of the solved maze. `S` marks the start, `G` the goal, `#` walls, `.` traversable space, and `*` the route. Coordinates are zero-indexed.

**Path length convention:** The application's displayed `Path Length` is the **number of coordinates in the returned path**, including both `S` and `G`. The number of moves is one less than that value.

## How the Algorithms Work

### Breadth-first search (BFS)

BFS uses a first-in, first-out `std::queue` to explore reachable cells in order of increasing distance from the starting position. A two-dimensional `visited` grid prevents repeated discovery of the same cell. Since every movement costs one unit, BFS produces a shortest path when a route exists.

### A* search

A* uses a `std::priority_queue` with a custom comparator to expand candidates with the lowest estimated total cost:

```text
f(n) = g(n) + h(n)
```

- `g(n)` is the accumulated movement cost from the start to node `n`.
- `h(n)` is the Manhattan-distance estimate from `n` to the goal.

A separate two-dimensional `lowest_g` grid records the least expensive route discovered so far to each coordinate. A neighbor's parent and recorded cost are updated only when a **strictly cheaper** route is found, and that candidate is added to the priority queue. A `std::set` of closed coordinates prevents already-expanded cells from being processed again.

Manhattan distance is admissible and consistent for this project's four-directional, unit-cost grid, so A* can find shortest paths without reopening closed nodes under these movement rules.

### Path reconstruction

Rather than storing a full path in each search candidate, both algorithms share a two-dimensional **parent grid**. Each traversable cell records its predecessor on the selected route. Once the goal is reached, `build_path()` follows parent coordinates backward to `S` and reverses the result. An empty reconstructed path signals an unavailable route, and the caller skips its visualization.

## Design Considerations

The following choices were made during implementation and debugging:

- **A grid as a graph:** A `std::vector<std::vector<char>>` stores the environment, while neighboring cells implicitly define graph edges; a separate adjacency list is unnecessary for fixed, local movement.
- **Four directions and uniform cost:** The same right/down/left/up offsets apply to both algorithms. This keeps the comparison focused on *search strategy*, not differing movement models or terrain weights.
- **Algorithm-specific frontiers:** BFS uses a FIFO queue; A* uses a min-priority queue implemented with `std::priority_queue` and a custom `Node` comparator based on `f`.
- **Distinct roles for frontier, costs, and parents:** The priority queue determines the next candidate to consider; `lowest_g` determines whether a proposed route improves a coordinate; `parent` preserves the chosen route. Merely changing the queue order would not make parent updates correct.
- **Closed-set timing:** A candidate becomes closed **when expanded**, not when first discovered. For this consistent heuristic, closed cells do not need to be reopened.
- **No decrease-key dependency:** Improved candidates can be inserted into the priority queue without modifying existing entries. Already-closed entries are skipped when popped.
- **Compact path storage:** Storing one predecessor per cell avoids copying entire paths into queued nodes and makes reconstruction reusable across BFS and A*.
- **Comparable exploration metrics:** Both algorithms count nodes actually **expanded**, excluding the goal node, rather than comparing nodes discovered in one algorithm with nodes expanded in the other.
- **Practical scope:** Maze dimensions and input files are small and controlled; the current implementation favors clarity over extensive input validation and generalized cost models.

## Validation

The three supplied mazes were checked using both algorithms. For each maze, BFS and A* produced a traversable, orthogonally connected path with the same shortest movement count; those lengths were also checked against an independent BFS implementation.

| Test maze | Grid size | BFS optimal moves | A* optimal moves |
| --- | --- | ---: | ---: |
| `grid_1.txt` | 9 × 13 | 16 | 16 |
| `grid_2.txt` | 17 × 25 | 108 | 108 |
| `grid_3.txt` | 25 × 39 | 228 | 228 |

These are results from the earlier executed validation of the project's core algorithms. Subsequent edits refined node-counting and unsuccessful-path handling; the table is not a claim of a separate final-version benchmark run. Your displayed path lengths are **17, 109, and 229 coordinates**, respectively.

The `Nodes Explored` statistic can be used to compare search effort on the same maze. A* is heuristic-guided but is **not guaranteed to expand fewer nodes on every possible maze**; results depend on the map, heuristic, and priority ties.

## Scope and Limitations

- Movement is limited to four orthogonal directions; there is no diagonal movement.
- All valid moves have cost 1; there are no weighted terrain cells.
- The A* implementation initializes undiscovered `g` costs to `1000.0`, suitable for these supplied test maps but not an unrestricted path-length bound.
- The input routine assumes well-formed, rectangular maze files with valid start/goal markers. It does not comprehensively validate missing files, missing markers, ragged rows, or different line endings.
- The program uses interactive prompts rather than command-line options, batch execution, or a graphical interface.
- Exploration counts describe the number of expanded cells, not wall-clock performance.

## Project Scope

This project demonstrates **independently implemented shortest-path search in C++**, from file parsing and data representation through search invariants, debugging, path reconstruction, and validation. It is intended as a compact algorithm-and-software-engineering portfolio project and a foundation for more advanced robotics planning work.
