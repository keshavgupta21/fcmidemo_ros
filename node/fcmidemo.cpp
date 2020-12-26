#include <iostream>
#include <chrono>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

#include <fcmidemo/fcmi.h>
#include <fcmidemo/gui.h>
#include <fcmidemo/scan.h>

class FCMIGui {
private:
    FCMIDriver fcmi;
    ScanHandler scan;
    GUI gui;

    ros::NodeHandle n;
    ros::Subscriber mapSub;

    int frameCounter;
    std::chrono::system_clock::time_point start, end;
public:
    FCMIGui():
        fcmi("/root/catkin_ws/src/racecar_simulator/bitstreams/top_cmap.bit"),
        scan(fcmi.getOccPtr()), gui(scan.getFrameBuf(), fcmi.getMIPtr()),
        n("~"), frameCounter(0), start(std::chrono::high_resolution_clock::now()) {
        
        std::string scanTopic;
        n.getParam("scan_topic", scanTopic);
        mapSub = n.subscribe(scanTopic, 1, &FCMIGui::updateScanCallback, this);
    }

    void updateScanCallback(const sensor_msgs::LaserScan &scanData) {
        // scan.updateScan(127);
        // fcmi.calcMI();
        // gui.updateWindow();
        frameCounter++;
        if (frameCounter == 100) {
            end = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Last 100 frames @ " << 1000.0 * frameCounter / time << " fps" << std::endl;
            start = std::chrono::high_resolution_clock::now();
            frameCounter = 0;
        }
    }

    /* assume default constructor */
};

int main(int argc, char ** argv) {
    ros::init(argc, argv, "fcmidemo_gui");
    FCMIGui gui;
    ros::spin();
    return 0;
}