class Robot:
    def __init__(self, x, y, available, status, degradation, task, hasPackage):
        self.x = x
        self.y = y
        self.available = available
        self.status = status
        self.degradation = degradation
        self.task = task
        self.hasPackage = hasPackage
        self.package = None
        self.actions = {"Left": (-1, 0), "Right": (1,0), "Up": (0,-1), "Down": (0,1)}

    def move(self, action):
        self.x += self.actions(action)[0]
        self.y += self.actions(action)[1]

        if self.hasPackage:
            if self.package.size == "Small":
                self.degradation -= 1.0
            if self.package.size == "Medium":
                self.degradation -= 2.0
            if self.package.size == "Large":
                self.degradation -= 3.0

    def lift(self, package):
        self.hasPackage = True
        self.package = package

        if self.package.size == "Small":
            self.degradation -= 1.0
        if self.package.size == "Medium":
            self.degradation -= 2.0
        if self.package.size == "Large":
            self.degradation -= 3.0

    def place(self):
        self.hasPackage = False

        self.package = None
        
        if self.package.size == "Small":
            self.degradation -= 1.0
        if self.package.size == "Medium":
            self.degradation -= 2.0
        if self.package.size == "Large":
            self.degradation -= 3.0

    def notify_scheduler(self):
        self.task.status = "Complete"