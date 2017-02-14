
def make_within(self, value, lower, upper):
    # Changes the value as needed to be within a range
    return min(upper, max(lower, value))

def are_same_sign(self, x, y):
    # Returns if x and y are same sign
    return (x * y) >= 0

def get_percent_error(self, current, goal):
    # Returns a decimal value -1.0 to 1.0 for correction
    return (current - goal) / goal
