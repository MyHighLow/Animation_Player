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

#define PREFIX "D:/usr/local/src/animation_player/v0.1"


int main(int argc, char **argv) {

	// load ply data
	std::pair<std::vector<float>, std::vector<int>> ply_data = parse_ascii_ply(PREFIX "/data/model3.ply");
	std::vector<float> ply_v_data = ply_data.first;
	std::vector<int> ply_i_data = ply_data.second;

	std::cout << "ply_v_data :\n";

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
	// glBufferData(GL_ARRAY_BUFFER, 4*vertex_data.size(), vertex_data.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, ply_v_data.size() * 4, ply_v_data.data(), GL_STATIC_DRAW);
	glBindVertexArray(vao);
	// s.JsonAttribPointers(PREFIX "/data/shaders/colored_triangle/attributes.json");
		s.EasyAttribPointer("a_position", 3, 32, 0);
		s.EasyAttribPointer("a_color", 3, 32, 12); // this is not true -- change after testing

	// index
		GLuint ivbo;
		glGenBuffers(1, &ivbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ply_i_data.size() * 4, ply_i_data.data(), GL_STATIC_DRAW);

	// cleanup
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


		};

		if (dirty) {
			// clear screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor3f(0.8, 0.0, 0.0);							

			// update uniforms
			glm::mat4 Projection = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 10.0f);
			glm::mat4 View = glm::lookAt(CameraPosition, glm::vec3(0,0,0), glm::vec3(0,1,0));
			glm::mat4 Model = matrices.at(frame_index).second; // original
			transformation = Projection * View * Model; // original
			glUniformMatrix4fv(s.Uniform("u_transform"), 1, GL_FALSE, glm::value_ptr(transformation));

			// draw stuff
			s.Use();
			glBindVertexArray(vao);
			// glDrawArrays(GL_TRIANGLES, 0, vertex_data.size() / 6);
				glDrawElements(GL_TRIANGLES, ply_i_data.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// swap buffers
			SDL_GL_SwapWindow(window);
			dirty = false;
		};
	};


	return 0;
};