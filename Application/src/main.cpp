#include <app.h>

int main() {
	auto& instance = app::get_instance();
	instance.init();
	instance.update();
	instance.destroy();
}