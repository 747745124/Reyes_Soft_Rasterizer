#pragma once
#include "./object3D.hpp"
#include "./vertex.hpp"

class Mesh : public Object3D
{

private:
    std::vector<Vertex> _vertices;
};
