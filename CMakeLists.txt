# Set minimum Version
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Set Project Name
set(PROJECT_NAME MasterOfDnDSolution)

# Minimal C++ version
set(CMAKE_CXX_STANDARD 17)

# Set Project
project (
	${PROJECT_NAME} 
	VERSION 0.0.0 
	DESCRIPTION "MasterOfDnDSolution - all libraries and code base for game")

# Subdirectories
add_subdirectory(./MasterOfDnD MasterOfDnD)