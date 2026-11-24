class Robot:
    def __init__(self, x, y, id):
        self.x = x
        self.y = y
        self.id = id

        self.available = True
        self.status = "Good"
        self.degradation = 100.0
        self.task = None
        self.hasPackage = False
        self.package = None
        self.lift_time = 3.0
        self.move_time = 1.0
        self.actions = {"Left": (-1, 0), "Right": (1,0), "Up": (0,-1), "Down": (0,1)}

    # Getters
    def get_id(self):
        return self.id
    
    def get_coords(self):
        return (self.x, self.y)

    def get_available(self):
        return self.available

    def health_check(self):
        return self.degradation

    def get_task(self):
        return self.task

    def get_package(self):
        if self.hasPackage:
            return self.package

    # Setters
    def set_coords(self, x, y):
        self.x = x
        self.y = y

    def set_available(self, available):
        self.available = available

    def set_status(self, status):
        self.status = status

    def set_task(self, task):
        self.task = task

    def set_hasPackage(self, hasPackage):
        self.hasPackage = hasPackage

    def set_package(self, package):
        self.package = package
    
    # Robot functions
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