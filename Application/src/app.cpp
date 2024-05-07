#include <app.h>
#include <engine/logger.h>
#include <render/gl_buffer.h>
#include <render/gl_camera.h>
#include <render/gl_program.h>
#include <render/gl_texture.h>
#include <render/gl_framebuffer.h>
#include <file_system/fs_helper.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

gl_camera global_camera(glm::vec3(0.0f, 0.0f, 3.0f));
gl_framebuffer m_scene_buffer;

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
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_window == nullptr) {
		glfwTerminate();
		std::exit(1);
	}

	//glfwSetWindowPos(m_window, 100, 100);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(m_window, app::on_resize_callback);

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);

	internal_init();
}

void app::update() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	m_scene_buffer.init(m_width, m_height);

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Begin("Scene");
		{
			if (ImGui::BeginChild("GameRender")) {
				float width = ImGui::GetContentRegionAvail().x;
				float height = ImGui::GetContentRegionAvail().y;

				m_scene_buffer.rescale(width, height);
				ImGui::Image(
					(ImTextureID)m_scene_buffer.get_texture().get_id(),
					ImGui::GetContentRegionAvail(),
					ImVec2(0, 1),
					ImVec2(1, 0)
				);
				ImGui::EndChild();
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		m_scene_buffer.bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_1.get_id());

		float width = m_scene_buffer.get_Width();
		float height = m_scene_buffer.get_height();

		glm::mat4 view = glm::mat4(1.0f);
		view = global_camera.get_view();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

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
		m_scene_buffer.unbind();
		glDisable(GL_DEPTH_TEST);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
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

void app::on_resize_callback(GLFWwindow* window, int width, int height) {
	app& instance = app::get_instance();
	glViewport(0, 0, width, height);
	instance.m_height = height;
	instance.m_width = width;
}

void app::on_error_callback(int error, const char* description) {
	logger::get_instance().err(description);
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

#ifdef _WIN32
LRESULT CALLBACK app::internal_proc(HWND h_wnd, UINT u_msg, WPARAM w_param, LPARAM l_param) {
	// Custom proc
	auto inst = app::get_instance();
	return CallWindowProc(inst.m_original_proc, h_wnd, u_msg, w_param, l_param);
}
#endif

void app::set_window_icon() {
	GLFWimage images[1];
	fs_helper& fs_inst = fs_helper::get_instance();
	std::string icon_path = (std::filesystem::path(fs_inst.get_root_path()) / "icon.png").string();

	stbi_set_flip_vertically_on_load(false);
	images[0].pixels = stbi_load(icon_path.c_str(), &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(m_window, 1, images);
	stbi_image_free(images[0].pixels);
	stbi_set_flip_vertically_on_load(true);
}

void app::internal_init() {
#ifdef _WIN32
	// 1. Get window handle
	HWND h_wnd = glfwGetWin32Window(m_window);
	
	// 2. Set custom proc
	m_original_proc = (WNDPROC)GetWindowLongPtr(h_wnd, GWLP_WNDPROC);
	(WNDPROC)SetWindowLongPtr(h_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app::internal_proc));

	// 3. Set icon
	set_window_icon();
	
	// 4. Check functions
	FARPROC dwm_set_win_attr = NULL;
	HMODULE library = LoadLibrary("dwmapi.dll");
	if (!library)
		return;

	dwm_set_win_attr = GetProcAddress(library, "DwmSetWindowAttribute");
	if (dwm_set_win_attr != NULL) {
		// Use Mica
		int use_mica = 1;
		DwmSetWindowAttribute(
			h_wnd, DWMWA_MICA_EFFECT,
			&use_mica, sizeof(int)
		);

		// Remove rounded corners
		//int attribute = DWMWCP_DONOTROUND;
		//DwmSetWindowAttribute(
		//	h_wnd, DWMWA_WINDOW_CORNER_PREFERENCE,
		//	&attribute, sizeof(int)
		//);

		COLORREF color = RGB(20, 20, 20);
		DwmSetWindowAttribute(
			h_wnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
			&color, sizeof(COLORREF)
		);
	}
#elif __unix__
	
#endif
}