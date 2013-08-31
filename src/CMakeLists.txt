if (WIN32)
    set(LIBS glfw3 glew32 opengl32 DevIL)
else (WIN32)
    set(LIBS m glfw GLEW GL IL)
endif (WIN32)

add_executable(editor editor.c map.c ui.c ui_manager.c input_manager.c
                      sf.c sf_list.c sf_window.c sf_frame.c sf_texture.c
                      sf_rect.c)

set_target_properties(editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(editor ${LIBS})
