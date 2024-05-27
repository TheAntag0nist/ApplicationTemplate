#ifndef BASE_TEXTURE_H
#define BASE_TEXTURE_H
#include <global.h>

class base_texture {
protected:
    std::string m_image_path;
    uint32_t m_texture_id;
    std::string m_type;

public:
    std::string get_path() const;
    std::string get_type() const;
    uint32_t get_id() const;
};

#endif