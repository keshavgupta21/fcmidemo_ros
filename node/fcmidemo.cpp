#include <iostream>
#include <chrono>

#include <ros/ros.h>

#include <std_msgs/Float32MultiArray.h>
#include <tf2/impl/utils.h>
#include <tf2/LinearMath/Quaternion.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <racecar_simulator/pose_2d.hpp>

#include <fcmidemo/fcmi.h>
#include <fcmidemo/gui.h>
#include <fcmidemo/scan.h>

class FCMIGui {
private:
    FCMIDriver fcmi;
    ScanHandler scan;
    GUI gui;

    ros::NodeHandle n;
    ros::Subscriber mapSub, scanPoseSub;
    ros::Publisher drivePub;

    int frameCounter;
    std::chrono::system_clock::time_point start;

    float maxDriveSpeed, maxDriveAngle;
    float driveSpeed, driveAngle;
    float delDriveSpeed, delDriveAngle;
    float driveDecay;

    const Uint8 *keyStates;

public:
    FCMIGui():
        fcmi("/root/catkin_ws/src/racecar_simulator/bitstreams/top_cmap.bit"),
        scan(fcmi.getOccPtr()), gui(scan.getFrameBuf(), fcmi.getMIPtr()),
        n("~"), frameCounter(0), start(std::chrono::high_resolution_clock::now()) {
        
        std::string scanTopic, scanPoseTopic, driveTopic;
        n.getParam("scan_topic", scanTopic);
        mapSub = n.subscribe(scanTopic, 1, &FCMIGui::updateScanCallback, this);

        n.getParam("scanpose_topic", scanPoseTopic);
        scanPoseSub = n.subscribe(scanPoseTopic, 1, &FCMIGui::updateScanPoseCallback, this);

        n.getParam("drive_topic", driveTopic);
        drivePub = n.advertise<ackermann_msgs::AckermannDriveStamped>(driveTopic, 1);

        n.getParam("max_speed", maxDriveSpeed);
        n.getParam("max_steering_angle", maxDriveAngle);
        driveSpeed = 0.0;
        driveAngle = 0.0;
        delDriveSpeed = 0.1;
        delDriveAngle = 0.05;
        driveDecay = 0.95;

        keyStates = SDL_GetKeyboardState(nullptr);
    }

    void updateScanPoseCallback(const std_msgs::Float32MultiArray &scanPoseData) {
        scan.setScanPose((int)(scanPoseData.data[0]), (int)(scanPoseData.data[1]), scanPoseData.data[2]);
        gui.setScanPose((int)(scanPoseData.data[0]), (int)(scanPoseData.data[1]), scanPoseData.data[2]);
        scan.setResolution(scanPoseData.data[3]);
    }

    void updateScanCallback(const sensor_msgs::LaserScan &scanData) {
        scan.updateScan(scanData);
        fcmi.calcMI();
        gui.updateWindow();
        frameCounter++;
        if (frameCounter == 100) {
            std::chrono::system_clock::time_point end = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Last 100 frames @ " << 1000.0 * frameCounter / time << " fps" << std::endl;
            start = std::chrono::high_resolution_clock::now();
            frameCounter = 0;
        }

        // check for keypress
        SDL_PumpEvents();
        if (keyStates[SDL_SCANCODE_UP]) {
            driveSpeed = std::min(maxDriveSpeed, driveSpeed + delDriveSpeed);
        }
        if (keyStates[SDL_SCANCODE_DOWN]) {
            driveSpeed = std::max(-maxDriveSpeed, driveSpeed - delDriveSpeed);
        }
        if (keyStates[SDL_SCANCODE_LEFT]) {
            driveAngle = std::max(-maxDriveAngle, driveAngle + delDriveAngle);
        }
        if (keyStates[SDL_SCANCODE_RIGHT]) {
            driveAngle = std::min(maxDriveAngle, driveAngle - delDriveAngle);
        } 
        // publish a drive message
        driveSpeed *= driveDecay;
        driveAngle *= driveDecay;
        ackermann_msgs::AckermannDriveStamped driveMsg;
        driveMsg.drive.speed = driveSpeed;
        driveMsg.drive.steering_angle = driveAngle;
        drivePub.publish(driveMsg);
    }

    /* assume default destructor */
};

int main(int argc, char ** argv) {
    ros::init(argc, argv, "fcmidemo_gui");
    FCMIGui gui;
    ros::spin();
    return 0;
}