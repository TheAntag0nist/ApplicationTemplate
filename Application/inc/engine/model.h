#ifndef MODEL_H
#define MODEL_H
#include <engine/mat_base.h>
#include <engine/mesh.h>
#include <global.h>

class model {
public:
	model() = default();
	virtual ~model() = default;

	void draw() const;
	void load_model(std::string filepath);

protected:
	std::vector<mesh> m_meshes;
	std::string m_filepath;
	mat_base m_material;

};

#endif