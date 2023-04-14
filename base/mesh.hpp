#pragma once
#include "./object3D.hpp"
#include "./vertex.hpp"

class Mesh : public Object3D
{
public:
    //need furthur dicing
    Mesh() : Object3D()
    {
        _grids.resize(U_SEGMENTS + 1);
        for (int i = 0; i < U_SEGMENTS + 1; i++)
        {
            _grids[i].resize(V_SEGMENTS + 1);
        }
    };
    
    //specify arbitary number of vertices, no dicing needed
    void resizeGrid(uint u_segments, uint v_segments)
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

    void applyTexture(){

    };

    // Dice happens before the mesh actually initialize the grid
    virtual void dice(uint x_span_pix,uint y_span_pix, float factor=2.0)=0;
    
    // Given a bounding volume, calculate the projected span (in pixel) of the mesh on screen 
    gl::vec2 getProjectedBoundingVolumeSpan(const gl::mat4& mvp,uint width, uint height){
        gl::vec2 span(0,0);
        assert(_bounding_volume.size()==3);

        const auto& [x_min,x_max] = _bounding_volume[0];
        const auto& [y_min,y_max] = _bounding_volume[1];
        const auto& [z_min,z_max] = _bounding_volume[2];

        // get the 8 corners of the bounding box
        std::vector<gl::vec3> corners;
        corners.push_back(gl::vec3(x_min,y_min,z_min));
        corners.push_back(gl::vec3(x_min,y_min,z_max));
        corners.push_back(gl::vec3(x_min,y_max,z_min));
        corners.push_back(gl::vec3(x_min,y_max,z_max));
        corners.push_back(gl::vec3(x_max,y_min,z_min));
        corners.push_back(gl::vec3(x_max,y_min,z_max));
        corners.push_back(gl::vec3(x_max,y_max,z_min));
        corners.push_back(gl::vec3(x_max,y_max,z_max));


        float min_x = 1e7;
        float min_y = 1e7;
        float max_x = -1e7;
        float max_y = -1e7;

        // project the corners to the screen
        for(auto& corner:corners){
            auto projected_coord = gl::getScreenCoordAndDepth(corner,mvp,width,height);
            auto scr_coord = projected_coord.xy();
            //update the min and max
            min_x = std::min(min_x,scr_coord.x());
            min_y = std::min(min_y,scr_coord.y());
            max_x = std::max(max_x,scr_coord.x());
            max_y = std::max(max_y,scr_coord.y());
        }

        span.x() = max_x - min_x;
        span.y() = max_y - min_y;

        return span;   
    }

    Vertex& setVertex(uint u, uint v)
    {   
        assert(u <= U_SEGMENTS && v <= V_SEGMENTS);
        return _grids[u][v];
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
    std::array<std::pair<float,float>,3> _bounding_volume;
    uint V_SEGMENTS = 3;
    uint U_SEGMENTS = 3;
    std::vector<std::vector<Vertex>> _grids;
    std::vector<size_t> _indices;
};
