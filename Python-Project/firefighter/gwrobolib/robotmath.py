
def make_within(value, lower, upper):
    # Changes the value as needed to be within a range
    return min(upper, max(lower, value))

def are_same_sign(x, y):
    # Returns if x and y are same sign
    return (x * y) >= 0

def get_percent_error(current, goal):
    # Returns a decimal value -1.0 to 1.0 for correction
    return abs(current - goal) / goal

def map(x, from_low, from_high, to_low, to_high):
    return (x - from_low) * (to_high - to_low) // (from_high - from_low) + to_low
