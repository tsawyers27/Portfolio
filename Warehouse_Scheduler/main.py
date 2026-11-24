import robot
import package
import task
import heapq
import os
import random
import numpy as np
import math


def get_grid(grid):
    robots = []
    packages = []
    delivery_zones = {}

    robot_chars = ['1', '2', '3']

    package_id = 1
    dz_counter = 1
    warehouse = []
    dir = os.path.join("warehouse_grids", grid)
    with open(dir, 'r') as file:
        for line in file:
            row = []
            for char in line:
                if char != '\n':
                    row.append(char)
                    if char == 'S':
                        new_package = package.Package(package_id, "Small", len(warehouse), len(row)-1)
                        package_id += 1
                        packages.append(new_package)
                    elif char == 'M':
                        new_package = package.Package(package_id, "Medium", len(warehouse), len(row)-1)
                        package_id += 1
                        packages.append(new_package)
                    elif char == 'L':
                        new_package = package.Package(package_id, "Large", len(warehouse), len(row)-1)
                        package_id += 1
                        packages.append(new_package)
                    elif char in robot_chars:
                        new_robot = robot.Robot(len(warehouse), len(row)-1, int(char))
                        robots.append(new_robot)
                    elif char == 'D':
                        delivery_zones[dz_counter] = (len(warehouse), len(row)-1)
                        dz_counter += 1

            warehouse.append(row)
    return warehouse, robots, packages, delivery_zones

def get_tasks(packages, delivery_zones):
    tasks = []
    id_counter = 1
    dz = list(delivery_zones.values())
    for package in packages:
        new_task = task.Task(id_counter, package.id, random.randint(1,3), (package.x, package.y), random.choice(dz))
        heapq.heappush(tasks, (new_task.priority, new_task.id, new_task))
        id_counter += 1
    return tasks

def manhattan_dist(robot, task):
    a = np.array([robot.x, robot.y])
    b = np.array([task[2].package_start[0], task[2].package_start[1]])
    return np.sum(np.abs(a - b))

def assign_robot(robots, task):
    shortest = math.inf
    shortest_id = 0
    for robot in robots:
        dist = manhattan_dist(robot, task)
        if dist <= shortest:
            shortest = dist
            shortest_id = robot.id
    return shortest_id

def main():
    warehouse, robots, packages, delivery_zones = get_grid("warehouse_1.txt")
    tasks = get_tasks(packages, delivery_zones)

    temp_task = heapq.heappop(tasks)
    first_assignment = assign_robot(robots, temp_task)
    assigned_robot = next((bot for bot in robots if bot.id == first_assignment), None)

    print(f"Task ID: {temp_task[2].id}")
    print(f"Task Location: {temp_task[2].package_start}")
    print(f"Closest Robot ID: {first_assignment}")
    print(f"Robot Location: ({assigned_robot.x}, {assigned_robot.y})")
    print(f"Manhattan Distance: {manhattan_dist(assigned_robot, temp_task)}")

if __name__ == "__main__":
    main()