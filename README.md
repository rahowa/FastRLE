         ________                   __      _______   __        ________  
        /        |                 /  |    /       \ /  |      /        |  
        $$$$$$$$/_____    _______ _$$ |_   $$$$$$$  |$$ |      $$$$$$$$/  
        $$ |__ /      \  /       / $$   |  $$ |__$$ |$$ |      $$ |__  
        $$    |$$$$$$  |/$$$$$$$/$$$$$$/   $$    $$< $$ |      $$    |  
        $$$$$/ /    $$ |$$      \  $$ | __ $$$$$$$  |$$ |      $$$$$/  
        $$ |  /$$$$$$$ | $$$$$$  | $$ |/  |$$ |  $$ |$$ |_____ $$ |_____  
        $$ |  $$    $$ |/     $$/  $$  $$/ $$ |  $$ |$$       |$$       |  
        $$/    $$$$$$$/ $$$$$$$/    $$$$/  $$/   $$/ $$$$$$$$/ $$$$$$$$/  
    

    +==============================================================================+
    |         Fast rle encoding-decoding library for segmentation mask.            |
    +==============================================================================+


## Table of Contents
- [Easy build](#easy-build)
- [Custom build](#custom-build)
- [Local becnhmarks ](#local-benchmarks)
- [Usage example](#usage-example)
- [License](#license)


## Easy build
```shell sript
$ sudo chmod a+x install.sh
$ ./install.sh
```

## Custom build
- clone repo recursively
```shell script 
$ git clone https://github.com/rahowa/FastRLE --recursive && cd FastRLE
```
- install `conan`, `numpy` and `pandas` via `pip`
```shell script
$ pip instal conan numpy pandas
```
- create build directory and `cd` inside
```shell script
$ mkdir build && cd build
```
- install dependencies via `conan`
``` shell script
$ conan install .. -s build_type=Release \
    --install-folder=. \
    -o boost:python_executable=$(which python) \
    -o boost:without_python=False \
    --build=boost \
    --build=opencv \
    --build=protobuf \
    --build=missing
```

- move to source dir and `setup.py`
```shell script
$ cd..
$ python setup.py install --user
```

- check installation with
```shell script
$ python -c "from fast_rle import processing as p; p.about()"
```

## Local benchmarks 
### To check perfomance on your local machine run files inside `benchmarks` folder
```shell script
$ cd bencmarks
$ python benchmarks_*.py
```

## Usage example
```python
import matplotlib.pyplot as plt 
from fast_rle import processin as fr #Base module with all operations

rles = fr.read_csv("path/to/rles.csv") #Fast .csv file parsing to list of RleFile objects
masks = fr.decode(rles) # Decode RLE format to binary masks

plt.imshow(masks[0])
plt.show()
```

## License
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

**[MIT license](http://opensource.org/licenses/mit-license.php)**

