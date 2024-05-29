#include <app.h>
#include <ui/ui_core.h>
#include <engine/logger.h>
#include <render/render_core.h>
#include <file_system/fs_helper.h>
#include <resources/mat_controller.h>
#include <resources/fonts_controller.h>

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

	// Internal window init
	internal_init();
	 
	// Init subsystems
	auto& render_inst = render_core::get_instance();
	auto& ui_inst = ui_core::get_instance();
	render_inst.init();
	ui_inst.init();

	io.DisplaySize;
	io.DisplayFramebufferScale;
}

void app::update() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	auto& fonts_ctrl_inst = fonts_controller::get_instance();
	auto& mat_ctrl_inst = mat_controller::get_instance();

	auto& render_inst = render_core::get_instance();
	auto& ui_inst = ui_core::get_instance();

	mat_ctrl_inst.load_materials();
	fonts_ctrl_inst.load_fonts();

	while (!glfwWindowShouldClose(m_window)) {
		float current_frame = static_cast<float>(glfwGetTime());
		m_delta_time = current_frame - m_last_frame;
		m_last_frame = current_frame;

		render_inst.set_height(m_height);
		render_inst.set_width(m_width);

		render_inst.update();
		ui_inst.update();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void app::destroy() {
	render_core::get_instance().destroy();
	ui_core::get_instance().destroy();
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
		int attribute = DWMWCP_DONOTROUND;
		DwmSetWindowAttribute(
			h_wnd, DWMWA_WINDOW_CORNER_PREFERENCE,
			&attribute, sizeof(int)
		);

		COLORREF color = RGB(20, 20, 20);
		DwmSetWindowAttribute(
			h_wnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
			&color, sizeof(COLORREF)
		);
	}
#elif __unix__
	
#endif
}