from setuptools import find_packages
from setuptools import setup

setup(
    name='detection_interfaces',
    version='1.0.0',
    packages=find_packages(
        include=('detection_interfaces', 'detection_interfaces.*')),
)
