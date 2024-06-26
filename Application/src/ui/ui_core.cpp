#include <ui/ui_core.h>
#include <ui/ui_nav_element.h>
#include <resources/fonts_controller.h>

ui_core& ui_core::get_instance() {
	static ui_core core;
	return core;
}

void ui_core::init() {
	auto& fonts_ctrl_inst = fonts_controller::get_instance();
	m_main_font_name = "Libertine_Regular";
	m_main_font = fonts_ctrl_inst.get_font_by_name(m_main_font_name);

	//render_core& render_inst = render_core::get_instance();
	//std::shared_ptr<ui_viewport> viewport(new ui_viewport());
	//viewport->set_framebuffer(render_inst.get_framebuffer());
	//add_layout(viewport);
}

void ui_core::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO io = ImGui::GetIO(); (void) io;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushFont(m_main_font);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));
	ImGui::Begin("Main", nullptr,
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBringToFrontOnFocus);
	
	for (auto& item : m_layouts) {
		ui_layout_ptr& layout = item.second;
		layout->update();
	}

	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ui_nav_element::begin_child("walking thru", size);

	bool temp;
	ImGui::Checkbox("Toggle", &temp);
	ImGui::Checkbox("Toggle 1", &temp);

	ui_nav_element::end_child();
	ImGui::PopStyleVar();

	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::PopFont();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ui_core::destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ui_core::add_layout(ui_layout_ptr layout) {
	m_layouts[layout->get_uuid()] = layout;
}

ui_layout_ptr ui_core::find_layout(boost::uuids::uuid uuid) {
	if (m_layouts.find(uuid) != m_layouts.end())
		return m_layouts[uuid];
	return nullptr;
}

void ui_core::remove_layout(boost::uuids::uuid uuid) {
	if (m_layouts.find(uuid) != m_layouts.end())
		m_layouts.erase(uuid);
}