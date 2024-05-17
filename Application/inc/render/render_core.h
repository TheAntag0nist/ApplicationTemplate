#ifndef RENDER_CORE_H
#define RENDER_CORE_H
#include <global.h>

class render_core {
public:
	static render_core& get_instance();
	virtual ~render_core() = default;

protected:
	render_core() = default;

};

#endif