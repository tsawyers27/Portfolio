#include <iostream>
#include "robot.h"

// Declare global variables
double dt = 0.1; // time measured in seconds

int main() {
    Robot robot;
    robot.set_pose(1.0, 2.0, 0.0);
    Pose2D pose = robot.get_pose();
    std::cout << "Pose: (" << pose.x << ", " << pose.y << ", " << pose.theta << ")\n";
    return 0;
}