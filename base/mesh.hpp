#pragma once
#include "./object3D.hpp"
#include "./vertex.hpp"

class Mesh : public Object3D
{

protected:
    std::vector<std::vector<Vertex>> _grids;
    std::vector<size_t> _indices;
};
