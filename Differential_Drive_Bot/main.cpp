#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <matplot/matplot.h>
#include "robot.h"

using namespace std;

struct Command {
    double vel_r;
    double vel_l;
    double exec_time;
};

// Declare global variables
double dt; // time measured in seconds
double t = 0;
double max_t;

void print_path(queue<Command> q)
{
    vector<Command> poses;
    queue<Command> temp_q = q;

    while(!temp_q.empty())
    {
        poses.push_back(temp_q.front());
        temp_q.pop();
    }

    for (Command pose : poses)
    {
        cout << "(" << pose.vel_r << ", " << pose.vel_l << ", " << pose.exec_time << ")\n";
    }
}

queue<Command> set_commands()
{
    // Get user inputs for robot path and execution timing
    cout << "The following prompts will create the robot path and timing.\n";
    cout << "Commands should be entered as three floating point numbers separated by spaces.\n";
    queue<Command> commands;
    string input;
    while (input != "Done")
    {
        Command temp;
        double v_r_temp;
        double v_l_temp;
        double exec_time_temp;
        
        cout << "Please enter a command in the form of (right wheel vel, left wheel vel, execution time):\n";
        cin >> v_r_temp >> v_l_temp >> exec_time_temp;
        temp.vel_r = v_r_temp;
        temp.vel_l = v_l_temp;
        temp.exec_time = exec_time_temp;
        commands.push(temp);

        cout << "Please type 'Done' if finished, type '0' otherwise\n";
        cin >> input;
    }
    return commands;
}

void set_globals()
{
    // Set global variable for simulation step size
    cout << "Please enter a simulation step size as a decimal (measured in seconds):\n";
    cin >> dt;
    cout << "Simulation step size saved as " << dt << " seconds.\n";
    cout << "Please enter a maximum simulation length in seconds:\n";
    cin >> max_t;
    cout << "Maximum simulation length is " << max_t << " seconds.\n";
}

int main() {
    set_globals();

    queue<Command> commands = set_commands();

    // Print out path to user
    cout << "Robot Path:\n";
    print_path(commands);

    // Instantiate variable for robot object
    double x;
    double y;
    double theta;
    double size;
    double width;
    double starting_v_r;
    double starting_v_l;

    // Take user input to feed to robot constructor
    cout << "Please enter a starting values for the robot.\n";
    cout << "Enter values as decimals in the form of (x, y, theta, wheel size, track width, starting right vel, starting left vel)\n";
    cin >> x >> y >> theta >> size >> width >> starting_v_r  >> starting_v_l;

    // Notify user of simulation start and print path
    cout << "Beginning simulation...\n";

    // Instantiate robot object
    Robot robot(x, y, theta, size, width, starting_v_r, starting_v_l);

    // Instantiate vector to hold robot poses for display later
    vector<double> pose_x;
    vector<double> pose_y;

    // Add starting position to graph vectors
    pose_x.push_back(robot.get_pose().x);
    pose_y.push_back(robot.get_pose().y);

    // Begin simulation
    while (t < max_t)
    {
        while (!commands.empty() && t >= commands.front().exec_time)
        {
            Command com = commands.front();
            commands.pop();

            robot.set_v_r(com.vel_r);
            robot.set_v_l(com.vel_l);
        }

        Pose2D pose = robot.get_pose();
        Pose2D new_pose = robot.transform_pose(pose, dt);

        pose_x.push_back(new_pose.x);
        pose_y.push_back(new_pose.y);
        
        t += dt;
    }

    // Notify user that simulation is complete
    cout << "Simulation completed.\n";
    cout << "Final Pose: (" << robot.get_pose().x << ", " << robot.get_pose().y << ", " << robot.get_pose().theta << ")\n";

    // Create and display graph
    matplot::plot(pose_x, pose_y);
    matplot::xlabel("X Position");
    matplot::ylabel("Y Position");
    matplot::title("Robot Poses");
    matplot::show();
    matplot::wait();
   
    return 0;
}