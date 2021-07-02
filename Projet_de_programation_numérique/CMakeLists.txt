cmake_minimum_required (VERSION 2.6)
project (projetgroupe)
include_directories(../projetgroupe)
add_executable(main 
		main.cpp
		Graphe.h
		dijkstra.cpp
		bellmanford.cpp
)
