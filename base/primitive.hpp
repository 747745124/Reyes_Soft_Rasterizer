#pragma once
#include "./mesh.hpp"
#include "../utils/Rheader.hpp"

// Basic primitives that descibed in RISpec 3.2
class Sphere : public Mesh
{
public:
    float radius, zmin, zmax, thetamax, phi_min, phi_max;
    // RISpec 3.2 representation,thetamax is in radian
    Sphere(float radius, float zmin, float zmax, float thetamax)
    {   
        this->radius = radius;
        this->zmin = zmin;
        this->zmax = zmax;
        this->thetamax = thetamax;

        // defined in RISpec 3.2
        this->phi_min = gl::to_radian(-90.f);
        if (zmin > -radius)
            this->phi_min = asinf(zmin / radius);

        this->phi_max = gl::to_radian(90.f);
        if (zmax < radius)
            this->phi_max = asinf(zmax / radius);
        
        //approximate here, tho z range is not accurate
        auto z_range = std::make_pair(-radius,radius);
        auto x_range = std::make_pair(-radius,radius);
        auto y_range = std::make_pair(-radius,radius);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Sphere(float radius, float zmin, float zmax, float thetamax, uint u_segments, uint v_segments) : Sphere(radius, zmin, zmax, thetamax)
    {   
        resizeGrid(u_segments,v_segments);

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float phi = phi_min + v_seg * (phi_max - phi_min);
                float x_pos = radius * cosf(theta) * cosf(phi);
                float y_pos = radius * sinf(theta)*cosf(phi);
                float z_pos = radius * sinf(phi);

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_pos, y_pos, z_pos}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.f});
            } 
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2.f) override{

        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

        for (int y = 0; y <= V_SEGMENTS; y++)
        {
            for (int x = 0; x <= U_SEGMENTS; x++)
            {
                float u_seg = (float)x / (float)U_SEGMENTS;
                float v_seg = (float)y / (float)V_SEGMENTS;

                float theta = u_seg * thetamax;
                float phi = phi_min + v_seg * (phi_max - phi_min);
                float x_pos = radius * cosf(theta) * cosf(phi);
                float y_pos = radius * sinf(theta)*cosf(phi);
                float z_pos = radius * sinf(phi);

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_pos, y_pos, z_pos}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.f});
            } 
        }
    }

    ~Sphere() = default;
};

class Cylinder : public Mesh
{
public:
    float radius, zmin, zmax, thetamax;
    Cylinder(float radius, float zmin, float zmax, float thetamax)
    {
        this->radius = radius;
        this->zmin = zmin;
        this->zmax = zmax;
        this->thetamax = thetamax;
        //approximate here, tho z range is not accurate
        auto z_range = std::make_pair(zmin,zmax);
        auto x_range = std::make_pair(-radius,radius);
        auto y_range = std::make_pair(-radius,radius);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Cylinder(float radius, float zmin, float zmax, float thetamax, uint u_segments, uint v_segments) : Cylinder(radius, zmin, zmax, thetamax)
    {
        resizeGrid(u_segments,v_segments);

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.0f});
            } 
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {

        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.0f});
            } 
        }
    };

    ~Cylinder() = default;
};


class Cone : public Mesh
{
public:
    float height, radius, thetamax;
    Cone(float height, float radius, float thetamax)
    {
        this->height = height;
        this->radius = radius;
        this->thetamax = thetamax;
     
        auto z_range = std::make_pair(0.f,height);
        auto x_range = std::make_pair(-radius,radius);
        auto y_range = std::make_pair(-radius,radius);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Cone(float height, float radius, float thetamax, uint u_segments, uint v_segments) : Cone(height, radius, thetamax)
    {
        resizeGrid(u_segments,v_segments);

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.0f});
            }
        }
    };

    
    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {
        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {x_norm, y_norm, z_norm}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.0f});
            }
        }
    };

    ~Cone() = default;
};

class Hyperboloid : public Mesh
{
public:
    gl::vec3 point1, point2;
    float thetamax;

    Hyperboloid(gl::vec3 point1, gl::vec3 point2, float thetamax)
    {
        this->point1 = point1;
        this->point2 = point2;
        this->thetamax = thetamax;
        float height = fabs(point2.z() - point1.z());
        float radius_x =fabs(point2.x() - point1.x());
        float radius_y =fabs(point2.y() - point1.y());
        auto z_range = std::make_pair(0.f,height);
        auto x_range = std::make_pair(-radius_x,radius_x);
        auto y_range = std::make_pair(-radius_y,radius_y);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Hyperboloid(gl::vec3 point1, gl::vec3 point2, float thetamax, uint u_segments, uint v_segments) :Hyperboloid(point1,point2,thetamax)
    {

        resizeGrid(u_segments,v_segments);

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {
        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg}, {x_pos, y_pos, z_pos, 1.0f});
            }
        }
    };

    ~Hyperboloid() = default;
};

class Paraboloid : public Mesh
{
public:
    float rmax, zmin, zmax, thetamax;

    Paraboloid(float rmax, float zmin, float zmax, float thetamax)
    {
        this->rmax = rmax;
        this->zmin = zmin;
        this->zmax = zmax;
        this->thetamax = thetamax;

        auto z_range = std::make_pair(zmin,zmax);
        auto x_range = std::make_pair(-rmax,rmax);
        auto y_range = std::make_pair(-rmax,rmax);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Paraboloid(float rmax, float zmin, float zmax, float thetamax, uint u_segments, uint v_segments) : Paraboloid(rmax, zmin, zmax, thetamax)
    {
        resizeGrid(u_segments,v_segments);

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {

        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    ~Paraboloid() = default;
};

class Disk : public Mesh
{
public:
    float height, radius, thetamax;
    Disk(float height, float radius, float thetamax)
    {
        this->height = height;
        this->radius = radius;
        this->thetamax = thetamax;

        auto z_range = std::make_pair(height,height);
        auto x_range = std::make_pair(-radius,radius);
        auto y_range = std::make_pair(-radius,radius);
        this->_bounding_volume = {x_range,y_range,z_range};
    };

    Disk(float height, float radius, float thetamax, uint u_segments, uint v_segments) : Disk(height, radius, thetamax)
    {   
        resizeGrid(u_segments,v_segments);

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {

        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    ~Disk() = default;
};


class Torus : public Mesh
{
public:
    float major_radius, minor_radius, phi_min, phi_max, theta_max;
    Torus(float major_radius, float minor_radius, float phi_min, float phi_max, float theta_max)
    {
        this->major_radius = major_radius;
        this->minor_radius = minor_radius;
        this->phi_min = phi_min;
        this->phi_max = phi_max;
        this->theta_max = theta_max;

        auto z_range = std::make_pair(-minor_radius, minor_radius);
        auto x_range = std::make_pair(-(major_radius+minor_radius), major_radius+minor_radius);
        auto y_range = std::make_pair(-(major_radius+minor_radius), major_radius+minor_radius);
        this->_bounding_volume = {x_range,y_range,z_range};
    }

    Torus(float major_radius, float minor_radius, float phi_min, float phi_max, float theta_max, uint u_segments, uint v_segments) : Torus(major_radius, minor_radius, phi_min, phi_max, theta_max)
    {
        resizeGrid(u_segments,v_segments);
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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    void dice(uint x_span_pix,uint y_span_pix, float factor=2) override {

        resizeGrid((uint)(x_span_pix*factor),(uint)(y_span_pix*factor));

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

                _grids[x][y] = Vertex({x_pos, y_pos, z_pos}, {0, 0, 0}, {u_seg, v_seg});
            }
        }
    };

    ~Torus() = default;
};
