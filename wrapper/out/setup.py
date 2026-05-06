import sys
from setuptools import setup

ext_name = 'ecf_wrap.pyd' if sys.platform.startswith('win') else 'ecf_wrap.so'

setup(
    name='ecf_wrap',
    packages=['ecf_wrap'],
    package_data={'ecf_wrap': [ext_name, 'parameters.txt']},
)