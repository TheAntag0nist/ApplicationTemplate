#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H
#include <engine/mat_base.h>
#include <global.h>

class gl_program : public mat_base {
public:
	gl_program() = default;
	virtual ~gl_program();

	virtual void init(const char* vert_path, const char* frag_path) override;

	void link();
	virtual void use_material();
	
	virtual void set_bool(const std::string& name, bool value) override;
	virtual void set_float(const std::string& name, float value) override;
	virtual void set_integer(const std::string& name, int value) override;
	virtual void set_vec2(const std::string& name, glm::vec2 value) override;
	virtual void set_vec2(const std::string& name, float x, float y) override;

	virtual void set_vec3(const std::string& name, glm::vec3 value) override;
	virtual void set_vec3(const std::string& name, float x, float y, float z) override;

	virtual void set_vec4(const std::string& name, glm::vec4 value) override;
	virtual void set_vec4(const std::string& name, float x, float y, float z, float w) override;

	virtual void set_mat2(const std::string& name, glm::mat2 value) override;
	virtual void set_mat3(const std::string& name, glm::mat3 value) override;
	virtual void set_mat4(const std::string& name, glm::mat4 value) override;

	uint32_t get_id() const;

protected:
	void check_compile_errors(uint32_t shader, std::string type);

private:
	uint32_t m_program_id;
};

#endif