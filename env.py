# 수정된 MapVisualizer 클래스 (aspect 비율 고정 추가)
import matplotlib.pyplot as plt
import numpy as np
import math
import heapq

class MapVisualizer:
    def __init__(self, size=800):
        self.size = size
        self.origin = size // 2
        self.grid = np.zeros((size, size), dtype=np.uint8)
        self.points = []
        self.goal = None
        self.path = []
        self.lines = []  # 파란색 거리 선용 저장 리스트

        plt.ion()
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(0, size)
        self.ax.set_ylim(0, size)
        self.ax.set_facecolor('white')
        self.ax.grid(True)
        self.ax.axhline(self.origin, color='gray', linestyle='--')
        self.ax.axvline(self.origin, color='gray', linestyle='--')
        self.ax.set_xlabel("X (cm)")
        self.ax.set_ylabel("Y (cm)")
        self.ax.set_aspect('equal')  # ✅ 비율 고정
        self.scatter = self.ax.scatter([], [], c='red', s=5)

        self.fig.canvas.mpl_connect('button_press_event', self.on_click)

    def on_click(self, event):
        if event.dblclick:  # 더블클릭일 때만 반응
            gx, gy = int(event.xdata), int(event.ydata)
            if 0 <= gx < self.size and 0 <= gy < self.size:
                if self.grid[gy][gx] == 1:
                    print("[오류] 선택한 위치는 장애물입니다. 다른 곳을 클릭해주세요.")
                    return
                self.goal = (gx, gy)
                print(f"[목표 설정] 클릭한 좌표: {gx}, {gy}")
                self.path = self.a_star((self.origin, self.origin), self.goal)
                if self.path:
                    print(f"[경로 계산 완료] 경로 길이: {len(self.path)}")
                else:
                    print("[경로 계산 실패] 목적지까지 도달할 수 없습니다.")
                self.draw()

    def update_map(self, distance, angle_rad, robot_pose):
        global_angle = robot_pose.theta + angle_rad
        x = robot_pose.x + (distance / 10.0) * math.cos(global_angle)
        y = robot_pose.y + (distance / 10.0) * math.sin(global_angle)

        px = int(self.origin + x)
        py = int(self.origin - y)
        if 0 <= px < self.size and 0 <= py < self.size:
            self.points.append((px, py))
            for dx in range(-1, 2):
                for dy in range(-1, 2):
                    nx, ny = px + dx, py + dy
                    if 0 <= nx < self.size and 0 <= ny < self.size:
                        self.grid[ny, nx] = 1
            self.lines.append(((self.origin, self.origin), (px, py)))

    def draw(self):
        xlim = self.ax.get_xlim()
        ylim = self.ax.get_ylim()

        self.ax.clear()
        self.ax.set_xlim(xlim)
        self.ax.set_ylim(ylim)
        self.ax.set_facecolor('white')
        self.ax.grid(True)
        self.ax.axhline(self.origin, color='gray', linestyle='--')
        self.ax.axvline(self.origin, color='gray', linestyle='--')
        self.ax.set_xlabel("X (cm)")
        self.ax.set_ylabel("Y (cm)")
        self.ax.set_aspect('equal')  # ✅ 비율 고정 유지

        if self.points:
            offsets = np.array(self.points)
            self.ax.scatter(offsets[:, 0], offsets[:, 1], c='red', s=5)

        for start, end in self.lines:
            self.ax.plot([start[0], end[0]], [start[1], end[1]], color='blue', linewidth=0.2, alpha=0.3)

        if self.goal:
            self.ax.plot(self.goal[0], self.goal[1], 'go', markersize=8)

        if self.path and len(self.path) > 1:
            path_np = np.array(self.path)
            self.ax.plot(path_np[:, 0], path_np[:, 1], color='lime', linewidth=2)

        self.fig.canvas.draw()
        self.fig.canvas.flush_events()

    def a_star(self, start, goal):
        def heuristic(a, b):
            return abs(a[0] - b[0]) + abs(a[1] - b[1])

        open_set = []
        heapq.heappush(open_set, (0, start))
        came_from = {}
        g_score = {start: 0}

        neighbors = [(-1,0),(1,0),(0,-1),(0,1),(-1,-1),(-1,1),(1,-1),(1,1)]

        while open_set:
            _, current = heapq.heappop(open_set)

            if current == goal:
                path = [current]
                while current in came_from:
                    current = came_from[current]
                    path.append(current)
                return path[::-1]

            for dx, dy in neighbors:
                neighbor = (current[0]+dx, current[1]+dy)
                if 0 <= neighbor[0] < self.size and 0 <= neighbor[1] < self.size:
                    if self.grid[neighbor[1]][neighbor[0]] == 1:
                        continue
                    tentative_g = g_score[current] + 1
                    if neighbor not in g_score or tentative_g < g_score[neighbor]:
                        g_score[neighbor] = tentative_g
                        f_score = tentative_g + heuristic(neighbor, goal)
                        heapq.heappush(open_set, (f_score, neighbor))
                        came_from[neighbor] = current

        return []
