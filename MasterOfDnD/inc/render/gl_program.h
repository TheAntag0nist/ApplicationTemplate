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

protected:
	void check_compile_errors(uint32_t shader, std::string type);

private:
	uint32_t m_program_id;
};

#endif