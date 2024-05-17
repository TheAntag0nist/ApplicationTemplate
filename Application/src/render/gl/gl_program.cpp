#include <render/gl/gl_program.h>

gl_program::~gl_program() {
    glDeleteProgram(m_program_id);
}

void gl_program::init(const char* vert_path, const char* frag_path) {
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        // open files
        v_shader_file.open(vert_path);
        f_shader_file.open(frag_path);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << v_shader_file.rdbuf();
        fShaderStream << f_shader_file.rdbuf();
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();
        // convert stream into string
        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* v_shader_code_cstr = vertex_code.c_str();
    const char* f_shader_code_cstr = fragment_code.c_str();
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code_cstr, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code_cstr, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // shader Program
    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, vertex);
    glAttachShader(m_program_id, fragment);
    glLinkProgram(m_program_id);
    check_compile_errors(m_program_id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void gl_program::use() {
	glUseProgram(m_program_id);
}

void gl_program::set_bool(const std::string& name, bool value) {
	auto location = glGetUniformLocation(m_program_id, name.c_str());
	glUniform1i(location, static_cast<int>(value));
}

void gl_program::set_float(const std::string& name, float value) {
	auto location = glGetUniformLocation(m_program_id, name.c_str());
	glUniform1f(location, value);
}

void gl_program::set_integer(const std::string& name, int value) {
	auto location = glGetUniformLocation(m_program_id, name.c_str());
	glUniform1i(location, value);
}

void gl_program::set_vec2(const std::string& name, glm::vec2 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniform2fv(location, 1, &value[0]);
}

void gl_program::set_vec2(const std::string& name, float x, float y) {
    auto value = glm::vec2(x, y);
    set_vec2(name, value);
}

void gl_program::set_vec3(const std::string& name, glm::vec3 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniform3fv(location, 1, &value[0]);
}

void gl_program::set_vec3(const std::string& name, float x, float y, float z) {
    auto value = glm::vec3(x, y, z);
    set_vec3(name, value);
}

void gl_program::set_vec4(const std::string& name, glm::vec4 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniform4fv(location, 1, &value[0]);
}

void gl_program::set_vec4(const std::string& name, float x, float y, float z, float w) {
    auto value = glm::vec4(x, y, z, w);
    set_vec4(name, value);
}

void gl_program::set_mat2(const std::string& name, glm::mat2 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void gl_program::set_mat3(const std::string& name, glm::mat3 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void gl_program::set_mat4(const std::string& name, glm::mat4 value) {
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

uint32_t gl_program::get_id() const {
    return m_program_id;
}

void gl_program::check_compile_errors(uint32_t shader, std::string type) {
    char info_log[1024];
    int success;

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}