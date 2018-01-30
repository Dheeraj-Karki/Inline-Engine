# Inline Engine

Introduction
---
Inline Engine is a modern game engine built with latest C++ that provides/will provide solutions to existing common industrial problems:
- Smoother Editor User Experience
- Higher Performance Editor & Engine
- Better Stability & More Focus On The Community
- Better PBR Graphics than the typical industrial PBR equations

The Engine leverages the power of DirectX 12 and similar APIs. It got its name from the overuse of inline methods to speed up the software, however, it's not a victim of such behavior. The Engine is component based so you can tear it to pieces and for example integrate the Graphics, Network, Physics, etc modules individually into an another complete game engine or real-time simulation environments.

Platforms
---
1. Windows (working)
2. Linux   (future)
4. XBoxOne (future)
3. PS4     (future)

How To Build
---
### Windows
1. Open Inline-Engine.sln in Visual Studio 2017 latest version (2015 might do as well, but not tested)
2. Build All Projects
3. Enjoy

At the moment 2 projects are interesting which you can run ( gpu must support DirectX 12 feature level 11_0  + install latest drivers ):
- QC_Simulator -> Quadcopter simulator with which you can play or tweak the PID controller.
- Editor       -> Where you can edit your whole game/project with tools

Core ideas
---
### High-level interface
The engine exposes geometries, material, and textures to work with. These are used as properties to describe an entity. Entities are grouped into multiple scenes that act like a virtual world. One might put terrain entities, lights, geometries or other types of objects into a scene. The purpose of having multiple scenes is to achieve the separation of 3D world, GUI and debug draw objects. With this, one might define completely different ways of rendering each scene, and can quickly toggle whether to display a scene at all. The results of each scene can be freely combined with desired final output.

### Visual render pipeline scripting
Inspired by CryEngine's flow graph and Unreal's BluePrint, users can leverage the power of task graphs to visually assemble the render pipeline. The task graph's tasks have access to the scenes and the objects inside them, so they can render it. The data flow from one task to the other allows the transport of opacity or depth maps which make combining scenes in an arbitrary fashion a breeze.

### Adding your own rendering algorithms
The above-mentioned rendering task graph provides an interface to implement custom task nodes. The nodes inputs and outputs are defined, and the programmer can code the data transform performed on the inputs. The node can access the underlying Direct3D 12 API through a simplified interface. The framework that executes the task graph takes the responsibility of distributing work across multiple CPU cores and the scheduling of generated GPU command lists.
