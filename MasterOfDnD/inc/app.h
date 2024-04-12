#ifndef APP_H
#define APP_H
#include <global.h>

class app {
public:
	void init();
	void update();
	void destroy();

protected:
	static void on_resize_callback(GLFWwindow* window, int width, int height);
	static void on_error_callback(int error, const char* description);
	static void show_console();
	static void hide_console();

protected:
	GLFWwindow* m_window;
	const char* m_title = "MasterOfDnD";
	
	int m_width = 800;
	int m_height = 600;
};

#endif