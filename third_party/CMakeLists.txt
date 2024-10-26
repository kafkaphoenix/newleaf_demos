include(FetchContent)
FetchContent_Declare(
    newleaf
    GIT_REPOSITORY https://github.com/kafkaphoenix/newleaf.git
    GIT_TAG        66fcaacc5bb78d1c49421ded0bf3ab5b4dc2964f
)

FetchContent_MakeAvailable(newleaf)

target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")

target_link_libraries(
    ${PROJECT_NAME} PRIVATE newleaf::newleaf
)
