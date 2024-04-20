#ifndef APP_H
#define APP_H
#include <global.h>

class app {
public:
	void init();
	void update();
	void destroy();

	static app& get_instance();

protected:
	app() = default;
	virtual ~app() = default;
	void process_input(GLFWwindow* window);
	
	static void on_scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
	static void on_mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
	
	static void on_resize_callback(GLFWwindow* window, int width, int height);
	static void on_error_callback(int error, const char* description);

	static void show_console();
	static void hide_console();

protected:
	const char* m_title = "MasterOfDnD";
	GLFWwindow* m_window = nullptr;
	float m_delta_time = 0.0f;
	float m_last_frame = 0.0f;

	int m_height = 600;
	int m_width = 800;

	bool m_ignore_mouse_input = true;
	float m_last_y = m_height / 2.0f;
	float m_last_x = m_width / 2.0f;
	bool m_first_mouse = true;
};

#endif