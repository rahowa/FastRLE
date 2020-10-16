import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


def load_long_description():
    with open("README.md", 'r') as readme_file:
        readme = readme_file.read()
    return readme


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cmake_args = [f'-DPython_ROOT_DIR={sys.executable[:-6]}',
                      f'-DPYTHON_EXECUTABLE={sys.executable}',]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        
        current_dir = ext.sourcedir
        build_folder = os.path.join(ext.sourcedir, 'build')
        subprocess.check_call(['cmake', current_dir] + cmake_args, cwd=build_folder, env=env)
        subprocess.check_call(['cmake', '--build', build_folder] + build_args, cwd=build_folder)
        subprocess.check_call(['cmake', '--install', build_folder], cwd=build_folder)


setup(
    name='fast_rle',
    version='0.0.1',
    author='rahowa',
    license="LICENSE",
    url="https://github.com/rahowa",
    author_email='roninfromsvf@gmail.com',
    description='A test project using pybind11 and CMake',
    cmdclass=dict(build_ext=CMakeBuild),
    long_description=load_long_description(),
    ext_modules=[CMakeExtension('Boost_to_python')],
    packages=find_packages(include=['fast_rle', 'fast_rle.*', 'tests', 'benchmarks']),
    zip_safe=False,
    package_data={'fast_rle': ['*.so',]},
    include_package_data=True,
)