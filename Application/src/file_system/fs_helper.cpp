#include <file_system/fs_helper.h>

fs_helper& fs_helper::get_instance() {
	static fs_helper instance;
	return instance;
}

std::string fs_helper::get_logger_path() const {
	return m_default_logger;
}

std::string fs_helper::get_current_dir() {
	return std::filesystem::current_path().string();
}

std::string fs_helper::get_root_path() const {
	return m_root;
}

std::string fs_helper::get_shaders_path() const {
	return m_shaders;
}

std::string fs_helper::get_textures_path() const {
	return m_textures;
}