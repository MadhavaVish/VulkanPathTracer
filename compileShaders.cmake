include(addShader.cmake)

add_shader(${TARGET_NAME} ${SHADER_PATH} "simple_shader.vert")
add_shader(${TARGET_NAME} ${SHADER_PATH} "simple_shader.frag")