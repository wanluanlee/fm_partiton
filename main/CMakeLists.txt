add_executable(fm main.cpp)

# options/flags pass to the compiler
#target_compile_features(qsat INTERFACE cxx_std_17)
#target_compile_options(qsat INTERFACE -Wall -Wextra -Wfatal-errors)

# include and libary linking
target_include_directories(fm PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(fm LINK_PUBLIC gap)


