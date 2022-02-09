import os
from distutils.core import setup, Extension
import numpy as np

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"

if 'CUDA_PATH' in os.environ:
   CUDA_PATH = os.environ['CUDA_PATH']
else:
   print("Could not find CUDA_PATH in environment variables. Defaulting to /usr/local/cuda!")
   CUDA_PATH = "/usr/local/cuda"

if not os.path.isdir(CUDA_PATH):
   print("CUDA_PATH {} not found. Please update the CUDA_PATH variable and rerun".format(CUDA_PATH))
   exit(0)

if not os.path.isdir(os.path.join(CUDA_PATH, "include")):
    print("include directory not found in CUDA_PATH. Please update CUDA_PATH and try again")
    exit(0)

setup(name='Delaunay_module', version='1.0',
   ext_modules = [
      Extension('Delaunay_module', ['Delaunay_module.cpp'],
      include_dirs=[np.get_include(), os.path.join(CUDA_PATH, "include")],
      libraries=["GpuDelaunay", "cudart"],
      library_dirs=[".", os.path.join(CUDA_PATH, "lib64")],
      extra_compile_args=['-std=c++14']
)])


# from distutils.core import setup, Extension
# import numpy.distutils.misc_util
#
# # Adding sources of the project
# # *****************************
#
# m_name = "delaunay_triangulation"
#
# SOURCES = ["GDelFlipping/src/gDel3D/GpuDelaunay.cu"]
#
# module = Extension(m_name,
#                    sources=SOURCES,
#                    extra_compile_args=['-std=c++11',
#                                        '-D_GLIBCXX_USE_CXX11_ABI=0'])
#
# setup(ext_modules=[module], include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs())








