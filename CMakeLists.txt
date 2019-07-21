cmake_minimum_required(VERSION 3.5)
project(Quantization_IPP_ C)

set(CMAKE_C_STANDARD 11)

add_executable(Quantization_IPP_ main.c Particle/particle.h Particle/particle.c Trie/trie.h Trie/trie.c UnionFind/unionFind.h UnionFind/unionFind.c Commands/commands.h Commands/commands.c)
