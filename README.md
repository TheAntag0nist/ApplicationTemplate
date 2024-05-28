# ApplicationTemplate
ApplicationTemplate - Template for different programs with 2D/3D.

# Short description
That part contains description for folders and headers in the project.

+ engine - folder with global 3D and 2D engine structures
+ file_system - folder contains classes for working with file system
+ loaders - library with different custom model | texture loaders
+ render - folder with low-level render implementations 
+ ui - folder with user interface widgets, controls and etc

# Resource Folder
Resource folder it's base folder for the program that contains shaders, materials and other resources.

+ user_space - folder for custom user purposes
+ shaders - folder with shaders for materials | or compute purposes
+ materials - folder with materials descriptions in XML format
+ textures - contains basic textures for project

# Start
For start new project check app.cpp and ui_core.cpp in ui folder.
Also you can check render_core.cpp in render folder.

# Roadmap
In the future will be added Vulkan API as render.
Now it's use opengl and ImGui Docking and Viewport branch.

# Third-Part
+ stb
+ glm
+ glad
+ gzip
+ ImGui
+ Boost
+ glfw3
+ implot
+ rapidxml
+ imguizmo
+ nlohmann_json
