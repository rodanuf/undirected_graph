#pragma once


template <typename t_vertex>
struct vertex
{
    int id;
    t_vertex data;

    vertex() = default;
    vertex(int id, const t_vertex &data) : id(id), data(data) {}
};
