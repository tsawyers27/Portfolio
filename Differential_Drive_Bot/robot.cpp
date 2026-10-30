#include "robot.h"
#include <cmath>

// Constructors
Robot::Robot() {
    pose_ = {0.0, 0.0, 0.0};
    wheel_size_ = 10.0;
    track_width_ = 10.0;
    v_r_ = 0.0;
    v_l_ = 0.0;
}

Robot::Robot(double x, double y, double theta, double size, double width, double v_right, double v_left) {
    pose_ = {x, y, theta};
    wheel_size_ = size;
    track_width_ = width;
    v_r_ = v_right;
    v_l_ = v_left;
}

// Getters
Pose2D Robot::get_pose() const {
    return pose_;
}

double Robot::get_v_r() const {
    return v_r_;
}

double Robot::get_v_l() const {
    return v_l_;
}

double Robot::get_v() const {
    return (v_r_ + v_l_) / 2;
}

double Robot::get_wheel_size() const {
    return wheel_size_;
}

double Robot::get_track_width() const {
    return track_width_;
}

double Robot::get_angular_v() const {
    return (v_r_ - v_l_) / track_width_;
}

// Setters
void Robot::set_pose(double x, double y, double theta) {
    pose_ = {x, y, theta};
}

void Robot::set_v_r(double v) {
    v_r_ = v;
}

void Robot::set_v_l(double v) {
    v_l_ = v;
}

void Robot::set_wheel_size(double size) {
    wheel_size_ = size;
}

void Robot::set_track_width(double width) {
    track_width_ = width;
}

// Pose transformation
Pose2D Robot::transform_pose(Pose2D pose, double dt) {
    double x_prime = pose.x + (get_v() * std::cos(pose.theta) * dt);
    double y_prime = pose.y + (get_v() * std::sin(pose.theta) * dt);
    double theta_prime = pose.theta + (get_angular_v() * dt);

    Pose2D new_pose = {x_prime, y_prime, theta_prime};
    pose_ = new_pose;
    return new_pose;
}