# ndGameEngine

THIS PROJECT IS DEPRECATED. All production will be moved to ndGameEngineV2.

 ndGameEngine a real-time rendering engine prototype originally built using OpenGL. The ndGameEngine application structure uses abstract module layers, ignorant of each other, that interact through a ubiquitous event system.

December 2023 update:
- ndGameEngine is being developed on an M2 Pro chip. However, OpenGL is depricated Apple Silicon. And in my developement, I have hit the limits of how far I can strecth OpenGL's support and performance. So in order to increase performance, I am migrating the entire ndGameEngine to run on the Metal graphics API. This will make ndGameEngine explicitly a macOS software. The new application window system will use the Cocoa framework and the shaders will use the Metal and MetalKit frameworks. These will primiarily be written in Objective-C. The main Cocoa run loop will be "unwrapped" and manually updated in the ndGameEngine run loop in C++ through an interface written in Objective-C++. The code to do such is inspired by the GLFW source code. I am writing my own interface so we can keep most of the old ndGameEngine source code as well has retain the flexibility and control of the macOS application and GPU by having access to the Objective-C source code.

 October 2023 update:
 - Currently supports a rudimentary mesh editing system. Can select vertices with a select box and select faces of vertices by clicking on face. Can translate selected points through space. Prototype extrude face function.
 
 ![Edit Space Environment](https://github.com/Ben-Rutkowski/ndGameEngine/assets/70175023/f7512520-5b66-4442-b19c-e205ba0d43a1)
 ![Selecting Points](https://github.com/Ben-Rutkowski/ndGameEngine/assets/70175023/b8ca2e05-4adb-4bef-9eda-81efb49098f7)
