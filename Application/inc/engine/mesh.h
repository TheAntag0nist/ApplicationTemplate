#ifndef MESH_H
#define MESH_H
#include <engine/base_texture.h>
#include <engine/base_vertex.h>
#include <render/render_core.h>
#include <engine/mat_base.h>
#include <global.h>

class mesh {
public:
    mesh() = default;
    mesh(std::vector<base_vertex>& vertexes, std::vector<uint32_t>& indexes, std::vector<base_texture>& textures);
    virtual ~mesh() = default;
    
    void init();
    void destroy();
    void draw(mat_base& material);

protected:
    std::vector<base_texture> m_textures;
    std::vector<base_vertex>  m_vertexes;
    std::vector<uint32_t>     m_indexes;

    gl_buffer m_vertex_buffer;
    gl_buffer m_index_buffer;
    uint32_t  m_vertex_array;
};


#endif