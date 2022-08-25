set(imgui_PATH ${CMAKE_SOURCE_DIR}/vendor/imgui)

add_library(imgui
        ${imgui_PATH}/imconfig.h
        ${imgui_PATH}/imgui.h
        ${imgui_PATH}/imgui.cpp
        ${imgui_PATH}/imgui_draw.cpp
        ${imgui_PATH}/imgui_internal.h
        ${imgui_PATH}/imgui_tables.cpp
        ${imgui_PATH}/imgui_demo.cpp
        ${imgui_PATH}/imgui_widgets.cpp
        ${imgui_PATH}/imstb_rectpack.h
        ${imgui_PATH}/imstb_textedit.h
        ${imgui_PATH}/imstb_truetype.h
)

set(IMGUI_INCLUDE_DIRS ${imgui_PATH})