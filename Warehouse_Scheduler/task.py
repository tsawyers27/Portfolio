class Task:
    def __init__(self, package_id, priority, package_start, package_end):
        self.package_id = package_id
        self.priority = priority
        self.package_start = package_start
        self.package_end = package_end
        self.status = "Not Complete"