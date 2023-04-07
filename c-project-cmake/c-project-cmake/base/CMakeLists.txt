file(GLOB_RECURSE base ./  *.cpp  "../external/imgui_1_87/*.cpp")


set(KTX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdParty/ktx)
set(KTX_SOURCES
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c)

add_library(base ${base} ${KTX_SOURCES})