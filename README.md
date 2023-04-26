# Cub3d : Full raycaster engine in C
## Description
Cub3d is a project from 42, where the goal is to create a raycaster in C.
It should take a simple 2D map, parse it and output a 3D view of the environment with the textures you linked.

## How to use
To launch the project first you need to configure a map:

Create a .cub file, and declare the textures and Floor and Ceilings according to the template.

The textures must be .xpm files.

The map is composed of 0s (Empty spaces) and 1s (Walls).
```
NO ./textures/white_grunge.xpm
SO ./textures/brick.xpm
WE ./textures/metal_plate.xpm
EA ./textures/wood.xpm

F 0,0,0
C 0,0,0

11111111111111111
10001111100000001       111 111
10000000000000001      100010001
10000100100100001      100010001
10000000000000001       1000001
10000000000000001        10001
11000111000000001         101
100001 1000000001111111111101
10011  1000000000000000000001
1001   1111111111111111111111
1111
```
