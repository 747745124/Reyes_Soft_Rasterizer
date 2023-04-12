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

        // resize the grids, _grids can be indexed by [u][v]
        // u from 0 to U_SEGMENTS
        // v from 0 to V_SEGMENTS
        _grids.resize(U_SEGMENTS + 1);
        for (int i = 0; i < U_SEGMENTS + 1; i++)
        {
            _grids[i].resize(V_SEGMENTS + 1);
        }
    };

    int getVertexCount() const
    {
        return (V_SEGMENTS+1) * (U_SEGMENTS+1);
    };

    Vertex getVertex(uint u, uint v) const
    {   
        assert(u <= U_SEGMENTS && v <= V_SEGMENTS);
        return _grids[u][v];
    };

    std::tuple<uint, uint> getResolution() const
    {
        return std::make_tuple(U_SEGMENTS, V_SEGMENTS);
    };

    // get the micropolygon starting from the bottom left corner
    // this ensures that the micropolygon is always counter-clockwise
    // and only visited once across the mesh
    Micropolygon getMicropolygon(uint u, uint v) const
    {
        assert(u < U_SEGMENTS && v < V_SEGMENTS);
        return Micropolygon(_grids[u][v], _grids[u + 1][v], _grids[u + 1][v + 1], _grids[u][v + 1]);
    };

protected:
    uint V_SEGMENTS = 50;
    uint U_SEGMENTS = 50;
    std::vector<std::vector<Vertex>> _grids;
    std::vector<size_t> _indices;
};
