# CMakeList.txt: c-project-cmake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#


add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")


aux_source_directory(. DIRSRCS)
# 将源代码添加到此项目的可执行文件。

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/glm/include
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/tinygltf
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/ktx/include
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/ktx/other_include
	SYSTEM C:/WorkEnv/VulkanSDK/1.2.189.2/Include
)


link_directories(
	SYSTEM C:/WorkEnv/VulkanSDK/1.2.189.2/Lib
)





add_subdirectory(base)

add_executable(c-project-cmake  ${DIRSRCS})

target_link_libraries(c-project-cmake base vulkan-1.lib)

if(WIN32)
  #set_target_properties(c-project-cmake PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()