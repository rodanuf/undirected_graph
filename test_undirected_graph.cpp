#include <gtest/gtest.h>
#include "undirected_graph.hpp"

TEST(test_undirected_graph, empty_graph)
{
    undirected_graph<int> graph;
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 0);
}

TEST(test_undirected_graph, add_vertex_increments_count)
{
    undirected_graph<int> graph;

    EXPECT_EQ(graph.vertex_count(), 0); 

    int id = graph.add_vertex(100);

    EXPECT_EQ(id, 0);                   
    EXPECT_EQ(graph.vertex_count(), 1); 
}

TEST(test_undirected_graph, single_vertex_no_edges)
{
    undirected_graph<int> graph;
    graph.add_vertex(100);

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 1);
    EXPECT_EQ(components[0].get_length(), 1);
    EXPECT_EQ(components[0].get(0), 0);
}

TEST(test_undirected_graph, multiple_isolated_vertices)
{
    undirected_graph<int> graph;
    for (int i = 0; i < 5; ++i)
    {
        graph.add_vertex(i * 100);
    }

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 5);

    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(components[i].get_length(), 1);
        EXPECT_EQ(components[i].get(0), i);
    }
}

TEST(test_undirected_graph, fully_connected_graph)
{
    undirected_graph<int> graph;

    for (int i = 0; i < 4; ++i)
    {
        graph.add_vertex(i * 100);
    }

    for (int i = 0; i < 4; ++i)
    {
        graph.set_edge_generator(i, [i]()
                                 {
            list_sequence<int> neighbors;
            for (int j = 0; j < 4; ++j) {
                if (j != i) {
                    neighbors.append_element(j);
                }
            }
            return neighbors; });
    }

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 1);
    EXPECT_EQ(components[0].get_length(), 4);

    auto &component = components[0];
    bool vertices_found[4] = {false};
    for (int i = 0; i < component.get_length(); ++i)
    {
        int vertex_id = component.get(i);
        EXPECT_GE(vertex_id, 0);
        EXPECT_LE(vertex_id, 3);
        vertices_found[vertex_id] = true;
    }

    for (bool found : vertices_found)
    {
        EXPECT_TRUE(found);
    }
}

TEST(test_undirected_graph, two_connected_components)
{
    undirected_graph<int> graph;

    for (int i = 0; i < 6; ++i)
    {
        graph.add_vertex(i * 100);
    }

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(2, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(3, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(4);
        return neighbors; });

    graph.set_edge_generator(4, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(3);
        neighbors.append_element(5);
        return neighbors; });

    graph.set_edge_generator(5, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(4);
        return neighbors; });

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 2);

    if (components[0].get_length() < components[1].get_length())
    {
        auto temp = components[0];
        components[0] = components[1];
        components[1] = temp;
    }

    EXPECT_EQ(components[0].get_length(), 3);
    EXPECT_EQ(components[1].get_length(), 3);

    bool vertex_in_first_component[6] = {false};
    for (int i = 0; i < components[0].get_length(); ++i)
    {
        int vertex_id = components[0].get(i);
        vertex_in_first_component[vertex_id] = true;
    }

    for (int i = 0; i < components[1].get_length(); ++i)
    {
        int vertex_id = components[1].get(i);
        EXPECT_FALSE(vertex_in_first_component[vertex_id]);
    }
}

