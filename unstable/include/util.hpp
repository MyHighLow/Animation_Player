
glm::mat4 parse_jmat4_object(json j) {
	glm::mat4 ret;
	for (int i=0;i<4;i++) {
		for (int k=0;k<4;k++) {
			ret[i][k] = j[i][k];
		};
	};
	return ret;
};

std::vector<std::pair<int, glm::mat4>> parse_jmat4_animation(json j) {
	std::vector<std::pair<int, glm::mat4>> ret;
	for (auto it = j.begin(); it != j.end(); ++it) {
		ret.push_back(
			std::pair<int, glm::mat4>(
				std::stoi(it.key()),
				parse_jmat4_object(it.value())
			)
		);
	};

	// sort matrices by frame index
	std::sort(
		ret.begin(),
		ret.end(),
		[](const std::pair<int, glm::mat4> a, std::pair<int, glm::mat4> b) -> bool {
			return a.first < b.first;
		}
	);

	return ret;
};


void print(glm::vec4 v, bool newline = true) {
	std::cout << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
	if (newline) {
		std::cout << "\n";
	};
};

void print(glm::mat4 m, bool newline = true) {
	std::cout << "{\n\t";
	print(m[0]);
	std::cout << "\t";
	print(m[1]);
	std::cout << "\t";
	print(m[2]);
	std::cout << "\t";
	print(m[3]);
	std::cout << "\n}";
};

glm::mat4 scaling_matrix(float s) {
	glm::mat4 ret(
		{s,    0.0f, 0.0f, 0.0f},
		{0.0f, s,    0.0f, 0.0f},
		{0.0f, 0.0f, s,    0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	);
	return ret;
};

glm::mat4 offset_matrix(glm::vec3 offset) {
	glm::mat4 ret(
		{1.0f, 0.0f, 0.0f, offset.x},
		{0.0f, 1.0f, 0.0f, offset.y},
		{0.0f, 0.0f, 1.0f, offset.z},
		{0.0f, 0.0f, 0.0f, 1.0f}
	);
	return ret;
};


std::vector<glm::vec4> basic_parse_obj_file(const char *filepath, bool Verbose = false) {
	std::ifstream fs(filepath, std::ifstream::in);
	if (!fs.good()) {
		std::cout << "ifstream is not good : " << filepath << "\n";
	};
	if (!fs) {
		std::cout << "could not open " << filepath << "\n";
	};
	
	char lead;
	std::vector<glm::vec4> vertex_pool;
	std::vector<glm::ivec3> vertex_indices;
	
	while (!fs.eof()) {
		lead = fs.get();

		if (lead == 'v') {
			float x, y, z;
			fs >> x >> y >> z;
			glm::vec4 vector(x,y,z,1.0f);
			vertex_pool.push_back(vector);
			fs.get();
		} else
		if (lead == 'f') {
			float a, b, c;
			fs >> a >> b >> c;
			glm::ivec3 indices(a,b,c);
			vertex_indices.push_back(indices);
			fs.get();
		} else {
			if (Verbose) {
				while (!fs.eof() & fs.peek() != '\n') {
					std::cout << fs.get();
				};
				std::cout << "\n";
			} else {
				while (!fs.eof() & fs.peek() != '\n') {
					fs.get();
				};
			};

			fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (fs.rdstate() & std::istream::failbit) {
				break;
			};
		};
	};


	std::vector<glm::vec4> ret;

	for (glm::ivec3 index_triplet : vertex_indices) {
		ret.push_back(vertex_pool.at(index_triplet.x - 1));
		ret.push_back(vertex_pool.at(index_triplet.y - 1));
		ret.push_back(vertex_pool.at(index_triplet.z - 1));
	};

	return ret;
};