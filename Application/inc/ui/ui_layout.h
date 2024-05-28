#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H
#include <global.h>

class ui_layout {
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void destroy() = 0;

	boost::uuids::uuid get_uuid() const {
		return m_uuid;
	}

protected:
	ui_layout() {
		m_uuid = boost::uuids::random_generator()();
	}

private:
	boost::uuids::uuid m_uuid;
};

typedef std::shared_ptr<ui_layout> ui_layout_ptr;

#endif