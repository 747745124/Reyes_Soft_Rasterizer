#pragma once
#include "./mesh.hpp"

class Sphere : public Mesh
{
public:
    Sphere();
    ~Sphere() = default;

    int getVertexCount() const
    {
        return V_SEGMENTS * U_SEGMENTS;
    };

    Vertex getVertex(uint u, uint v) const
    {
        return _grids[u][v];
    };

    std::tuple<uint, uint> getResolution() const
    {
        return std::make_tuple(U_SEGMENTS, V_SEGMENTS);
    };

private:
    uint V_SEGMENTS = 50;
    uint U_SEGMENTS = 50;
};
