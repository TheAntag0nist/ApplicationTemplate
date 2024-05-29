#include <resources/mat_controller.h>
#include <file_system/fs_helper.h>
#include <file_system/fs_helper.h>
#include <render/gl/gl_program.h>
#include <engine/logger.h>

mat_controller& mat_controller::get_instance() {
	static mat_controller instance;
	return instance;
}

void mat_controller::load_materials() {
	std::filesystem::path mat_path = fs_helper::get_instance().get_materials_path();

	for (auto file_it : std::filesystem::directory_iterator(mat_path)) {
		if (file_it.is_regular_file() && file_it.path().has_extension()
			&& is_mat_file(file_it.path().extension().string())) {
			std::filesystem::path file_path = file_it.path();
			std::filesystem::path filename = file_path.filename();
			std::string material_name = filename.replace_extension("").string();
			auto mat = inner_material_loader(file_path);
			m_materials.emplace(material_name, mat);
		}
	}
}

void mat_controller::load_material(std::string material) {
	std::filesystem::path mat_path = fs_helper::get_instance().get_materials_path();
	auto& logg_inst = logger::get_instance();
	mat_path /= material + ".mat";

	if (m_materials.find(material) != m_materials.end()) {
		logg_inst.err("Material already loaded");
		return;
	}

	std::shared_ptr<mat_base> mat = inner_material_loader(mat_path);
	m_materials.emplace(material, mat);
}

void mat_controller::reload_materials() {
	release_materials();
	load_materials();
}

void mat_controller::reload_material(std::string mat_name) {
	release_material(mat_name);
	load_material(mat_name);
}

void mat_controller::release_materials() {
	m_materials.clear();
}

void mat_controller::release_material(std::string material) {
	auto mat = m_materials.find(material);
	if (mat != m_materials.end())
		m_materials.erase(mat);
}


std::string mat_controller::get_shader_by_type(std::shared_ptr<rapidxml::xml_document<>> doc, std::string shader_type) {
	rapidxml::xml_node<>* root = doc->first_node("material");
	if (!root) {
		std::cerr << "No root node found." << std::endl;
		return "";
	}

	rapidxml::xml_node<>* shaders_node = root->first_node("shaders");
	if (!shaders_node) {
		std::cerr << "No shaders node found." << std::endl;
		return "";
	}

	for (rapidxml::xml_node<>* shader_node = shaders_node->first_node("shader"); shader_node; shader_node = shader_node->next_sibling("shader")) {
		rapidxml::xml_attribute<>* type_attr = shader_node->first_attribute("type");
		if (!type_attr) {
			std::cerr << "Shader node does not have a type attribute." << std::endl;
			continue;
		}
		std::string type_value = type_attr->value();

		// Если тип шейдера совпадает с заданным, возвращаем имя файла шейдера
		if (type_value == shader_type) {
			rapidxml::xml_attribute<>* name_attr = shader_node->first_attribute("name");
			if (!name_attr) {
				std::cerr << "Shader node does not have a name attribute." << std::endl;
				return "";
			}
			return name_attr->value();
		}
	}

	std::cerr << "Shader of type " << shader_type << " not found." << std::endl;
	return "";
}

std::shared_ptr<mat_base> mat_controller::inner_material_loader(std::filesystem::path& mat_path) {
	auto& logg_inst = logger::get_instance();
	auto& fs_ints = fs_helper::get_instance();
	std::fstream mat_file;

	mat_file.open(mat_path);
	if (mat_file.is_open()) {
		auto xml_doc = std::make_shared<rapidxml::xml_document<>>();
		std::string buffer = get_file_as_buffer(mat_file);
		xml_doc->parse<0>(&buffer[0]);

		std::filesystem::path shaders_path = fs_ints.get_shaders_path();
		auto vert_sh = (shaders_path / get_shader_by_type(xml_doc, "vert")).string();
		auto frag_sh = (shaders_path / get_shader_by_type(xml_doc, "frag")).string();

		auto mat = std::make_shared<gl_program>();
		mat->init(vert_sh.c_str(), frag_sh.c_str());
		return mat;
	}
		
	return nullptr;
}

std::string mat_controller::get_file_as_buffer(std::fstream& file) {
	std::string buffer = "";
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	buffer.reserve(fileSize);

	buffer.assign(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	);
	return buffer;
}

bool mat_controller::is_mat_file(std::string ext) const {
	if (ext == ".mat")
		return true;
	return false;
}