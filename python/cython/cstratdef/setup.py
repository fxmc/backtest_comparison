# To run this:
#   python setup.py build_ext --inplace
# or
#   python setup.py install --record files.txt
#       so that we know what to remove if necessary
# copy resulting pyd file to cython directory or make pyd modules available in PYTHONPATH


from setuptools import setup
from Cython.Build import cythonize

setup(
    name='rsi2_def',
    ext_modules=cythonize('rsi2_def.pyx', compiler_directives={'language_level' : "3"}),
    version = "0.1.0",
    zip_safe=False,
)
