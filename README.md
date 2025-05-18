# About this repository

This project is a research project focusing on computer graphics.
The program allos users to render volumetric shadows of hair-shaped objects with following algorithms.

- Opacity Shadow Maps
- Deep Opacity Maps
- Shadow Map

However, when I write this program, I did not have enough skill to generate cmake or Scons files, so that there is no compile support systems in this repository.

If you have any questions, please don't hesitate to send me messages about this repository.

Thank you.

# About vhHairShadow

This program was developed and used by Ikeda during his undergraduate and master's thesis research.  
As both a reference for future studies and to ensure that the work I did doesn’t get lost over time, I’ve decided to preserve it by managing the repository on a Git server.

## Dependencies

- glm  
- freeglut  
- OpenGL  
- GLEW  

## Design Philosophy

This project aimed to minimize the use of **global variables** as much as possible.

At the time, GLFW had not yet become as widespread as it is today, so I used GLUT. However, for efficiency—especially with callback functions—it was sometimes inevitable to use global references.

Still, I felt that excessive use of global state was not only inelegant but also harmed readability and maintainability. So, I made an effort to eliminate it wherever possible.

To achieve this, I designed a **top-level management class** that sits above GLUT. The core framework responsible for rendering operates entirely under this manager class, with all submodules implemented as instance variables.

### Object-Oriented Structure

To support an object-oriented programming model, nearly every renderable entity has its own dedicated class.  
This makes it easy to add new objects: simply define a new class and register it with the **manager class**, and it will be ready to use.

The rendering pipeline is also organized into modular classes.  
Due to time constraints, I couldn't achieve full instancing at the time, but each stage of the rendering process is managed in its own class.

For example, the volumetric shading pipeline consists of:

- Shadow map generation  
- Volume map construction  
- Final rendering pass  

Each of these stages is encapsulated in its own class, allowing the pipeline to be built and executed by simply calling them in order.

### Use of Modern OpenGL

As of 2013, although newer versions of OpenGL had already been released, most tutorials and documentation were still written for OpenGL 1.2 or 2.1.  
I found it unsatisfying to ignore the latest developments, so in this project I experimented with techniques specific to **modern OpenGL**, such as structured data transfer.

Since I was still a student, I had the freedom to care deeply about small details.  
If you're interested, feel free to take a look—even just at the shader code.

## Compatibility Notes

Please note that the OpenGL environment has evolved significantly since this program was written.  
Modern GPU drivers and hardware are more strict about things like memory leaks.

As a result, bugs that used to be silently tolerated may now cause the program to crash or abort after a few minutes.  
Please be aware of this when running the program.

## Regrets

- The project was originally named `[OSM]loadHairModel`, and since renaming Visual Studio projects can be a hassle, the name has remained unchanged.
