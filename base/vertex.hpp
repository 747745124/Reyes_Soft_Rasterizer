#pragma once
#include "../utils/matrix.hpp"
struct Vertex
{
    gl::vec3 position;
    gl::vec3 normal;
    gl::vec2 texCoords;
    gl::vec3 baseColor;

    Vertex(gl::vec3 position = {0.0f, 0.0f, 0.0f}, gl::vec3 normal = {0.0f, 0.0f, 0.0f}, gl::vec2 texCoords = {0.0f, 0.0f}, gl::vec3 baseColor = {1.0f, 1.0f, 1.0f})
    {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
    };

    bool operator==(const Vertex &other)
    {
        return position == other.position && normal == other.normal && texCoords == other.texCoords;
    }
};