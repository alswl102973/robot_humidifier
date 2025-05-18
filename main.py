import time
from env import MapVisualizer
from sensors import LidarSensor
from localization import RobotPose
import math

def main():
    visualizer = MapVisualizer()
    sensor = LidarSensor(port='COM4')
    robot = RobotPose()

    try:
        while True:
            data = sensor.read_lidar_data()
            if data:
                distance, angle_rad = data
                print(f"[LIDAR] 거리: {distance:.1f} mm, 각도: {math.degrees(angle_rad):.1f}도")
                visualizer.update_map(distance, angle_rad, robot)
                visualizer.draw()
            time.sleep(0.01)

    except KeyboardInterrupt:
        print("종료됨")

if __name__ == '__main__':
    main()
