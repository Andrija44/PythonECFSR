#!/usr/bin/env python

from setuptools import setup

setup(
    name='ecf_wrap',
    # version='1.6.6',
    # description='Python GP-GOMEA',
    # author='Marco Virgolin',
    # author_email='marco.virgolin@cwi.nl',
    # url='https://github.com/marcovirgolin/GP-GOMEA',
    packages=['ecf_wrap'],
    package_data={'ecf_wrap': ['ecf_wrap.so', 'parameters.txt']},
)
