#pragma once
#include "./object3D.hpp"
#include "./vertex.hpp"

class Mesh : public Object3D
{
public:
    Mesh(uint u_segments, uint v_segments)
    {
        this->U_SEGMENTS = u_segments;
        this->V_SEGMENTS = v_segments;

        // resize the grids
        _grids.resize(U_SEGMENTS + 1);
        for (int i = 0; i < U_SEGMENTS + 1; i++)
        {
            _grids[i].resize(V_SEGMENTS + 1);
        }
    };

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

protected:
    uint V_SEGMENTS = 50;
    uint U_SEGMENTS = 50;
    std::vector<std::vector<Vertex>> _grids;
    std::vector<size_t> _indices;
};
