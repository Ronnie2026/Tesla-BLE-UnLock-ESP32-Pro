idf_component_register(SRCS "main.cpp" REQUIRES unlock_core esp_timer)
target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)