TEST(test_undirected_graph, complex_graph_with_multiple_components)
{
    undirected_graph<std::string> graph;

    int v0 = graph.add_vertex("A");
    int v1 = graph.add_vertex("B");
    int v2 = graph.add_vertex("C");
    int v3 = graph.add_vertex("D");
    int v4 = graph.add_vertex("E");
    int v5 = graph.add_vertex("F");
    int v6 = graph.add_vertex("G");
    int v7 = graph.add_vertex("H");

    EXPECT_EQ(v0, 0);
    EXPECT_EQ(v7, 7);

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(2, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(3, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(4);
        return neighbors; });

    graph.set_edge_generator(4, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(3);
        neighbors.append_element(5);
        return neighbors; });

    graph.set_edge_generator(5, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(4);
        neighbors.append_element(6);
        return neighbors; });

    graph.set_edge_generator(6, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(5);
        return neighbors; });

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 3);

    list_sequence<int> component_of_size_3;
    list_sequence<int> component_of_size_4;
    list_sequence<int> component_of_size_1;

    for (int i = 0; i < components.get_length(); ++i)
    {
        auto &comp = components[i];
        if (comp.get_length() == 3)
        {
            component_of_size_3 = comp;
        }
        else if (comp.get_length() == 4)
        {
            component_of_size_4 = comp;
        }
        else if (comp.get_length() == 1)
        {
            component_of_size_1 = comp;
        }
    }

    EXPECT_EQ(component_of_size_3.get_length(), 3);
    EXPECT_EQ(component_of_size_4.get_length(), 4);
    EXPECT_EQ(component_of_size_1.get_length(), 1);

    EXPECT_EQ(component_of_size_1.get(0), 7);
}

TEST(test_undirected_graph, self_loop)
{
    undirected_graph<int> graph;

    graph.add_vertex(100);
    graph.add_vertex(200);

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        return neighbors; });

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 1);
    EXPECT_EQ(components[0].get_length(), 2);
}

TEST(test_undirected_graph, large_graph_with_one_component)
{
    undirected_graph<int> graph;
    const int n = 100;

    for (int i = 0; i < n; ++i)
    {
        graph.add_vertex(i * 100);
    }

    for (int i = 0; i < n; ++i)
    {
        graph.set_edge_generator(i, [i, n]()
                                 {
            list_sequence<int> neighbors;
            int prev = (i - 1 + n) % n;
            int next = (i + 1) % n;
            neighbors.append_element(prev);
            neighbors.append_element(next);
            return neighbors; });
    }

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 1);
    EXPECT_EQ(components[0].get_length(), n);
}

TEST(test_undirected_graph, graph_with_duplicate_edges)
{
    undirected_graph<int> graph;

    graph.add_vertex(100);
    graph.add_vertex(200);
    graph.add_vertex(300);

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(2, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        return neighbors; });

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 1);
    EXPECT_EQ(components[0].get_length(), 3);
}

TEST(test_undirected_graph, graph_with_custom_vertex_type)
{
    struct point
    {
        int x, y;
        bool operator==(const point &other) const
        {
            return x == other.x && y == other.y;
        }
    };

    undirected_graph<point> graph;

    graph.add_vertex(point{0, 0});
    graph.add_vertex(point{1, 0});
    graph.add_vertex(point{0, 1});
    graph.add_vertex(point{10, 10});

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(2, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(1);
        return neighbors; });

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.get_length(), 2);

    auto &component_3 = components[0].get_length() == 3 ? components[0] : components[1];
    auto &component_1 = components[0].get_length() == 1 ? components[0] : components[1];

    EXPECT_EQ(component_3.get_length(), 3);
    EXPECT_EQ(component_1.get_length(), 1);
    EXPECT_EQ(component_1.get(0), 3);
}

TEST(test_undirected_graph, algorithm_does_not_modify_graph)
{
    undirected_graph<int> graph;

    graph.add_vertex(100);
    graph.add_vertex(200);
    graph.add_vertex(300);

    graph.set_edge_generator(0, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        return neighbors; });

    graph.set_edge_generator(1, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(0);
        neighbors.append_element(2);
        return neighbors; });

    graph.set_edge_generator(2, []()
                             {
        list_sequence<int> neighbors;
        neighbors.append_element(1);
        return neighbors; });

    int initial_vertex_count = graph.vertex_count();
    auto initial_neighbors_0 = graph.neighbors(0);

    auto components = graph.find_connected_components();

    EXPECT_EQ(graph.vertex_count(), initial_vertex_count);
    EXPECT_EQ(graph.vertex_data(0), 100);

    auto neighbors_0 = graph.neighbors(0);
    EXPECT_EQ(neighbors_0.get_length(), initial_neighbors_0.get_length());
    EXPECT_EQ(neighbors_0.get(0), initial_neighbors_0.get(0));
}