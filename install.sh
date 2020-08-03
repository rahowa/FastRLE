pip install numpy 
pip install conan

mkdir build 
python_path=$(wich python)

conan install . -s build_type=Release --install-folder=build --build=boost -o boost:python_executable=$python_path  -o boost:with_python=True --build=opencv --build=protobuf

cd build 

cmake -DCMAKE_BUILD_TYPE=Release -DPython_ROOT_DIR=$python_path ..
make -j $(nproc)
make install   

python setup.py install --user