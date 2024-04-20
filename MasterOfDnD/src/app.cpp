#include <app.h>
#include <engine/logger.h>
#include <render/gl_buffer.h>
#include <render/gl_camera.h>
#include <render/gl_program.h>
#include <render/gl_texture.h>
#include <file_system/fs_helper.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

gl_camera global_camera(glm::vec3(0.0f, 0.0f, 3.0f));

void app::init() {
	fs_helper& fs_inst = fs_helper::get_instance();
	logger& log_inst = logger::get_instance();
	
	auto log_path = fs_inst.get_logger_path();
	log_inst.set_file(log_path);

	glfwSetErrorCallback(on_error_callback);
	if (glfwInit() != GLFW_TRUE)
		std::exit(1);

#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_window == nullptr) {
		glfwTerminate();
		std::exit(1);
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(m_window, app::on_resize_callback);
	//glfwSetCursorPosCallback(m_window, app::on_mouse_callback);
	//glfwSetScrollCallback(m_window, app::on_scroll_callback);

	stbi_set_flip_vertically_on_load(true);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		log_inst.err("Failed to initialize GLAD");
		glfwTerminate();
		std::exit(1);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void app::update() {
	float vertices[] = {
		// positions         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	gl_program base_program;
	auto fs = fs_helper::get_instance();
	std::string vert_path = (std::filesystem::path(fs.get_shaders_path()) / "base.vert").string();
	std::string frag_path = (std::filesystem::path(fs.get_shaders_path()) / "base.frag").string();
	base_program.init(vert_path.c_str(), frag_path.c_str());

	uint32_t VAO;
	gl_buffer vertex_buff;
	gl_buffer indexes_buff;
	glGenVertexArrays(1, &VAO);
	vertex_buff.init(GL_ARRAY_BUFFER);
	indexes_buff.init(GL_ELEMENT_ARRAY_BUFFER);

	glBindVertexArray(VAO);
	vertex_buff.bind();
	vertex_buff.write<float>(vertices, sizeof(vertices));

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	indexes_buff.bind();
	indexes_buff.write(indices, 6);

	// VAO close
	vertex_buff.unbind();
	glBindVertexArray(0);

	gl_texture texture_1;
	std::string texture_path = (std::filesystem::path(fs.get_textures_path()) / "skybox.jpg").string();
	texture_1.init(texture_path.c_str());

	base_program.use();
	glUniform1i(glGetUniformLocation(base_program.get_id(), "texture1"), 0);

	while (!glfwWindowShouldClose(m_window)) {
		float current_frame = static_cast<float>(glfwGetTime());
		m_delta_time = current_frame - m_last_frame;
		m_last_frame = current_frame;

		//process_input(m_window);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool value = true;
		ImGui::ShowDemoWindow(&value);

		ImGui::Render();
		glViewport(0, 0, m_width, m_height);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_1.get_id());

		glm::mat4 view = glm::mat4(1.0f); 
		view = global_camera.get_view();
		
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
		
		base_program.set_mat4("projection", projection); 
		base_program.set_mat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		float angle = 20.0f * glfwGetTime();
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		base_program.use();
		base_program.set_mat4("model", model);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);
	}
}

void app::destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

app& app::get_instance() {
	static app instance;
	return instance;
}

void app::on_mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	app& instance = app::get_instance();
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (instance.m_ignore_mouse_input)
		return;

	if (instance.m_first_mouse)
	{
		instance.m_last_x = xpos;
		instance.m_last_y = ypos;
		instance.m_first_mouse = false;
		return;
	}

	float xoffset = xpos - instance.m_last_x;
	float yoffset = instance.m_last_y - ypos;

	instance.m_last_x = xpos;
	instance.m_last_y = ypos;

	global_camera.process_mouse_movement(xoffset, yoffset);
}

void app::on_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	global_camera.process_mouse_scroll(static_cast<float>(yoffset));
}

void app::on_resize_callback(GLFWwindow* window, int width, int height) {
	app& instance = app::get_instance();
	glViewport(0, 0, width, height);
	instance.m_height = height;
	instance.m_width = width;
}

void app::on_error_callback(int error, const char* description) {
	logger::get_instance().err(description);
}

void app::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_ignore_mouse_input = false;
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_ignore_mouse_input = true;
		m_first_mouse = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		global_camera.process_keyboard(FORWARD, m_delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		global_camera.process_keyboard(BACKWARD, m_delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		global_camera.process_keyboard(LEFT, m_delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		global_camera.process_keyboard(RIGHT, m_delta_time);
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