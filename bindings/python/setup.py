from setuptools import setup, Extension
from Cython.Build import cythonize

import os
root_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
src_dir = os.path.join(root_dir, "src")
include_dir = os.path.join(root_dir, "include")

lib = Extension('lemmagen.libLemmagen',
                ["lemmagen/libLemmagen.pyx", os.path.join(src_dir, "lemmagen.cpp"), os.path.join(src_dir, "RdrLemmatizer.cpp")],
                extra_compile_args=["-O3"],
                include_dirs=[include_dir])

setup(name="Lemmagen",
      version="1.3.0",
      description="LemmaGen lemmatizer for Python supporing Slovene, Serbian, Romanian, Estonian, Bulgarian and other languages",
      package_data={'lemmagen': [os.path.join(root_dir, "dictionaries/*")]},
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
