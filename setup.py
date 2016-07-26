#!/usr/bin/env python

"""
setup.py file for Python wrapping of a C++ network data structure
"""

from distutils.core import setup, Extension


eventpy_module = Extension('eventpy._eventpy_module',['eventpy/eventpy_module.i'],
                           swig_opts=["-c++"]
                           )

setup (name = 'eventpy',
       version = '0.1',
       author      = "Mikko Kivela",
       description = """Python wrapping of a C++ event list data structure.""",
       ext_modules = [eventpy_module],
       #py_modules = ["eventpy"],
       packages = ["eventpy","eventpy.tests"],
       package_dir = {"eventpy" : "eventpy",
                      "eventpy.tests":"eventpy/tests"}, 
       package_data = {"eventpy": ["data/*.txt"]}
       )
