include(CMakeFindDependencyMacro)

find_dependency(Boost 1.69 COMPONENTS filesystem)
find_dependency(ROOT 6)

get_filename_component(MENSURA_CMAKE_DIR
    "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY
)

if (NOT TARGET mensura::mensura)
    # The file with exported targets below is generated by CMake
    include("${MENSURA_CMAKE_DIR}/mensuraTargets.cmake")
endif()

