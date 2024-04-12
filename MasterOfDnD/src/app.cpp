#include <app.h>
#include <render/gl_buffer.h>
#include <render/gl_program.h>
#include <file_system/fs_helper.h>

void app::init() {
#ifdef NDEBUG
	hide_console();
#endif

	glfwSetErrorCallback(on_error_callback);
	if (glfwInit() != GLFW_TRUE)
		std::exit(1);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_window == nullptr) {
		glfwTerminate();
		std::exit(1);
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, app::on_resize_callback);

	stbi_set_flip_vertically_on_load(true);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		std::exit(1);
	}
}

void app::update() {
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	gl_program base_program;
	auto fs = fs_helper::get_instance();
	std::string vert_path = (std::filesystem::path(fs.get_current_dir()) / "resources" / "shaders" / "base.vert").string();
	std::string frag_path = (std::filesystem::path(fs.get_current_dir()) / "resources" / "shaders" / "base.frag").string();
	base_program.init(vert_path.c_str(), frag_path.c_str());

	uint32_t VAO;
	gl_buffer vertex_buff;
	gl_buffer indexes_buff;
	glGenVertexArrays(1, &VAO);
	vertex_buff.init(GL_ARRAY_BUFFER);
	indexes_buff.init(GL_ELEMENT_ARRAY_BUFFER);

	glBindVertexArray(VAO);

	vertex_buff.bind();
	vertex_buff.write<float>(vertices, 12);
	
	indexes_buff.bind();
	indexes_buff.write(indices, 6);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// VAO close
	vertex_buff.unbind();
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(m_window)) {
		glViewport(0, 0, m_width, m_height);
		glClear(GL_COLOR_BUFFER_BIT);

		base_program.use();
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void app::destroy() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void app::on_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void app::on_error_callback(int error, const char* description) {
	std::cout << "[ERR]:> " << description << std::endl;
}

void app::show_console() {
#ifdef _WIN32
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
}

void app::hide_console() {
#ifdef _WIN32
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
}