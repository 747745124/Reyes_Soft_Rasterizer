#pragma once
#include "./mesh.hpp"
#include "../utils/Rheader.hpp"

// Basic primitives that descibed in RISpec 3.2
class Sphere : public Mesh
{
public:
    Sphere() : Mesh(50, 50)
    {
        // another representaion of a sphere
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
    };

    // RISpec 3.2 representation
    Sphere(float radius, float zmin, float zmax, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {
        // defined in RISpec 3.2
        float phi_min = gl::to_radian(-90.f);
        if (zmin > -radius)
            phi_min = asinf(zmin / radius);

        float phi_max = gl::to_radian(90.f);
        if (zmax < radius)
            phi_max = asinf(zmax / radius);

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float phi = phi_min + v_seg * (phi_max - phi_min);
                float theta = u_seg * thetamax;
                float x_pos = radius * cosf(theta) * cosf(phi);
                float y_pos = radius * sinf(theta) * cosf(phi);
                float z_pos = radius * sinf(phi);

                float x_norm = cosf(theta) * cosf(phi);
                float y_norm = sinf(theta) * cosf(phi);
                float z_norm = sinf(phi);

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {0, 0});
            }
        }
    };

    ~Sphere() = default;
};

class Cylinder : public Mesh
{
public:
    Cylinder(float radius, float zmin, float zmax, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float x_pos = radius * cosf(theta);
                float y_pos = radius * sinf(theta);
                float z_pos = v_seg * (zmax - zmin);

                float x_norm = cosf(theta);
                float y_norm = sinf(theta);
                float z_norm = 0;

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {0, 0});
            }
        }
    };

    ~Cylinder() = default;
};

class Cone : public Mesh
{
public:
    Cone(float height, float radius, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float x_pos = (1 - v_seg) * radius * cosf(theta);
                float y_pos = (1 - v_seg) * radius * sinf(theta);
                float z_pos = v_seg * height;

                float x_norm = cosf(theta);
                float y_norm = sinf(theta);
                float z_norm = height / radius;

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {0, 0});
            }
        }
    };

    ~Cone() = default;
};

class Hyperboloid : public Mesh
{
public:
    Hyperboloid(gl::vec3 point1, gl::vec3 point2, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float x_r = (1 - v_seg) * point1.x() + v_seg * point2.x();
                float y_r = (1 - v_seg) * point1.y() + v_seg * point2.y();
                float z_r = (1 - v_seg) * point1.z() + v_seg * point2.z();
                float x_pos = x_r * cosf(theta) - y_r * sinf(theta);
                float y_pos = x_r * sinf(theta) + y_r * cosf(theta);
                float z_pos = z_r;

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {0, 0});
            }
        }
    };

    ~Hyperboloid() = default;
};

class Paraboloid : public Mesh
{
public:
    Paraboloid(float rmax, float zmin, float zmax, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float z_pos = v_seg * (zmax - zmin);
                float radius = rmax * sqrtf(z_pos / zmax);
                float x_pos = radius * cosf(theta);
                float y_pos = radius * sinf(theta);

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {0, 0});
            }
        }
    };

    ~Paraboloid() = default;
};

class Disk : public Mesh
{
public:
    Disk(float height, float radius, float thetamax, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;

                float x_pos = radius * (1 - v_seg) * cosf(theta);
                float y_pos = radius * (1 - v_seg) * sinf(theta);
                float z_pos = height;

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {0, 0});
            }
        }
    };

    ~Disk() = default;
};

class Torus : public Mesh
{
public:
    Torus(float major_radius, float minor_radius, float phi_min, float phi_max, float theta_max, uint u_segments = 50, uint v_segments = 50) : Mesh(u_segments, v_segments)
    {

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * theta_max;
                float phi = phi_min + v_seg * (phi_max - phi_min);

                float radius = minor_radius * cosf(phi);
                float x_pos = (major_radius + radius) * cosf(theta);
                float y_pos = (major_radius + radius) * sinf(theta);
                float z_pos = minor_radius * sinf(phi);

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {0, 0});
            }
        }
    };

    ~Torus() = default;
};
