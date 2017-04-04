try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

config = {
    'description': '2017 Fire Fighter',
    'author': 'GW Robotics',
    'url': 'https://github.com/GW-Robotics/2017-Fire-Fighter/tree/master',
    'download_url': 'https://github.com/GW-Robotics/2017-Fire-Fighter/tree/master',
    'author_email': 'robotics@gwu.edu',
    'version': '0.1',
    'install_requires': ['nose'],
    'packages': ['2017-Fire-Fighter'],
    'scripts': [],
    'name': '2017-Fire-Fighter'
}

setup(**config)
