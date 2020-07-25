         ________                   __      _______   __        ________  
        /        |                 /  |    /       \ /  |      /        |  
        $$$$$$$$/_____    _______ _$$ |_   $$$$$$$  |$$ |      $$$$$$$$/  
        $$ |__ /      \  /       / $$   |  $$ |__$$ |$$ |      $$ |__  
        $$    |$$$$$$  |/$$$$$$$/$$$$$$/   $$    $$< $$ |      $$    |  
        $$$$$/ /    $$ |$$      \  $$ | __ $$$$$$$  |$$ |      $$$$$/  
        $$ |  /$$$$$$$ | $$$$$$  | $$ |/  |$$ |  $$ |$$ |_____ $$ |_____  
        $$ |  $$    $$ |/     $$/  $$  $$/ $$ |  $$ |$$       |$$       |  
        $$/    $$$$$$$/ $$$$$$$/    $$$$/  $$/   $$/ $$$$$$$$/ $$$$$$$$/  
    

    ================================================================================
    ==        Fast rle encoding-decoding library for segmentation mask.           ==
    ================================================================================

## Build
- install `conan` package manager via `pip`
```shell script
$ pip instal conan
```
- create build directory
```shell script
$ mkdir build
```
- install dependencies via `conan`
``` shell script
$ conan install . -s build_type=Release --install-folder=build --build=boost -o boost:python_executable=[path to python]]bin/python  boost:python_version=[major.minor] -o boost:with_python=True --build=opencv --build=protobuf
```
- cd inside `build` and build library
```shell script
$ cmake -DCMAKE_BUILD_TYPE=Release -DPython_ROOT_DIR=[path to python]/bin ..
$ make -j($nproc)
$ make install
```

## Usage

## License

