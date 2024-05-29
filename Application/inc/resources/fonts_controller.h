#ifndef FONTS_CONTROLLER_H
#define FONTS_CONTROLLER_H
#include <global.h>

class fonts_controller {
public:
	static fonts_controller& get_instance();

	void load_fonts();
	void load_font(std::string& fontname);
	void load_font_by_path(std::string& fontpath);

	const std::map<std::string, ImFont*> get_fonts_collection() const;
	ImFont* get_font_by_name(std::string name) const;
	void release_fonts();

protected:
	ImFont* inner_font_loader(std::string fontpath);

private:
	std::map<std::string, ImFont*> m_fonts;
	uint32_t m_default_font_size = 16;
	fonts_controller() = default;

};

#endif