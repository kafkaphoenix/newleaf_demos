target_compile_options(${PROJECT_NAME} PRIVATE
    /EHs
    /nologo
    /std:c++latest
    /Od
    /utf-8
    /WX
    /Wall
    /bigobj
    /wd4996 # For deprecated functions in MSVC
)
target_compile_options(${PROJECT_NAME} PRIVATE /external:I ${CMAKE_SOURCE_DIR}/build /W0) # Exclude from warnings
