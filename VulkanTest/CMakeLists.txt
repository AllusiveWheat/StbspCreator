# CMakeList.txt : CMake project for VulkanTest, include source and define
# project specific logic here.
#



find_package(Vulkan REQUIRED)
if(NOT WIN32)
    find_package(glm REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(TBB REQUIRED)
endif()
# Add source to this project's executable.
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable (rStream "VulkanTest.cpp" "VulkanTest.h" "st_window.cpp" "st_app.cpp" "st_pipeline.cpp" "st_device.cpp" "st_swap_chain.cpp" "st_model.cpp" "st_renderer.cpp" "simple_render_system.cpp" "st_camera.cpp" "keyboard_movement_controller.cpp" "st_bsp_loader.cpp" "st_game_object.cpp" "st_mdl_loader.cpp" "st_buffer.cpp" "st_descriptors.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rStream PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.


set(GLSLC_PATH )

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.vert.spv"
    COMMAND $ENV{VK_SDK_PATH}/bin/glslc "shaders/simple_shader.vert.glsl" -o "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.vert.spv"
    DEPENDS "shaders/simple_shader.vert.glsl"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Buiding simple vert shader"
)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.frag.spv"
    COMMAND $ENV{VK_SDK_PATH}/bin/glslc "shaders/simple_shader.frag.glsl" -o "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.frag.spv"
    DEPENDS "shaders/simple_shader.frag.glsl"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Buiding simple frag shader"
)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/histogram.comp.spv"
    COMMAND $ENV{VK_SDK_PATH}/bin/glslc "shaders/histogram.comp.glsl" -o "${CMAKE_CURRENT_BINARY_DIR}/histogram.comp.spv"
    DEPENDS "shaders/histogram.comp.glsl"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Buiding histogram shader"
)

add_custom_target(FragShader DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.frag.spv")
add_custom_target(VertShader DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/simple_shader.vert.spv")
add_custom_target(CompShader DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/histogram.comp.spv")

if(WIN32)
    target_link_libraries(rStream PRIVATE Vulkan::Vulkan ${PROJECT_SOURCE_DIR}/lib/glfw3.lib)
else()
    target_link_libraries(rStream PRIVATE Vulkan::Vulkan glm::glm glfw TBB::tbb)
endif()
target_compile_options(rStream PRIVATE -mavx)

add_dependencies(rStream VertShader FragShader CompShader)
