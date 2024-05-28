#ifndef MAT_BASE_H
#define MAT_BASE_H
#include <global.h>

/// <summary>
/// Base class for all materials in project.
/// Should be used with implemented versions of gl_program and vk_material.
/// </summary>
class mat_base {
public:
	mat_base() = default;
	virtual ~mat_base() = default;

	virtual void use_material() = 0;
	virtual void init(const char* vert_path, const char* frag_path) = 0;

	virtual void set_bool(const std::string& name, bool value) = 0;
	virtual void set_float(const std::string& name, float value) = 0;
	virtual void set_integer(const std::string& name, int value) = 0;
	virtual void set_vec2(const std::string& name, glm::vec2 value) = 0;
	virtual void set_vec2(const std::string& name, float x, float y) = 0;

	virtual void set_vec3(const std::string& name, glm::vec3 value) = 0;
	virtual void set_vec3(const std::string& name, float x, float y, float z) = 0;

	virtual void set_vec4(const std::string& name, glm::vec4 value) = 0;
	virtual void set_vec4(const std::string& name, float x, float y, float z, float w) = 0;

	virtual void set_mat2(const std::string& name, glm::mat2 value) = 0;
	virtual void set_mat3(const std::string& name, glm::mat3 value) = 0;
	virtual void set_mat4(const std::string& name, glm::mat4 value) = 0;
};

#endif