from nose.tools import *
from firefighter import firefighter

# def setup():
#     print "SETUP!"

# def teardown():
#     print "TEAR DOWN!"

# def test_basic():
#     print "I RAN!"

def test_config():
    assert_equal(firefighter.get_pin('input', 'input_example1'), '1')
    assert_equal(firefighter.get_pin('output', 'output_example1'), '2')