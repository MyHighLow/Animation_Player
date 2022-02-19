#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <queue>
#include <limits>
#include <math.h>

#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#undef main

#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/type_ptr.hpp>
#include <json.hpp>
using json = nlohmann::json;

#include <srtools.h>
// #include <uit.h>

#include <util.hpp>

#define PREFIX "D:/usr/local/src/animation_player/unstable"

#define p1 -0.5f, -0.125f, 0.0f
#define p2 0.5f, -0.125f, 0.0f
#define p3 0.0f, 0.25f, 0.0f
#define c 0.85f, 0.0125f, 0.125f
float data[] = {p1, c, p2, c, p3, c};
#undef p1
#undef p2
#undef p3
#undef c




int main(int argc, char **argv) {

	// load animation data
	std::ifstream f(PREFIX "/data/animation.json");
	json j;
	f >> j;
	std::vector<std::pair<int, glm::mat4>> matrices = parse_jmat4_animation(j);

	// load obj file
	std::vector<glm::vec4> obj_verts = basic_parse_obj_file(PREFIX "/data/untitled.obj");

	// create initial transformation
	glm::mat4 transformation(
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	);
	glm::vec3 CameraPosition({0,0,10});
	float CameraSpeed = 0.1f;
	float far_plane = 10.0f;
	float fov = 45.0f;
	int frame_index = 0;

	// combine positions and colors
	std::vector<float> vertex_data;
	vertex_data.reserve(obj_verts.size() * 2);
	for (int i=0;i<obj_verts.size();i++) {
		vertex_data.push_back(obj_verts[i].x);
		vertex_data.push_back(obj_verts[i].y);
		vertex_data.push_back(obj_verts[i].z);
		// vertex_data.push_back(obj_verts[i].w);
		vertex_data.push_back(1.0f); // this shouldn't have to be the case
		vertex_data.push_back(0.85);								// r
		vertex_data.push_back((float)i / (float)obj_verts.size());	// g
		vertex_data.push_back(0.125f);								// b
	};

	// sdl => window => gl => glew => gl options
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Joe Mother", 10, 100, 300, 300, SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);
	glewInit();
	glClearColor(0.8f, 0.8f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// compile => link => use
	Shader vs = Shader::FromFile(PREFIX "/data/shaders/colored_triangle/vert.glsl", GL_VERTEX_SHADER);
	Shader fs = Shader::FromFile(PREFIX "/data/shaders/colored_triangle/frag.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram s = ShaderProgram::FromShaders({vs,fs});
	s.Use();
	
	// vbo, vao => data => attrib pointers => cleanup
	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4*vertex_data.size(), vertex_data.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(vao);
	s.JsonAttribPointers(PREFIX "/data/shaders/colored_triangle/attributes.json");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUniformMatrix4fv(s.Uniform("u_transform"), 1, GL_FALSE, glm::value_ptr(transformation));

	// main loop
	bool dirty = true;
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
				return 0;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
				CameraPosition -= glm::vec3(0,0,CameraSpeed);
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
				CameraPosition += glm::vec3(0,0,CameraSpeed);
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
				CameraPosition -= glm::vec3(CameraSpeed,0,0);
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
				CameraPosition += glm::vec3(CameraSpeed,0,0);
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) {
				CameraPosition -= glm::vec3(0,CameraSpeed,0);
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E]) {
				CameraPosition += glm::vec3(0,CameraSpeed,0);
				dirty = true;
			};

			if (event.type == SDL_MOUSEWHEEL) {
				frame_index += (int)event.wheel.y;
				frame_index = ((frame_index < 0) ? 0 : frame_index);
				frame_index = ((frame_index >= matrices.size()) ? matrices.size()-1 : frame_index);
				std::cout << "frame_index : " << frame_index << "\n";
				dirty = true;
			};


			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFTBRACKET]) {
				frame_index -= 1;
				frame_index = ((frame_index < 0) ? 0 : frame_index);
				std::cout << "frame_index : " << frame_index << "\n";
				dirty = true;
			};
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHTBRACKET]) {
				frame_index += 1;
				frame_index = ((frame_index >= matrices.size()) ? matrices.size()-1 : frame_index);
				std::cout << "frame_index : " << frame_index << "\n";
				dirty = true;
			};


		};

		if (dirty) {
			// clear screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor3f(0.8, 0.0, 0.0);							

			// update uniforms
			glm::mat4 Projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 10.0f);
			glm::mat4 View = glm::lookAt(
				CameraPosition, // Camera is at (4,3,3), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
			glm::mat4 Model = matrices.at(frame_index).second; // original
			// glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(1,0,0));
			// print(Model);
			// Model = matrices.at(frame_index).second;
			// std::cout << "\n\n";
			// print(Model);

			transformation = Projection * View * Model; // original

			glUniformMatrix4fv(s.Uniform("u_transform"), 1, GL_FALSE, glm::value_ptr(transformation));
			

			// draw stuff
			s.Use();
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, vertex_data.size() / 6);
			glBindVertexArray(0);

			// swap buffers
			SDL_GL_SwapWindow(window);
			dirty = false;
		};
	};


	return 0;
};