#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

using namespace std;
using namespace ros;

turtlesim::Pose turtle1, my_turtle; // моя черепашка будет догонять управляемую с клавиатуры

const double dbt = 1.0; // сохраняемое расстояние между черепашками

void poseCallback(const turtlesim::Pose::ConstPtr& msg);
void myposeCallback(const turtlesim::Pose::ConstPtr& msg);
void myHunt(Publisher pub);
double dist();

int main(int argc, char **argv){

    init(argc, argv, "my_go");
    NodeHandle vel_n, t_n, my_n;
    Subscriber t_sub, my_sub;
    Publisher vel_pub;

    t_sub = t_n.subscribe("/turtle1/pose", 10, poseCallback);
    my_sub = my_n.subscribe("/my_turtle/pose", 10, myposeCallback);
    vel_pub = vel_n.advertise<geometry_msgs::Twist>("/my_turtle/cmd_vel", 10);

    Rate loop_rate(1);
    while(ok()){
        myHunt(vel_pub);
        spinOnce();
        loop_rate.sleep();
    }
}

void poseCallback(const turtlesim::Pose::ConstPtr& msg){
    turtle1.x = msg->x;
    turtle1.y = msg->y;
    turtle1.theta = msg->theta;
    turtle1.linear_velocity = msg->linear_velocity;
    turtle1.angular_velocity = msg->angular_velocity;
}

void myposeCallback(const turtlesim::Pose::ConstPtr& msg){
    my_turtle.x = msg->x;
    my_turtle.y = msg->y;
    my_turtle.theta = msg->theta;
    my_turtle.linear_velocity = msg->linear_velocity;
    my_turtle.angular_velocity = msg->angular_velocity;
}

void myHunt(Publisher pub){

    geometry_msgs::Twist msg;

    if(dist() >= dbt){
        cout<<"hunting.."<<endl;
        msg.linear.x = 0.3 * dist();
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = 0.3 * (atan2((turtle1.y - my_turtle.y),(turtle1.x - my_turtle.x)) - my_turtle.theta) * 1.5;
        pub.publish(msg);
    }
    else{
        cout<<"stop!"<<endl;
        msg.linear.x = 0;
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = 0;
        pub.publish(msg);
    }
}

double dist(){
    return sqrt(pow(turtle1.x - my_turtle.x, 2) + pow(turtle1.y - my_turtle.y, 2));
}
