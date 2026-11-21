class Package:
    def __init__(self, id, size, x, y):
        self.id = id
        self.size = size
        self.x = x
        self.y = y
        self.delivered = False

    # Getters
    def get_id(self):
        return self.id

    def get_size(self):
        return self.size

    def get_coords(self):
        return (self.x, self.y)

    def get_status(self):
        return self.delivered

    # Setters
    def set_status(self, status):
        self.delivered = status