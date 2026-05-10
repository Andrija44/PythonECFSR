import sys
from setuptools import setup

ext_name = 'ecf_sr.pyd' if sys.platform.startswith('win') else 'ecf_sr.so'

setup(
    name='ecf_sr',
    packages=['ecf_sr'],
    package_data={'ecf_sr': [ext_name, 'parameters.txt']},
)