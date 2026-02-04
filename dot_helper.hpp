#pragma once
#include <sstream>
#include <string>
#include <type_traits>
#include "undirected_graph.hpp" 

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
vertex_to_string(const T &value)
{
    return std::to_string(value);
}

inline std::string vertex_to_string(const std::string &value)
{
    return value;
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value &&
                            !std::is_same<T, std::string>::value,
                        std::string>::type
vertex_to_string(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename t_vertex, typename t_edge = std::monostate>
std::string to_dot(const undirected_graph<t_vertex, t_edge> &graph)
{
    std::ostringstream dot;
    dot << "graph G {\n";
    dot << "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n";
    dot << "    edge [color=gray40];\n";

    for (int i = 0; i < graph.vertex_count(); ++i)
    {
        std::string label = std::to_string(i) + ": " + vertex_to_string(graph.vertex_data(i));

        size_t pos;
        while ((pos = label.find('"')) != std::string::npos)
        {
            label.replace(pos, 1, "\\\"");
        }

        dot << "    " << i << " [label=\"" << label << "\"];\n";
    }

    for (int u = 0; u < graph.vertex_count(); ++u)
    {
        auto neighbors = graph.neighbors(u);
        for (int i = 0; i < neighbors.get_length(); ++i)
        {
            int v = neighbors.get(i);
            if (u < v)
            {
                dot << "    " << u << " -- " << v << ";\n";
            }
        }
    }

    dot << "}\n";
    return dot.str();
}

template <typename graph>
void export_to_dot(const graph &gr, const std::string &filename = "graph.dot")
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << "\n";
        return;
    }
    out << gr.to_dot();
    out.close();
    std::cout << "Graph exported to " << filename << "\n";
}