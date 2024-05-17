#ifndef UI_CORE_H
#define UI_CORE_H
#include <ui/ui_layout.h>
#include <global.h>

class ui_core {
public:
	static ui_core& get_instance();
	virtual ~ui_core() = default;

	void update();

	void add_layout(ui_layout_ptr layout);
	void find_layout(boost::uuids::uuid uudi);
	void remove_layout(boost::uuids::uuid uuid);

protected:
	std::map<boost::uuids::uuid, ui_layout_ptr> m_layouts;

private:
	ui_core() = default;

};

#endif