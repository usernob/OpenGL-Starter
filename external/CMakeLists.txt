add_subdirectory(glad)
add_subdirectory(glfw)

# Add the Include Directories for the Libraries / Files Used for adding #include
# into the cpp files

# Include Files From Within the Workspace Folder
target_include_directories(${PROJECT_NAME} PUBLIC glfw/include
                                                  glad/include/glad stb)

# Add the Link Directories for the Libraries / Files Used for creation of
# Static/Dynamic Libraries

# Include Files From Within the Workspace Folder
target_link_directories(${PROJECT_NAME} PRIVATE glfw/src PRIVATE glad/src})

target_link_libraries(${PROJECT_NAME} glad glfw)
