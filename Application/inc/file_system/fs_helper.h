#ifndef FS_HELPER_H
#define FS_HELPER_H
#include <global.h>

class fs_helper {
public:
	static fs_helper& get_instance();
	~fs_helper() = default;

	std::string get_current_dir();

	std::string get_root_path() const;
	const char* get_root_path_cstr() const;

	std::string get_logger_path() const;
	std::string get_shaders_path() const;
	std::string get_textures_path() const;

protected:
	fs_helper() = default;

protected:

	const char* m_root = "./resources";
	const char* m_shaders = "./resources/shaders";
	const char* m_textures = "./resources/textures";
	const char* m_user_space = "./resources/user_space";
	const char* m_default_logger = "./resources/app_log.log";
};

#endif