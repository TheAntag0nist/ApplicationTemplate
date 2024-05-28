#ifndef UI_VIEWPORT_H
#define UI_VIEWPORT_H
#include <render/gl/gl_framebuffer.h>
#include <ui/ui_layout.h>
#include <global.h>

class ui_viewport : public ui_layout {
public:
	ui_viewport();
	virtual ~ui_viewport() = default;

	virtual void init() override;
	virtual void update() override;
	virtual void destroy() override;

	void set_title(const std::string& title);
	std::string get_title();

	void set_framebuffer(std::shared_ptr<gl_framebuffer> framebuffer);
	std::shared_ptr<gl_framebuffer> get_framebuffer();

protected:
	std::shared_ptr<gl_framebuffer> m_framebuffer;
	std::string m_title;
	float m_height;
	float m_width;

};

#endif