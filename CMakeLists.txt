cmake_minimum_required(VERSION 2.8)

project(mymalloc)

file(GLOB malloc *.c)

include_directories(/home/gandoulf/epitech/PSU_project/rendu/malloc)

add_library(malloc SHARED ${malloc})

add_definitions("-DDEBUG")

target_link_libraries(malloc
			-lmy
			)