#include "unicorn/reversing_state.h"

REVERSINGState::REVERSINGState(ros::NodeHandle node) : move_base_clt_("move_base", false)
{
    cmd_vel_pub_ = node.advertise<geometry_msgs::Twist>("/unicorn/cmd_vel", 0);
    move_base_cancel_pub_ = node.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 0);
    rear_lidar_sub_ = node.subscribe("/RIO_lidarBackPublisher_avgDist", 0, &REVERSINGState::rearLidarCallback, this);
    state_identifier_ = STATE_REVERSING;
    at_desired_distance_ = false;
    man_cmd_vel_.angular.z = 0;
    man_cmd_vel_.linear.x = -0.1;
}

REVERSINGState::~REVERSINGState()
{
    
}

void REVERSINGState::cancelGoal()
{
    actionlib_msgs::GoalID cancel_all;
    move_base_cancel_pub_.publish(cancel_all);
    ROS_INFO("[UNICORN State Machine] Canceling move_base goal");
}

Command REVERSINGState::run()
{

    Command new_cmd;
    new_cmd.state = STATE_IDLE;
    ROS_INFO("[UNICORN State Machine] Reversing towards to refuse bin...");
    ros::Rate rate(50);
    while (ros::ok())
    {
        ros::spinOnce();
//        ROS_INFO("[UNICORN State Machine] Current velocity: %f", current_vel_);
//        ROS_INFO("[UNICORN State Machine] Current yaw: %f", current_yaw_);
        if(command.state != -1)
        {
            cancelGoal();
            ROS_INFO("[UNICORN State Machine] New command was issued, halting navigation.");
            return command;
        }
        if(at_desired_distance_)
        {
            man_cmd_vel_.linear.x = 0.0;
            cancelGoal();
            cmd_vel_pub_.publish(man_cmd_vel_);
            ROS_INFO("[UNICORN State Machine] Robot reached desired distance exiting with new state set to LIFT");
            new_cmd.state = STATE_LIFT;
            return new_cmd;
        }
        cmd_vel_pub_.publish(man_cmd_vel_);
        rate.sleep();
    }
}

void REVERSINGState::rearLidarCallback(const std_msgs::Float32 &msg)
{
	ROS_INFO("Current Distance: %f", msg.data);
    if(msg.data <= desired_distance_)
    {
        at_desired_distance_ = true;
    }
}

void REVERSINGState::odomCallback(const nav_msgs::Odometry &msg)
{
	tf::Pose pose;
	tf::poseMsgToTF(msg.pose.pose, pose);
	current_yaw_ = tf::getYaw(pose.getRotation());
	current_vel_ = msg.twist.twist.linear.x;
}