#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include "undirected_graph.hpp"
#include "dot_helper.hpp"

template <typename T>
void generate_random_graph(undirected_graph<T> &g, int n, double p, std::mt19937 &rng)
{
    for (int i = 0; i < n; ++i)
    {
        g.add_vertex(i);
    }

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < n; ++i)
    {
        list_sequence<int> neighbors;
        for (int j = 0; j < n; ++j)
        {
            if (i != j && dist(rng) < p)
            {
                neighbors.append_element(j);
            }
        }
        g.set_edge_generator(i, [neighbors]() mutable -> list_sequence<int>
                             { return neighbors; });
    }
}

int main(int argc, char *argv[])
{
    array_sequence<int> sizes = {100, 500, 1000, 2000};
    array_sequence<double> densities = {0.1, 0.3, 0.5, 0.7, 0.9};
    int runs_per_config = 5;

    if (argc > 1)
    {
        sizes.clear();
        for (int i = 1; i < argc; ++i)
        {
            sizes.append_element(std::stoi(argv[i]));
        }
    }

    std::cout << "========================================\n";
    std::cout << "  Benchmark: Connected Components\n";
    std::cout << "========================================\n";
    std::cout << "Sizes: ";
    for (int s : sizes)
        std::cout << s << " ";
    std::cout << "\nDensities: ";
    for (double d : densities)
        std::cout << d << " ";
    std::cout << "\nRuns per config: " << runs_per_config << "\n\n";

    // Файл для результатов
    std::ofstream csv("benchmark.csv");
    csv << "n;edge_density;avg_time_ms;min_time_ms;max_time_ms;components\n";

    std::mt19937 rng(42);

    for (int n : sizes)
    {
        for (double p : densities)
        {
            array_sequence<double> times;
            int comp_count = 0;

            for (int run = 0; run < runs_per_config; ++run)
            {
                undirected_graph<int> g;
                generate_random_graph(g, n, p, rng);

                auto start = std::chrono::high_resolution_clock::now();
                auto components = g.find_connected_components();
                auto end = std::chrono::high_resolution_clock::now();

                double time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
                times.append_element(time_ms);
                if (run == 0)
                    comp_count = components.get_length();
            }

            double sum = 0.0, min_t = times[0], max_t = times[0];
            for (double t : times)
            {
                sum += t;
                if (t < min_t)
                    min_t = t;
                if (t > max_t)
                    max_t = t;
            }
            double avg_t = sum / runs_per_config;

            csv << n << ";" << p << ";" << avg_t << ";" << min_t << ";" << max_t << ";" << comp_count << "\n";

            std::cout << "n=" << n << ", p=" << p
                      << "avg=" << avg_t << " ms"
                      << " (min=" << min_t << ", max=" << max_t << ")"
                      << ", components=" << comp_count << "\n";
        }
        std::cout << "----------------------------------------\n";
    }

    csv.close();
    std::cout << "\nBenchmark results saved to benchmark.csv\n";
    std::cout << "\nNext steps:\n";
    std::cout << "1.Open benchmark.csv in Excel/LibreOffice\n";
    std::cout << "2.Create scatter plot: X=n, Y=avg_time_ms, series=edge_density\n";
    std::cout << "3.Add trendline observe linear O(n+m) complexity\n";

    return 0;
}