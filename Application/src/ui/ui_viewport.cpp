#include <ui/ui_viewport.h>

ui_viewport::ui_viewport() {
	m_title = "Viewport";
	m_height = 800;
	m_width = 600;
}

void ui_viewport::init() {}

void ui_viewport::update()  {
	ImGui::Begin(m_title.c_str());
	{
		if (ImGui::BeginChild(m_title.c_str())) {
			m_width = ImGui::GetContentRegionAvail().x;
			m_height = ImGui::GetContentRegionAvail().y;

			m_framebuffer->rescale(m_width, m_height);
			auto tex_id = m_framebuffer->get_texture().get_id();
			ImGui::Image(
				reinterpret_cast<ImTextureID>(tex_id),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

void ui_viewport::destroy() {

}

void ui_viewport::set_framebuffer(std::shared_ptr<gl_framebuffer> framebuffer) {
	m_framebuffer = framebuffer;
}

std::shared_ptr<gl_framebuffer> ui_viewport::get_framebuffer() {
	return m_framebuffer;
}

void ui_viewport::set_title(const std::string& title) {
	m_title = title;
}

std::string ui_viewport::get_title() {
	return m_title;
}