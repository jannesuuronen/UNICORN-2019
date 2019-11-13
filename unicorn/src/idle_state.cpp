#include "unicorn/idle_state.h"

IDLEState::IDLEState()
{
   state_identifier_ = STATE_IDLE;
}

IDLEState::~IDLEState()
{
}

Command IDLEState::run()
{
    ROS_INFO("Waiting for new command...");
    ros::Rate rate(50);
    while(command.state == -1)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("New command has been recieved: %d", command.state);
    return command;
}