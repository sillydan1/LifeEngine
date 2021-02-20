# LIFE ENGINE V4
Life Engine is a for-fun game engine. This will likely not become anything.

## Build
Development is being done on Linux (Debian 10 buster) so there is no official support for non-linux compilation pipelines.

 - Initialize the submodules: 

```
git submodule update --init
```

 - Install the dependencies: 
   
```
sudo apt-get install libglew-dev libassimp-dev libglfw3-dev libglm-dev --install-recommends
```

Building is then standard CMake procedure:
```
mkdir build && cd build
cmake ..
make
```

## Latest Screenshot
![Latest Screenshot](readme_resources/first_triangle.png)

###### Authors: sillydan1
