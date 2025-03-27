import random

WIDTH = 800
HEIGHT = 600
SEG_SIZE = 20
MAPPING = {"Down": (0, 1), "Right": (1, 0),
            "Up": (0, -1), "Left": (-1, 0)}
IMAPPING = {(0, 1): "Down", (1, 0): "Right",
            (0, -1): "Up", (-1, 0): "Left"}

snake_immunity_period = 6
game_period = 1

class Snake:
    def __init__(self, color):
        self.color = color
        self.segments = []
        self.eaten = False
        self.immunity = 0

    def first_update(self, x, y):
        self.segments = [(x-4, y), (x-3, y), (x-2, y), (x-1, y), (x, y)]

    def get_dir(self):
        xh, yh = self.segments[-1]
        xn, yn = self.segments[-2]
        return IMAPPING[(xh-xn, yh-yn)]

    def update(self, x, y, eaten, apple):
        self.immunity = self.immunity - game_period if self.immunity > 0 else 0

        ax, ay = apple
        xh, yh = self.segments[-1]
        xdn, ydn = x-xh, y-yh

        if eaten:
            collinear = xdn * (ay - yh) == ydn * (ax - xh)
            within_bounds = min(xh, x) <= ax <= max(xh, x) and min(yh, y) <= ay <= max(yh, y)
            if eaten and collinear and within_bounds: # the apple was eaten by me
                self.segments.insert(0, None)
                self.immunity += snake_immunity_period

        norm = abs(sum([xdn, ydn]))
        dir_x, dir_y = xdn//norm, ydn//norm
        for _ in range(norm):
            self.segments.pop(0)
            xh, yh = self.segments[-1]
            self.segments.append((xh+dir_x, yh+dir_y))

class SnakeGame:
    def __init__(self, my_color):
        self.state = False
        self.height = HEIGHT // SEG_SIZE  # grid height in cells
        self.width = WIDTH // SEG_SIZE    # grid width in cells
        self.ysnake = Snake('Y')
        self.rsnake = Snake('R')
        if my_color == 'R':
            self.my_snake = self.rsnake
            self.other_snake = self.ysnake
        else:
            self.my_snake = self.ysnake
            self.other_snake = self.rsnake
        self.apple = (None, None)
        self.goal = (None, None)

    def update(self, yx, yy, rx, ry, ax, ay):
        if not self.state:  # first update: initialize snakes and apple
            self.ysnake.first_update(yx, yy)
            self.rsnake.first_update(rx, ry)
            self.state = True
        else:
            eaten = (self.apple != (ax, ay))
            self.ysnake.update(yx, yy, eaten, self.apple)
            self.rsnake.update(rx, ry, eaten, self.apple)
        self.apple = (ax, ay)
    
    def get_goal_dir(self):
        xg, yg = self.goal
        xh, yh = self.my_snake.segments[-1]
        directions = []
        if xg-xh > 0:
            directions.append('Right')
        if xg-xh < 0:
            directions.append('Left')
        if yg-yh > 0:
            directions.append('Down')
        if yg-yh < 0:
            directions.append('Up')
        return directions
    
    def dist(self, pos1, pos2):
        return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])

    def set_goal(self):
        # if self.my_snake.immunity > 0 and False: # no need to attack
        #     distances = [self.dist(self.my_snake.segments[-1], e) for e in self.other_snake.segments[:-1]]
        #     min_dist = min(distances)
        #     if min_dist < self.my_snake.immunity:
        #         self.goal = self.other_snake.segments[distances.index(min_dist)]
        #         print(min_dist, self.goal, self.my_snake.immunity)

        # my_dist = self.dist(self.my_snake.segments[-1], self.apple)
        # other_dist = self.dist(self.other_snake.segments[-1], self.apple)
        # xh, yh = self.my_snake.segments[-1]
        # xho, yho = self.other_snake.segments[-1]
        # xa, ya = self.apple
        # reachable_me = ((xh-xa)%game_period == 0 or (yh-ya)%game_period == 0)
        # reachable_other = ((xho-xa)%game_period == 0 or (yho-ya)%game_period == 0)
        # # if my_dist < other_dist: #and reachable_me:
        # if reachable_me:
        #     self.goal = self.apple
        # else:
        #     self.goal = (self.width // 2, self.height // 2)

        self.goal = self.apple

    def move(self):
        xh, yh = self.my_snake.segments[-1]

        allowed = []
        for dir, (x, y) in MAPPING.items():
            # check wall collision
            xf = xh + x*game_period # x futur
            yf = yh + y*game_period
            if  0 <= xf < self.width and 0 <= yf < self.height:
                allowed.append(dir)
            
            # check snake collision
            if dir in allowed:
                if xf == xh:
                    path = [(xf, y) for y in range(min(yf, yh), max(yf, yh)+1)]
                elif yf == yh:
                    path = [(x, yf) for x in range(min(xf, xh), max(xf, xh)+1)]
                segset = set(self.other_snake.segments)
                for pos in path:
                    if pos in segset:
                        allowed.remove(dir)
                        break
            
            # if dir in allowed:
            #     if self.dist((xf, yf), self.other_snake.segments[-1]) < game_period:
            #         allowed.remove(dir)

            if dir in allowed and self.dist((xf, yf), self.other_snake.segments[-1]) <= game_period:
                advantage = 0
                if self.my_snake.color == 'Y':
                    advantage = 1
                xho, yho = self.other_snake.segments[-1]
                if xf == xh and abs(yh-yf) + advantage >= abs(xho-xf): # vertical
                    allowed.remove(dir)
                elif yf == yh and abs(xh-xf) + advantage >= abs(yho-yf): # horizontal
                    allowed.remove(dir)
            
            # # new ver
            # xho, yho = self.other_snake.segments[-1]
            # x_diff, y_diff = abs(xh-xho), abs(yh-yho)

            # if dir in allowed and x_diff < game_period and y_diff < game_period:
            #     advantage = 0
            #     if self.my_snake.color == 'Y':
            #         advantage = 1

            #     if xf == xh and y_diff >= x_diff + advantage: # vertical
            #         allowed.remove(dir)
            #     elif yf == yh and x_diff >= y_diff + advantage: # horizontal
            #         allowed.remove(dir)
        
        self.set_goal()
        goal_dir = self.get_goal_dir()

        if goal_allowed := list(set(goal_dir).intersection(allowed)):
            return random.choice(goal_allowed)
        elif allowed:
            return random.choice(allowed)
        return random.choice(['Up', 'Down', 'Right', 'Left'])


