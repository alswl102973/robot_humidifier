import math

class RobotPose:
    def __init__(self):
        self.x = 0.0  # cm
        self.y = 0.0
        self.theta = 0.0  # rad

    def move_forward(self, dist_cm):
        self.x += dist_cm * math.cos(self.theta)
        self.y += dist_cm * math.sin(self.theta)

    def rotate(self, angle_rad):
        self.theta += angle_rad