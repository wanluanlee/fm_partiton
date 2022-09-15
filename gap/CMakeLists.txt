add_library(gap Cell.cpp Net.cpp Circuit.cpp)

#target_compile_features(gap INTERFACE cxx_std_17)
set_property(TARGET gap PROPERTY CXX_STANDARD 17)

target_link_options(gap INTERFACE -lstdc++fs)
target_compile_options(gap INTERFACE -Wall -Wextra -Wfatal-errors)
