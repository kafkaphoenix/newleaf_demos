include(FetchContent)
FetchContent_Declare(
    newleaf
    GIT_REPOSITORY https://github.com/kafkaphoenix/newleaf.git
    GIT_TAG        6cfbebe175f0e5236d295c93925b4ab945259b0e
)

FetchContent_MakeAvailable(newleaf)

target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")

target_link_libraries(
    ${PROJECT_NAME} PRIVATE newleaf::newleaf
)
