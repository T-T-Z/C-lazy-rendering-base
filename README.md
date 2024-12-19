A lazy marching cubes renderer in C++.
It uses SFML (for window managment only) so can be easily swapped out for another library.
Also SFML is the wrong library for 3d graphics since it has no built in support for 3d graphics unlike other libraries I only needed a light weight window/audio/io manager so I use it for that.
It uses OpenGL for the Graphics.

![Screenshot_20241219_191240](https://github.com/user-attachments/assets/89bfc465-c078-4bd2-acdc-ebc75514713c)
Figure 1: Rendering some randomly generated points

There is functionality to turn a point cloud into a mesh using marching cubes. The size can be arbitrary as it doesn't use the standard "marching" aspect of marching cubes and instead works on a per point basis. Also allowing the points to be places anywhere.
It will also separate the point cloud into indivudual joined clusters. This is done because I find floating objects are almost always unwanted anyway so this makes it easy to detect and do something with them.
This was structed to make it easy to expand this to make procedural anythings as well as to make point cloud visualisation easy, also to physically simulate these point cloud structures (another reason to split the point cloud into individual solid objects).

It has the basic optimisations done like:
not having duplicate verticies and instead sharing them between cubes.
Spatial hashing to check nearby points.
Not generating some edge cases of the marching cubes (Not intended and causes holes in the mesh sometimes)

I was able to generate a 10 million point meshand split it into clusters in about 1 second and render it at 144+fps (RTX 2060 + unknown "amd" used $60 cpu from ebay in 2022)

I put it together more to refresh my C++ and have an easily expandable rendering setup but it can be a good foundation for anyone that wants to spend time learning how to properly implement marching cubes. Or just for fun.

Licence: Do what you want with my code, But check SFML OpenGL licences if you want to use it (or use a different window manager)
