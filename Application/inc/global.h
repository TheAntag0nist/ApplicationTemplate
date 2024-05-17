#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _WIN32
	#include <windows.h>
	#include <dwmapi.h>

	#pragma comment(lib, "dwmapi.lib")
	#define DWMWA_MICA_EFFECT DWORD(1029)
	#define DWMWA_WINDOW_CORNER_PREFERENCE DWORD(33)
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <stb_image.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp> 

#include <thread>
#include <mutex>

#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <string>
#include <list>
#include <map>

#endif