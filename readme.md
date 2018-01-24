# ScaleMail Prototype

## Build

Install the following into extlibs directory:
+ [glad](https://github.com/Dav1dde/glad)
+ [GLFW](https://github.com/glfw/glfw)
+ [GLM](https://github.com/g-truc/glm)
+ [LodePNG](https://github.com/lvandeve/lodepng)

Use lowercase directory names (glad, glfw, glm, lodepng).

Use the [glad webservice](http://glad.dav1d.de/) to generate source for glad.

All other dependencies can be cloned using Git.

The included CMake scripts will build the libraries and the application from source.

```shell
mkdir build
cd build
cmake ..
make
```