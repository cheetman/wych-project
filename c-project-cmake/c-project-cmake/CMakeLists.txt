# CMakeList.txt: c-project-cmake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#



aux_source_directory(. DIRSRCS)
# 将源代码添加到此项目的可执行文件。

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/glm/include
	SYSTEM C:/WorkEnv/VulkanSDK/1.2.189.2/Include
)

link_directories(
	SYSTEM C:/WorkEnv/VulkanSDK/1.2.189.2/Lib
)


add_executable(c-project-cmake  ${DIRSRCS})

target_link_libraries(c-project-cmake vulkan-1.lib)

if(WIN32)
  #set_target_properties(c-project-cmake PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()