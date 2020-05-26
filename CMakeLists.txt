cmake_minimum_required(VERSION 3.16)
project(f00l_tiny_stl)

set(CMAKE_CXX_STANDARD 11)

add_executable(f00l_tiny_stl main.cpp stl_config.h stl_alloc.h type_traits.h stl_construct.h stl_iterator_base.h stl_iterator.h stl_algobase.h stl_uninitial.h stl_vector.h stl_list.h stl_pair.h stl_hashtable.h)