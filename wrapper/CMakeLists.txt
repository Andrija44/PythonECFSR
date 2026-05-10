cmake_minimum_required(VERSION 3.17)
project(ecf-sr)

set(CMAKE_CXX_STANDARD 17)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module NumPy)
include_directories(${Python_INCLUDE_DIRS})
set(pyver_short ${Python_VERSION_MAJOR}${Python_VERSION_MINOR})

find_package(Boost CONFIG REQUIRED COMPONENTS python${pyver_short} numpy${pyver_short})
include_directories(${Boost_INCLUDE_DIRS})

add_library(ecf-sr MODULE library.cpp Model.cpp)

set_target_properties(ecf-sr PROPERTIES PREFIX "" OUTPUT_NAME "ecf_sr")

target_link_libraries(ecf-sr ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

target_link_libraries(ecf-sr
    ${CMAKE_CURRENT_SOURCE_DIR}/../ECF/build/libECF.a
)

install(TARGETS ecf-sr DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/ecf_sr)