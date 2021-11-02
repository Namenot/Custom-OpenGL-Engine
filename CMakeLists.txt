# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (3D_Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o3")

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

#if(INCLUDE_DISTRIB)
#	add_subdirectory(distrib)
#endif(INCLUDE_DISTRIB)


#no idea whether i need this
include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

#main build
add_executable(3D_Engine
	#main target
	3D_Engine/main.cpp
	3D_Engine/mainloop.hpp

	#my headers
	3D_Engine/customheaders/camcontroller.hpp
	#camcontrolls.cpp

	#my headers (not properly defined)
	3D_Engine/customheaders/allheaders.hpp
	3D_Engine/customheaders/mesh.hpp
	3D_Engine/customheaders/basewindow.hpp
	3D_Engine/customheaders/noisegenerator.hpp
	3D_Engine/customheaders/pclock.hpp
	3D_Engine/customheaders/perlinnoisegenerator.hpp
	3D_Engine/customheaders/Clockclass.hpp
	3D_Engine/customheaders/polarmesh.hpp
	3D_Engine/customheaders/renderer.hpp
	3D_Engine/customheaders/terminal.hpp
	3D_Engine/customheaders/meshdivider.hpp

	#external headers
	common/shader.cpp
	common/shader.hpp
	#common/controls.cpp
	#common/controls.hpp


	#shaders (needs to be shifted into a seperat folder)
	3D_Engine/shaders/TransformVertexShader.vertexshader
	3D_Engine/shaders/ColorFragmentShader.fragmentshader

)

target_link_libraries(3D_Engine
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(3D_Engine PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3D_Engine/")
create_target_launcher(3D_Engine WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3D_Engine/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET 3D_Engine POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/3D_Engine${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/3D_Engine/bin"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )