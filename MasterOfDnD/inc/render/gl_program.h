#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H
#include <global.h>

class gl_program {
public:
	gl_program() = default;
	~gl_program();

	void init(const char* vert_path, const char* frag_path);

	void use();
	void set_bool(const std::string& name, bool value);
	void set_float(const std::string& name, float value);
	void set_integer(const std::string& name, int value);

	void set_vec2(const std::string& name, glm::vec2 value);
	void set_vec2(const std::string& name, float x, float y);

	void set_vec3(const std::string& name, glm::vec3 value);
	void set_vec3(const std::string& name, float x, float y, float z);

	void set_vec4(const std::string& name, glm::vec4 value);
	void set_vec4(const std::string& name, float x, float y, float z, float w);

	void set_mat2(const std::string& name, glm::mat2 value);
	void set_mat3(const std::string& name, glm::mat3 value);
	void set_mat4(const std::string& name, glm::mat4 value);

	uint32_t get_id() const;

protected:
	void check_compile_errors(uint32_t shader, std::string type);

private:
	uint32_t m_program_id;
};

#endif