#include <file_system/fs_helper.h>

fs_helper& fs_helper::get_instance() {
	static fs_helper instance;
	return instance;
}

std::string fs_helper::get_logger_path() const {
	std::filesystem::path main_path = m_root;
	main_path /= m_default_logger;
	return main_path.string();
}

std::string fs_helper::get_current_dir() {
	return std::filesystem::current_path().string();
}

std::string fs_helper::get_root_path() const {
	return m_root;
}

std::string fs_helper::get_shaders_path() const {
	std::filesystem::path main_path = m_root;
	main_path /= m_shaders;
	return main_path.string();
}

std::string fs_helper::get_materials_path() const {
	std::filesystem::path main_path = m_root;
	main_path /= m_materials;
	return main_path.string();
}

std::string fs_helper::get_textures_path() const {
	std::filesystem::path main_path = m_root;
	main_path /= m_textures;
	return main_path.string();
}