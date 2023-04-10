#include "./primitive.hpp"

Sphere::Sphere()
{
    // resize the grids
    _grids.resize(U_SEGMENTS + 1);
    for (int i = 0; i < U_SEGMENTS + 1; i++)
    {
        _grids[i].resize(V_SEGMENTS + 1);
    }

    /*2-Calculate the vertices of the sphere*/
    // Generate the vertices of the ball
    for (int y = 0; y < V_SEGMENTS; y++)
    {
        for (int x = 0; x < U_SEGMENTS; x++)
        {
            float x_seg = (float)x / (float)U_SEGMENTS;
            float y_seg = (float)y / (float)V_SEGMENTS;
            float xPos = cos(x_seg * 2.0f * M_PI) * sin(y_seg * M_PI);
            float yPos = cos(y_seg * M_PI);
            float zPos = sin(x_seg * 2.0f * M_PI) * sin(y_seg * M_PI);
            _grids[x][y] = Vertex({xPos, yPos, zPos}, {xPos, yPos, zPos}, {0, 0});
        }
    }

    // get the indices of the vertices
    for (int i = 0; i < V_SEGMENTS; i++)
    {
        for (int j = 0; j < U_SEGMENTS; j++)
        {
            _indices.push_back(i * (U_SEGMENTS + 1) + j);
            _indices.push_back((i + 1) * (U_SEGMENTS + 1) + j);
            _indices.push_back((i + 1) * (U_SEGMENTS + 1) + j + 1);
            _indices.push_back(i * (U_SEGMENTS + 1) + j);
            _indices.push_back((i + 1) * (U_SEGMENTS + 1) + j + 1);
            _indices.push_back(i * (U_SEGMENTS + 1) + j + 1);
        }
    }
}