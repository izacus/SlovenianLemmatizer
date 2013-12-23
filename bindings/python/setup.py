from distutils.core import setup,Extension

lib = Extension('lemmagen.libLemmagen',
                ["src/sl_lemmatizer.cpp", "src/RdrLemmatizer.cpp"],
                extra_compile_args=["-O3"])

setup(name="Lemmagen",
      version="1.0.3",
      description="LemmaGen lemmatizer for Python supporing Slovene, Serbian, Romanian, Estonian, Bulgarian and other languages",
      package_data={'lemmagen': ["dictionaries/*"]},
      license="GPLv2+",
      author="Jernej Virag",
      author_email="jernej@virag.si",
      classifiers=[
        "License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)",
        "Operating System :: OS Independent",
        "Intended Audience :: Developers",
        "Development Status :: 4 - Beta",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Programming Language :: Python",
        "Topic :: Text Processing :: Linguistic",
      ],
      ext_modules=[lib],
      packages=["lemmagen"])
