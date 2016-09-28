#!/usr/bin/env python

"""
setup.py file for Python wrapping of a C++ network data structure
"""

from distutils.core import setup, Extension

# The copying comes before building (not for all systems/versions). We use code from:
# http://stackoverflow.com/questions/12491328/python-distutils-not-include-the-swig-generated-module
from distutils.command.build import build


class CustomBuild(build):
    sub_commands = [
        ('build_ext', build.has_ext_modules),
        ('build_py', build.has_pure_modules),
        ('build_clib', build.has_c_libraries),
        ('build_scripts', build.has_scripts),
    ]


eventpy_module = Extension('eventpy._eventpy_module', ['eventpy/eventpy_module.i'],
                           swig_opts=["-c++"]
                           )

setup(name='eventpy',
      cmdclass={'build': CustomBuild},
      version='0.1',
      author="Mikko Kivela",
      description="""Python wrapping of a C++ event list data structure.""",
      ext_modules=[eventpy_module],
      # py_modules = ["eventpy"],
      packages=["eventpy", "eventpy.tests"],
      package_dir={"eventpy": "eventpy",
                   "eventpy.tests": "eventpy/tests"},
      package_data={"eventpy": ["data/*.txt"]}
      )
