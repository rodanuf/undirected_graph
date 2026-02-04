#pragma once

#include "lab3_2ndsem/headers/list_sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "pointers/uniq_ptr.hpp"
#include "vertex.hpp"
#include <functional>
#include <sstream>
#include <variant>



template<typename t_vertex, typename t_edge = std::monostate>
class undirected_graph
{
private:
    list_sequence<vertex<t_vertex>> vertices;
    list_sequence<std::function<list_sequence<int>()>> adjacency;

public:
    undirected_graph() = default;

    int add_vertex(const t_vertex &value);
    int vertex_count() const;

    void set_edge_generator(int vertex_id, std::function<list_sequence<int>()> edge_generator);

    list_sequence<int> neighbors(int vertex_id) const;

    const t_vertex &vertex_data(int vertex_id) const;

    array_sequence<list_sequence<int>> find_connected_components();
};

#include "undirected_graph.tpp"
