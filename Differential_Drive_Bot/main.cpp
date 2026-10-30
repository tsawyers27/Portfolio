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

int main() {
    // Set global variable for simulation step size
    cout << "Please enter a simulation step size as a decimal (measured in seconds):\n";
    cin >> dt;
    cout << "Simulation step size saved as " << dt << " seconds.\n";
    cout << "Please enter a maximum simulation length in seconds:\n";
    cin >> max_t;
    cout << "Maximum simulation length is " << max_t << " seconds.\n";

    // Get user inputs for robot path and execution timing
    cout << "The following prompts will create the robot path and timing.\n";
    queue<Command> commands;
    string input;
    while (input != "Done")
    {
        Command temp;
        double num_temp;
        
        cout << "Please enter a right wheel velocity:\n";
        cin >> num_temp;
        temp.vel_r = num_temp;
        cout << "Please enter a left wheel velocity:\n";
        cin >> num_temp;
        temp.vel_l = num_temp;
        cout << "Please enter an execution time in seconds:\n";
        cin >> num_temp;
        temp.exec_time = num_temp;
        commands.push(temp);

        cout << "Please type 'Done' if finished, type '0' otherwise\n";
        cin >> input;
    }

    // Instantiate variable for robot object
    double x;
    double y;
    double theta;
    double size;
    double width;
    double starting_v_r;
    double starting_v_l;

    // Take user input to feed to robot constructor
    cout << "Please enter a starting x-coordinate as a decimal:\n";
    cin >> x;
    cout << "Please enter a starting y-coordinate as a decimal:\n";
    cin >> y;
    cout << "Please enter a starting heading as a decimal: \n";
    cin >> theta;
    cout << "Please enter a wheel size in cm:\n";
    cin >> size;
    cout << "Please enter a track width in m:\n";
    cin >> width;
    cout << "Please enter a starting right wheel velocity in m/s^2:\n";
    cin >> starting_v_r;
    cout << "Please enter a starting left wheel velocity in m/s^2:\n";
    cin >> starting_v_l;

    // Notify user of simulation start and print path
    cout << "Beginning simulation...\n";
    vector<Command> poses;
    queue<Command> temp_q = commands;

    while(!temp_q.empty())
    {
        poses.push_back(temp_q.front());
        temp_q.pop();
    }

    for (Command pose : poses)
    {
        cout << "(" << pose.vel_r << ", " << pose.vel_l << ", " << pose.exec_time << ")\n";
    }

    // Instantiate robot object
    Robot robot(x, y, theta, size, width, starting_v_r, starting_v_l);

    // Instantiate vector to hold robot poses for display later
    vector<double> pose_x;
    vector<double> pose_y;

    // Begin simulation
    Command com = commands.front();
    commands.pop();

    // Add initial pose to path vector
    pose_x.push_back(robot.get_pose().x);
    pose_y.push_back(robot.get_pose().y);

    while (true)
    {
        if (t >= max_t)
        {
            break;
        }

        Pose2D pose = robot.get_pose();
        if (t >= com.exec_time)
        {
            robot.set_v_r(com.vel_r);
            robot.set_v_l(com.vel_l);

            if (!commands.empty())
            {
                com = commands.front();
                commands.pop();
            }
            else
            {
                robot.set_v_r(0.0);
                robot.set_v_l(0.0);
            }
        }
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
    matplot::save("robot_path.png");
    matplot::show();
   
    return 0;
}