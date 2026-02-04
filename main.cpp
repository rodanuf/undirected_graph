// src/main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "undirected_graph.hpp"
#include "dot_helper.hpp"

int main()
{
    undirected_graph<std::string> graph;
    int choice;

    std::cout << "========================================\n";
    std::cout << "  Undirected Graph\n";
    std::cout << "========================================\n";

    do
    {
        std::cout << "\nMenu:\n";
        std::cout << "  1. Add vertex\n";
        std::cout << "  2. Set edges for vertex (manual)\n";
        std::cout << "  3. Find connected components\n";
        std::cout << "  4. Export graph to Graphviz (.dot)\n";
        std::cout << "  5. Export results to CSV\n";
        std::cout << "  0. Exit\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
        {
            std::string label;
            std::cout << "Enter vertex label: ";
            std::getline(std::cin, label);
            if (label.empty())
                label = "V" + std::to_string(graph.vertex_count());
            int id = graph.add_vertex(label);
            std::cout << "Vertex " << id << " added with label \"" << label << "\"\n";
            break;
        }

        case 2:
        {
            if (graph.vertex_count() == 0)
            {
                std::cout << "Graph is empty. Add vertices first.\n";
                break;
            }

            std::cout << "Available vertices:\n";
            for (int i = 0; i < graph.vertex_count(); ++i)
            {
                std::cout << "  " << i << ": " << graph.vertex_data(i) << "\n";
            }

            int vertex_id;
            std::cout << "Enter vertex ID to set edges for: ";
            std::cin >> vertex_id;
            std::cin.ignore(10000, '\n');

            if (vertex_id < 0 || vertex_id >= graph.vertex_count())
            {
                std::cout << "Invalid vertex ID.\n";
                break;
            }

            list_sequence<int> neighbors;
            std::cout << "Enter neighbor IDs (one per line, -1 to finish):\n";
            while (true)
            {
                std::cout << "  Neighbor ID: ";
                std::string input;
                std::getline(std::cin, input);
                if (input.empty())
                    continue;

                try
                {
                    int nid = std::stoi(input);
                    if (nid == -1)
                        break;
                    if (nid < 0 || nid >= graph.vertex_count())
                    {
                        std::cout << "Invalid neighbor ID. Try again.\n";
                        continue;
                    }
                    if (nid == vertex_id)
                    {
                        std::cout << "Skipping self-loop (not allowed in undirected graph).\n";
                        continue;
                    }
                    neighbors.append_element(nid);
                    std::cout << "Added neighbor " << nid << " (" << graph.vertex_data(nid) << ")\n";
                }
                catch (...)
                {
                    std::cout << "Invalid number. Try again.\n";
                }
            }

            graph.set_edge_generator(vertex_id,
                                     [neighbors]() mutable -> list_sequence<int>
                                     {
                                         return neighbors;
                                     });
            std::cout << "Edges set for vertex " << vertex_id << "\n";
            break;
        }

        case 3:
        {
            if (graph.vertex_count() == 0)
            {
                std::cout << "Graph is empty.\n";
                break;
            }

            auto start = std::chrono::high_resolution_clock::now();
            auto components = graph.find_connected_components();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << "\n Found " << components.get_length() << " component(s) in "
                      << duration_ms << " ms\n";

            for (int i = 0; i < components.get_length(); ++i)
            {
                auto &comp = components.get(i);
                std::cout << "  Component " << i << " (" << comp.get_length() << " vertices): ";
                for (int j = 0; j < comp.get_length(); ++j)
                {
                    int vid = comp.get(j);
                    std::cout << vid << " (\"" << graph.vertex_data(vid) << "\") ";
                }
                std::cout << "\n";
            }
            break;
        }

        case 4:
        {
            if (graph.vertex_count() == 0)
            {
                std::cout << "Graph is empty.\n";
                break;
            }

            std::string filename = "graph.dot";
            std::ofstream out(filename);
            if (!out.is_open())
            {
                std::cout << "Cannot open file " << filename << "\n";
                break;
            }
            out << to_dot(graph);
            out.close();

            std::cout << "   Graph exported to " << filename << "\n";
            std::cout << "   To visualize:\n";
            std::cout << "VS Code: open file -> Ctrl+Shift+V\n";
            std::cout << "Online: https://dreampuf.github.io/GraphvizOnline\n";
            break;
        }

        case 5:
        {
            if (graph.vertex_count() == 0)
            {
                std::cout << "Graph is empty.\n";
                break;
            }

            auto components = graph.find_connected_components();
            std::string filename = "components.csv";
            std::ofstream csv(filename);
            if (!csv.is_open())
            {
                std::cout << "Cannot open file " << filename << "\n";
                break;
            }

            csv << "vertex_id;vertex_label;component_id\n";
            for (int comp_id = 0; comp_id < components.get_length(); ++comp_id)
            {
                auto &comp = components.get(comp_id);
                for (int i = 0; i < comp.get_length(); ++i)
                {
                    int vid = comp.get(i);
                    csv << vid << ";" << graph.vertex_data(vid) << ";" << comp_id << "\n";
                }
            }
            csv.close();

            std::cout << "Results exported to " << filename << "\n";
            break;
        }

        case 0:
            break;

        default:
            std::cout << "Invalid choice. Try 0-5.\n";
        }

    } while (choice != 0);

    return 0;
}