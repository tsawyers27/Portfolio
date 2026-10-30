#ifndef ROBOT_H
#define ROBOT_H

struct Pose2D {
    double x;
    double y;
    double theta;
};

class Robot {
public:
    // Constructors
    Robot();
    Robot(double x, double y, double theta,
          double size, double width,
          double v_right, double v_left);

    // Getters
    Pose2D get_pose() const;
    double get_v_r() const;
    double get_v_l() const;
    double get_v() const;
    double get_wheel_size() const;
    double get_track_width() const;
    double get_angular_v() const;

    // Setters
    void set_pose(double x, double y, double theta);
    void set_v_r(double v);
    void set_v_l(double v);
    void set_wheel_size(double size);
    void set_track_width(double width);

    // Pose transformation
    Pose2D transform_pose(Pose2D pose, double dt);

private:
    Pose2D pose_;
    double wheel_size_;
    double track_width_;
    double v_r_;
    double v_l_;
};

#endif