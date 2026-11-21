import robot
import package
import task
import os

def print_grid(grid):
    dir = os.path.join("warehouse_grids", grid)
    with open(dir, 'r') as file:
        for line in file:
            print(line.strip())


def main():
    print_grid("warehouse_1.txt")

if __name__ == "__main__":
    main()