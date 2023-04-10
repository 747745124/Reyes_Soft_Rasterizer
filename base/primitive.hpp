#pragma once
#include "./mesh.hpp"

class Sphere : public Mesh
{
    Sphere();
    ~Sphere();

private:
    const int Y_SEGMENTS = 50;
    const int X_SEGMENTS = 50;
    std::vector<Vertex> _vertices;
};
