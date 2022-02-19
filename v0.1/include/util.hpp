
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



// returns text in a string object until a whtiespace character is reached. does not consume the whitespace from the stream
std::string read_until_whitespace(std::ifstream &fs, int error_size = -1) {
	std::string ret = "";
	while (true) {
		if (fs.peek() == ' ' || fs.peek() == '\t' || fs.peek() == '\n' || fs.peek() == '\r') {
			return ret;
		};
		if (error_size > 0 && ret.size() > error_size) {
			std::cout << "ERROR : read_until_whitespace reached error size. returning early\n";
			return ret;
		};
		ret += fs.get();
	};
};

// consumes text until the next character read will be on a new line
void go_to_next_line(std::ifstream &fs) {
	while (fs.peek() != '\n') {
		fs.get();
	};
	fs.get();
	if (fs.peek() == '\r') {
		fs.get();
	};
	return;
};

// returns how many bytes are left in a filestream
int bytes_left(std::ifstream &fs) {
	int a = fs.tellg();
	fs.seekg(0, std::ifstream::end);
	int b = fs.tellg();
	fs.seekg(a, std::ifstream::beg);
	return b - a;
};

// returns true if a is in b
template <typename T>
bool is_in(std::vector<T> b, T a) {
	for (int i=0; i<b.size();i++) {
		if (b.at(i) == a) {
			return true;
		};
	};
	return false;
};

template <typename T>
int index_of(std::vector<T> b, T a) {
	for (int i=0;i<b.size();i++) {
		if (b.at(i) == a) {
			return i;
		};
	};
	return -1;
};

int get_data_type_size_bytes(std::string data_type) {
	if (data_type.compare("char") == 0 || data_type.compare("uchar") == 0) {
		return 1;
	} else
	if (data_type.compare("short") == 0 || data_type.compare("ushort") == 0) {
		return 2;
	} else
	if (data_type.compare("int") == 0 || data_type.compare("uint") == 0 || data_type.compare("float") == 0) {
		return 4;
	} else
	if (data_type.compare("double") == 0) {
		return 8;
	} else {
		return 0;
	};
};


std::pair<std::vector<uint8_t>, std::vector<int>> better_parse_ply_file(const char *filepath, bool Verbose = false) {
	// open file
	std::ifstream fs(filepath, std::ifstream::in);
	if (!fs.good()) {
		std::cout << "ifstream is not good : " << filepath << "\n";
	};
	if (!fs) {
		std::cout << "could not open " << filepath << "\n";
	};

	std::string current_element = "";
	int num_verts = 0;
	int num_faces = 0;
	int vert_size_bytes = 0;

	// parse header
	while (!fs.eof()) {
		std::string lead_word = read_until_whitespace(fs);
		// std::cout << "lead word : " << lead_word << "\n";
		if (lead_word.compare("comment") == 0) {
			go_to_next_line(fs);
		} else
		if (lead_word.compare("element") == 0) {
			fs.get(); // burn whitespace
			current_element = read_until_whitespace(fs);
			fs.get(); // burn whitespace
			int tmp_i_num = stoi(read_until_whitespace(fs));
			if (current_element.compare("vertex") == 0) {
				num_verts = tmp_i_num;
			} else
			if (current_element.compare("face") == 0) {
				num_faces = tmp_i_num;
			};
			go_to_next_line(fs);
		} else 
		if (lead_word.compare("property") == 0) {
			if (current_element.compare("vertex") == 0) {
				fs.get(); // burn whitespace
				vert_size_bytes += get_data_type_size_bytes(read_until_whitespace(fs));
				go_to_next_line(fs);
			};
		} else 
		if (lead_word.compare("end_header") == 0) {
			std::cout << "reached the end of the header\n";
			std::cout << "remaining bytes : " << bytes_left(fs) << "\n"; // this isn't necessary but is being used for debugging
			go_to_next_line(fs);
			break;
		} else {
			go_to_next_line(fs);
		};
	};

	// print header summary
	std::cout << "num verts : " << num_verts << "\n";
	std::cout << "num faces : " << num_faces << "\n";
	std::cout << "vertex byte size : " << vert_size_bytes << "\n";

	// parse vertex data
	std::vector<uint8_t> raw_v_data;
	raw_v_data.reserve(num_verts * vert_size_bytes);
	for (int i=0;i<num_verts * vert_size_bytes;i++) {
		raw_v_data.push_back(fs.get());
	};
	std::cout << "done parsing vertex data\n";
	std::cout << "remaining bytes : " << bytes_left(fs) << "\n";

	// parse face data
	std::vector<int> indices;
	fs.get(); fs.get(); // for some reason, two bytes must be burnt... idk why
	for (int i=0; i<num_faces;i++) {
		uint8_t num_verts_in_face;
		fs >> num_verts_in_face;
		std::cout << "face (" << (unsigned int)(num_verts_in_face) << ") : ";
		for (int k=0;k<num_verts_in_face; k++) {
			unsigned int tmp;
			fs.read((char *)(&tmp), 4);
			std::cout << tmp << ", ";
			indices.push_back(tmp);
		};
		std::cout << "\n";
	};

	// construct final buffer
	std::pair<std::vector<uint8_t>, std::vector<int>> ret;
	ret.first = raw_v_data;
	ret.second = indices;
	
	return ret;
};


