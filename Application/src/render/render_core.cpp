#include <render/render_core.h>

render_core& render_core::get_instance() {
	static render_core core;
	return core;
}