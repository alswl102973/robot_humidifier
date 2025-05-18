import serial
import math

class LidarSensor:
    def __init__(self, port='COM4', baudrate=115200):
        self.ser = serial.Serial(port, baudrate=baudrate, timeout=1)

    def read_lidar_data(self):
        try:
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            if line and ',' in line:
                parts = line.split(',')
                if len(parts) != 2:
                    return None
                distance = float(parts[0])
                angle_deg = float(parts[1])
                if 0 < distance < 8000 and 0 <= angle_deg <= 360:
                    return distance, math.radians(angle_deg)
        except (ValueError, IndexError):
            pass
        return None