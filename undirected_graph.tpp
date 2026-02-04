#include "undirected_graph.hpp"
#include <stdexcept>
#include <functional>

template <typename t_vertex, typename t_edge>
int undirected_graph<t_vertex, t_edge>::add_vertex(const t_vertex &data)
{
    int id = vertices.get_length();
    vertices.append_element(vertex<t_vertex>(id, data));
    adjacency.append_element([]() -> list_sequence<int>
                             {
                                 return list_sequence<int>{};
                             });
    return id;
}

template <typename t_vertex, typename t_edge>
void undirected_graph<t_vertex, t_edge>::set_edge_generator(int vertex_id, std::function<list_sequence<int>()> edge_generator)
{
    if (vertex_id < 0 || vertex_id >= adjacency.get_length())
    {
        throw std::out_of_range("Invalid vertex ID");
    }
    adjacency.get(vertex_id) = std::move(edge_generator);
}

template <typename t_vertex, typename t_edge>
list_sequence<int> undirected_graph<t_vertex, t_edge>::neighbors(int vertex_id) const
{
    if (vertex_id < 0 || vertex_id >= adjacency.get_length())
    {
        throw std::out_of_range("Invalid vertex ID");
    }

    auto generator = adjacency.get(vertex_id);
    if (generator)
    {
        list_sequence<int> neighbors_list = generator();
        return neighbors_list;
    }
    return list_sequence<int>{};
}

template <typename t_vertex, typename t_edge>
int undirected_graph<t_vertex, t_edge>::vertex_count() const 
{ 
    return vertices.get_length(); 
}

template <typename t_vertex, typename t_edge>
const t_vertex &undirected_graph<t_vertex, t_edge>::vertex_data(int id) const
{
    return vertices.get(id).data;
}

template <typename t_vertex, typename t_edge>
array_sequence<list_sequence<int>> undirected_graph<t_vertex, t_edge>::find_connected_components()
{
    int n = this->vertex_count();

    array_sequence<bool> visited;
    for (int i = 0; i < n; ++i)
    {
        visited.append_element(false);
    }

    array_sequence<list_sequence<int>> components;

    std::function<void(int, list_sequence<int> &)> dfs =
        [&](int v, list_sequence<int> &comp)
    {
        visited[v] = true;

        comp.append_element(v);

        auto neighbors = this->neighbors(v);

        int neighbor_count = neighbors.get_length();
        for (int i = 0; i < neighbor_count; ++i)
        {
            int u = neighbors.get(i);
            if (!visited.get(u))
            {
                dfs(u, comp);
            }
        }
    };

    for (int v = 0; v < n; ++v)
    {
        if (!visited.get(v))
        {
            list_sequence<int> component;              
            dfs(v, component);                          
            components.append_element(std::move(component)); 
        }
    }

    return components;
}