class Task:
    def __init__(self, package_id, priority, package_start, package_end):
        self.package_id = package_id
        self.priority = priority
        self.package_start = package_start
        self.package_end = package_end
        self.status = "Not Complete"

    # Getters
    def get_package_id(self):
        return self.package_id

    def get_priority(self):
        return self.priority

    def get_package_start(self):
        return self.package_start

    def get_package_end(self):
        return self.package_end

    def get_status(self):
        return self.status

    # Setters
    def set_status(self, status):
        self.status = status