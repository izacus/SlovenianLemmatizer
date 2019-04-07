from setuptools import setup, Extension
from Cython.Build import cythonize

import os
import platform

root_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
src_dir = os.path.join(root_dir, "src")
include_dir = os.path.join(root_dir, "include")

# Setup compilation arguments for native code
compile_args = []
link_args = []
if os.name == 'posix':
  compile_args = ["-O3", "-ffunction-sections", "-fdata-sections", "-fvisibility-inlines-hidden", "-std=c++11"]
  if platform.system() == 'Darwin':
    link_args = ["-Wl,-dead_strip"]
  else:
    link_args = ["-Wl,-z,noexecstack", "-Wl,-z,now", "-Wl,-z,relro", "-Wl,--gc-sections"]
elif os.name == 'nt':
  compile_args = ["/Ox", "/EHsc"]
  link_args = []

lib = Extension('lemmagen.libLemmagen',
                ["lemmagen/libLemmagen.pyx", os.path.join(src_dir, "lemmagen.cpp"), os.path.join(src_dir, "RdrLemmatizer.cpp")],
                extra_compile_args=compile_args,
                extra_link_args=link_args,
                include_dirs=[include_dir])

setup(name="Lemmagen",
      version="1.3.0",
      description="LemmaGen lemmatizer for Python supporing Slovene, Serbian, Romanian, Estonian, Bulgarian and other languages",
      package_data={'lemmagen':["dictionaries/*"]},
      license="GPLv2+",
      author="Jernej Virag",
      author_email="jernej@virag.si",
      classifiers=[
        "License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)",
        "Operating System :: OS Independent",
        "Intended Audience :: Developers",
        "Development Status :: 5 - Production/Stable",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
        "Topic :: Text Processing :: Linguistic",
      ],
      ext_modules=cythonize([lib]),
      test_suite="tests",
      packages=["lemmagen"])
