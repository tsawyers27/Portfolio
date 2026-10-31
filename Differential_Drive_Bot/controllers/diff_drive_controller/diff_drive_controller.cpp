
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>

#include <iostream>
#include <queue>
#include <limits>

#include "../../robot.h"

// Scheduled wheel commands
struct Command 
{
    double vel_r;      // m/s
    double vel_l;      // m/s
    double exec_time;  // seconds
};

int main(int argc, char **argv) 
{

    // Create the Webots robot and your mathematical model
    webots::Robot webots_robot;

    // Approximate e-puck dimensions:
    // wheel diameter = 4.1 cm
    // track width = 0.052 m
    ::Robot model(0.0, 0.0, 0.0, 4.1, 0.052, 0.0, 0.0);

    // Webots timestep in milliseconds
    int timeStep = static_cast<int>(webots_robot.getBasicTimeStep());

    double dt = timeStep / 1000.0;

    // Wheel radius in meters
    const double wheel_radius = 0.0205;

    // Simulation duration in seconds
    const double max_t = 5.0;

    // Retrieve the e-puck motors
    webots::Motor* left_motor = webots_robot.getMotor("left wheel motor");

    webots::Motor* right_motor = webots_robot.getMotor("right wheel motor");

    // Configure motors for continuous velocity control
    left_motor->setPosition(std::numeric_limits<double>::infinity());

    right_motor->setPosition(std::numeric_limits<double>::infinity());

    left_motor->setVelocity(0.0);
    right_motor->setVelocity(0.0);

    // Pre-planned wheel commands
    std::queue<Command> commands;

    commands.push({0.05, 0.05, 0.0});  // Straight
    commands.push({0.02, 0.08, 1.0});  // Turn clockwise
    commands.push({0.08, 0.02, 2.0});  // Turn counterclockwise
    commands.push({-0.04, 0.04, 3.0}); // Rotate in place
    commands.push({0.0, 0.0, 4.0});    // Stop

    std::cout << "Beginning Webots simulation...\n";

    // Simulation loop
    while (webots_robot.getTime() < max_t) {

        double t = webots_robot.getTime();

        // Apply commands scheduled for the current time
        while (!commands.empty() && t >= commands.front().exec_time) 
        {

            Command com = commands.front();
            commands.pop();

            model.set_v_r(com.vel_r);
            model.set_v_l(com.vel_l);

            std::cout << "Time: " << t << " | Right: " << com.vel_r << " | Left: " << com.vel_l << '\n';
        }

        // Convert wheel linear speeds to angular speeds
        double omega_left = model.get_v_l() / wheel_radius;

        double omega_right = model.get_v_r() / wheel_radius;

        // Send the commands to Webots motors
        left_motor->setVelocity(omega_left);
        right_motor->setVelocity(omega_right);

        // Update your mathematical model
        Pose2D pose = model.get_pose();
        model.transform_pose(pose, dt);

        // Advance Webots by one simulation step
        if (webots_robot.step(timeStep) == -1) 
        {
            break;
        }
    }

    // Stop the motors when the scheduled run finishes
    left_motor->setVelocity(0.0);
    right_motor->setVelocity(0.0);

    Pose2D final_pose = model.get_pose();

    std::cout << "Simulation completed.\n";
    std::cout << "Predicted final pose: (" << final_pose.x << ", " << final_pose.y << ", " << final_pose.theta << ")\n";

    return 0;
}
