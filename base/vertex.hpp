#pragma once
#include "../utils/matrix.hpp"
#include "../utils/transformations.hpp"
struct Vertex
{
    gl::vec3 position;
    gl::vec3 normal;
    gl::vec2 texCoords;
    gl::vec4 baseColor;

    Vertex(gl::vec3 position = {0.0f, 0.0f, 0.0f}, gl::vec3 normal = {0.0f, 0.0f, 0.0f}, gl::vec2 texCoords = {0.0f, 0.0f}, gl::vec4 baseColor = {1.0f, 1.0f, 1.0f,1.0f})
    {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
        this->baseColor = baseColor;
    };

    bool operator==(const Vertex &other)
    {
        return position == other.position && normal == other.normal && texCoords == other.texCoords;
    }
};

struct Micropolygon
{
    Vertex v0;
    Vertex v1;
    Vertex v2;
    Vertex v3;

    // return a vec4 (min_x, max_x, min_y, max_y)
    gl::vec4 getProjectedBoundingBox(gl::mat4 mvp, int width, int height) const
    {
        auto projectedRectPos = getProjectedRectPos(mvp, width, height);
        float min_x = 1e7;
        float min_y = 1e7;
        float max_x = -1e7;
        float max_y = -1e7;

        for (auto &pos : projectedRectPos)
        {
            min_x = std::min(min_x, pos.x());
            min_y = std::min(min_y, pos.y());
            max_x = std::max(max_x, pos.x());
            max_y = std::max(max_y, pos.y());
        }

        return gl::vec4(min_x, max_x, min_y, max_y);
    }

    Micropolygon(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
    {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    };

    std::array<gl::vec3, 4> getProjectedRectPos(gl::mat4 mvp, int width, int height) const
    {
        std::array<gl::vec3, 4> projectedRectPos;
        projectedRectPos[0] = gl::getScreenCoordAndDepth(v0.position, mvp, width, height);
        projectedRectPos[1] = gl::getScreenCoordAndDepth(v1.position, mvp, width, height);
        projectedRectPos[2] = gl::getScreenCoordAndDepth(v2.position, mvp, width, height);
        projectedRectPos[3] = gl::getScreenCoordAndDepth(v3.position, mvp, width, height);
        return projectedRectPos;
    }
};
