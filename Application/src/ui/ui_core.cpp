#include <ui/ui_core.h>

ui_core& ui_core::get_instance() {
	static ui_core core;
	return core;
}

void ui_core::update() {

}

void ui_core::add_layout(ui_layout_ptr layout) {
	
}

void ui_core::find_layout(boost::uuids::uuid uudi) {

}

void ui_core::remove_layout(boost::uuids::uuid uuid) {

}