#ifndef MAT_CONTROLLER_H
#define MAT_CONTROLLER_H
#include <engine/mat_base.h>
#include <global.h>

/// <summary>
/// Class for controlling materials in project.
/// </summary>
class mat_controller {
public:
	static mat_controller& get_instance();
	virtual ~mat_controller() = default;

	void load_materials();
	void load_material(std::string material);

	void reload_materials();
	void reload_material(std::string mat_name);
	
	void release_materials();
	void release_material(std::string material);

protected:
	std::string get_shader_by_type(std::shared_ptr<rapidxml::xml_document<>> doc, std::string shader_type);
	std::shared_ptr<mat_base> inner_material_loader(std::filesystem::path& mat_path);
	std::string get_file_as_buffer(std::fstream& file);
	bool is_mat_file(std::string filaname) const;
	mat_controller() = default;

private:
	std::map<std::string, std::shared_ptr<mat_base>> m_materials;
};

#endif