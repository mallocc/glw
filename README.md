# C++ GLFW/GLEW Wrapper [Graphics Library Wrapper] 
### A readme for OpenGL Wrapper project.
## Dependancies 
- cmake
- build-essential
- xorg-dev
- libglew-dev
- libglm-dev
- libx11-dev
- libxxf86vm-dev
- libpthread-stubs0-dev
- libxi-dev
- libxinerama-dev
- libxcursor-dev

Found this link extremely helpful: [how-to-build-install-glfw-3-and-use-it-in-a-linux-project](https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project)

This was built on Linux Xubuntu 18.04

Useful apt install command:
```
sudo apt-get install cmake build-essential xorg-dev libglew-dev libglm-dev libx11-dev libxxf86vm-dev libpthread-stubs0-dev libxi-dev libxinerama-dev libxcursor-dev
```

###  glfw3
 Compiling with version 3.2.1 and core profile version has been set to 4.0

How I installed GLFW:
```
git clone https://github.com/glfw/glfw.git
cd glfw
cmake -G "Unix Makesfiles"
make
make install
```
Should be installed into ```/usr/local/```

(May need elevated privileges)

###  stb
How I installed stb_image:
```
git clone https://github.com/nothings/stb.git
cd stb
mkdir /usr/include/stb_image
sudo cp * /usr/include/stb_image/
```

(May need elevated privileges)

# Directory Structure
Should have a tree structure of:
```
./
../
add*            (skeleton class creator)
build/          (build files)
CMakeLists.txt  (main cmake script)
inc/            (*.h)
run*            (helpful build/run command)
src/            (*cpp)
shaders/        (*.frag *.vert)
```

# Tips
- CMakeLists.txt will build the make files.
- ```cmake``` and ```make``` should be run inside of ```build/```.
- Put all ```*.h``` files in ```inc/```.
- Put all ```*.cpp``` files in ```src/```.
- Make sure to add each new ```.cpp```, you want to compile, to the list of ```.cpp```'s
  in ```CMakeLists.txt``` (after the ```#files``` line).
- ```run``` is a bash script that will:
  - ```cmake``` inside of ```build/```,
  - ```make``` inside of ```build/```,
  - Run the resulting program from ```make```, if ```make``` succeeds.
- ```add``` is a bash script that creates a skeleton class and saves it to a ```.h```
  and ```.cpp``` in the correct directories. It will also add the new ```.cpp``` to the 
  ```CMakeLists.txt``` files.

# (The following is out of date)

General class structure for the graphics wrapper:
 
GContent
- This is essentially your GL content.
- It will create a GLFW window and link your specified functions: 
graphics loop, initialisation, etc.
- Will generate perspective matrices.
- Can be run standalone from all the other classes.

GShaderProgram
- This is a wrapper for handling shader programs.
- Can load glsl shader files into GL.
- Can link variable handles for the program.
  
GShaderProgramManager
- This makes it easier to organise more than one GShaderProgram.
- Can load a GShaderProgram according to their GShaderProgramId.
  
GShaderVariableHandle
- This is a wrapper for linking and referencing uniforms variables for your
shaders.
- Can load pointers to be loaded quickly or load directly with a variable.
  
GVertexBufferObject
- This is a simple vertex buffer wrapper that will handle loading and drawing of
vertex buffers.

