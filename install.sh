git submodule update --init --recursive
mkdir build 
cd build 

python -m venv env
source ./env/bin/activate 
pip install numpy conan pandas

python_path=$(which python)
 
conan install .. -s build_type=Release \
 --install-folder=. \
 -o boost:python_executable=$python_path \
 -o boost:without_python=False \
 --build=boost \
 --build=opencv \
 --build=protobuf \
 --build=missing

cd ..
python setup.py install --user
