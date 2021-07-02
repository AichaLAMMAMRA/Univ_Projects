cmake_minimum_required (VERSION 2.6)
project (projetgroupe)
include_directories(../src)
add_executable(main 
		main.cpp
		Graphe.h
		dijkstra.cpp
		bellmanfordfinal.h
)
