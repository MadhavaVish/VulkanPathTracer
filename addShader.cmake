function(add_shader TARGET SHADER_PATH SHADER)
    find_program(GLSLC glslc)

    set(current-shader-path ${SHADER_PATH}/${SHADER})
    set(current-output-path ${SHADER_PATH}/compiled/${SHADER}.spv)

    # Call GLSLC to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})
    execute_process(COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path})
endfunction(add_shader)