# Animation Player
This project is not going to become a library itself. It is intended to be a proof of concept showing that:
	animation matrices can be exported from an external tool (such as a blender script)
	those animation matrices can be easily ported into C++ projects utilizing OpenGL
	
Includes a blender script for exporting basic animations into a usable format



## Version Notes
unstable - Works completely fine. uses an animation made in blender (stored in animation.json) as well as a model made in blender (stored in some .obj file idk what it's called)
	CONTROLS: Scroll wheel to scrub animation
			wasd to move around (always looking at origin)

v0.1_ply_working_backup - backup of a working PLY version in case something goes wrong :)

v0.1 - Work in progress. Adding support for Stanford Polygon Files (binary and ascii)

![AnimationDemo](https://user-images.githubusercontent.com/30155130/154782715-0930a4f1-d65b-4e89-aeff-eb3916943982.gif)
