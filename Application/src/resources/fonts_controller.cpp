#include <resources/fonts_controller.h>
#include <file_system/fs_helper.h>

fonts_controller& fonts_controller::get_instance() {
	static fonts_controller instance;
	return instance;
}

void fonts_controller::load_fonts() {
	auto& io = ImGui::GetIO(); (void)io;
	auto& fs_inst = fs_helper::get_instance();
	std::filesystem::path fonts_path = fs_inst.get_fonts_path();

	for (auto& file_it : std::filesystem::directory_iterator(fonts_path)) {
		auto file_path = file_it.path();
		auto file_ext = file_path.has_extension() ? file_path.extension() : "";
		auto file_name = file_path.filename().replace_extension("").string();
		
		if (file_it.is_regular_file() && file_ext == ".ttf") {
			ImFont* new_font = inner_font_loader(file_path.string());
			if (new_font != nullptr) m_fonts.emplace(file_name, new_font);
		}
	}

	io.Fonts->Build();
}

void fonts_controller::load_font(std::string& font_name) {
	auto& fs_inst = fs_helper::get_instance();
	std::filesystem::path fonts_path = fs_inst.get_fonts_path();
	auto font_path = fonts_path / (font_name + ".ttf");

	ImFont* font = inner_font_loader(font_path.string());
	if(font != nullptr) m_fonts.emplace(font_name, font);
}

void fonts_controller::load_font_by_path(std::string& fontpath) {
	std::filesystem::path path = fontpath;
	std::string file_name = path.filename().replace_extension("").string();

	ImFont* new_font = inner_font_loader(fontpath);
	if (new_font != nullptr) m_fonts.emplace(file_name, new_font);
}

const std::map<std::string, ImFont*> fonts_controller::get_fonts_collection() const {
	return m_fonts;
}

ImFont* fonts_controller::get_font_by_name(std::string name) const {
	auto item = m_fonts.find(name);
	if (item != m_fonts.end())
		return item->second;
	return nullptr;
}

void fonts_controller::release_fonts() {
	auto& io = ImGui::GetIO(); (void)io;
	io.Fonts->Clear();
	m_fonts.clear();
}

ImFont* fonts_controller::inner_font_loader(std::string fontpath) {
	auto& io = ImGui::GetIO(); (void)io;
	ImFont* new_font = io.Fonts->AddFontFromFileTTF(
		fontpath.c_str(), m_default_font_size
	);
	return new_font;
}