std::pair<std::vector<float>, std::vector<int>> parse_ascii_ply(const char *filepath, bool Verbose = false) {
// open file
	std::ifstream fs(filepath, std::ifstream::in);
	if (!fs.good()) {
		std::cout << "ifstream is not good : " << filepath << "\n";
	};
	if (!fs) {
		std::cout << "could not open " << filepath << "\n";
	};

	// metadata
	std::string current_element = "";
	int num_verts = 0;
	int num_faces = 0;
	int vert_size_bytes = 0;

	// parse header
	while (!fs.eof()) {
		std::string lead_word = read_until_whitespace(fs);
		std::cout << "lead word : " << lead_word << "\n";
		if (lead_word.compare("comment") == 0) {
			go_to_next_line(fs);
		} else
		if (lead_word.compare("element") == 0) {
			fs.get(); // burn whitespace
			current_element = read_until_whitespace(fs);
			fs.get(); // burn whitespace
			int tmp_i_num = stoi(read_until_whitespace(fs));
			if (current_element.compare("vertex") == 0) {
				num_verts = tmp_i_num;
			} else
			if (current_element.compare("face") == 0) {
				num_faces = tmp_i_num;
			};
			go_to_next_line(fs);
		} else 
		if (lead_word.compare("property") == 0) {
			if (current_element.compare("vertex") == 0) {
				fs.get(); // burn whitespace
				vert_size_bytes += get_data_type_size_bytes(read_until_whitespace(fs));
			};
			go_to_next_line(fs);
		} else 
		if (lead_word.compare("end_header") == 0) {
			std::cout << "reached the end of the header\n";
			std::cout << "remaining bytes : " << bytes_left(fs) << "\n"; // this isn't necessary but is being used for debugging
			go_to_next_line(fs);
			break;
		} else {
			go_to_next_line(fs);
		};
	};

	// print header summary
	std::cout << "\n\n";
	std::cout << "num verts : " << num_verts << "\n";
	std::cout << "num faces : " << num_faces << "\n";
	std::cout << "vertex byte size : " << vert_size_bytes << "\n";

	std::cout << "\n\n";

	// parse vertex data
	std::vector<float> vertex_data;
	for (int i=0;i<num_verts;i++) {
		std::string tmp = read_until_whitespace(fs);
		fs.get();
		std::cout << tmp << " ";
		tmp = read_until_whitespace(fs);
		fs.get();
		std::cout << tmp << "\n";

		
		go_to_next_line(fs);
		
	};

	// parse face indices
	std::vector<int> face_indices;
	for (int i=0;i<num_faces;i++) {

	};


	std::pair<std::vector<float>, std::vector<int>> ret;
	ret.first = vertex_data;
	ret.second = face_indices;

	return ret;
};