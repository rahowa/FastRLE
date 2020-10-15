pip install numpy 
pip install conan

mkdir build 
python_path=$(which python)

cd build 
conan install .. -s compiler.version=12.0 -s build_type=Release \
 --install-folder=. \
 -o boost:python_executable=$python_path \
 -o boost:without_python=False \
 --build=boost \
 --build=opencv \
 --build=protobuf \
 --build=missed

# cmake -DCMAKE_BUILD_TYPE=Release -DPython_ROOT_DIR=$python_path ..
# cmake --build . && cmake --install . 

cd ..
python setup.py install --user
