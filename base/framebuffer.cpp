#include "./framebuffer.hpp"

FrameBuffer::FrameBuffer(uint width, uint height, uint m, uint n)
{
    this->width = width;
    this->height = height;
    this->_pixel_color.resize(width);
    this->_buffers.resize(width);

    for (uint i = 0; i < width; i++)
    {
        this->_pixel_color[i].resize(height);
        this->_buffers[i].resize(height);
    }

    setSampleNumber(m, n);
}

void FrameBuffer::setPixelColor(uint u, uint v, gl::vec3 color)
{
    this->_pixel_color[u][v] = color;
}

void FrameBuffer::initBuffers(){
    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            this->_buffers[i][j] = std::vector<Sample>(this->getSampleNumber(), Sample());
        }
    }
};

void FrameBuffer::setPixelColorFromBuffer(){
    clearColor();

    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            for (uint k = 0; k < this->getSampleNumber(); k++)
            {
                this->_pixel_color[i][j] += this->_buffers[i][j][k].color;
            }

            this->_pixel_color[i][j] /= (float)this->getSampleNumber();
        }
    }
};

void FrameBuffer::setSampleNumber(uint m, uint n)
{
    this->sample_offset.clear();
    // sample_offset offset in the grid
    for (uint i = 0; i < m; i++)
    {
        for (uint j = 0; j < n; j++)
        {
            float i_seg = (float)i / (float)m;
            float i_seg_next = (float)(i + 1) / (float)m;
            float j_seg = (float)j / (float)n;
            float j_seg_next = (float)(j + 1) / (float)n;
            gl::vec2 offset = {gl::rand_num(i_seg, i_seg_next), gl::rand_num(j_seg, j_seg_next)};
            this->sample_offset.push_back(offset);
        }
    }

    initBuffers();
};

void FrameBuffer::clearBuffer(buffer_type b_type)
{
    if(b_type == buffer_type::color){
        for (uint i = 0; i < width; i++)
        {
            for (uint j = 0; j < height; j++)
            {
                for (uint k = 0; k < this->getSampleNumber(); k++)
                {
                    this->_buffers[i][j][k].color = gl::vec3(0.0);
                }
            }
        }
    }else if (b_type == buffer_type::depth){
        for (uint i = 0; i < width; i++)
        {
            for (uint j = 0; j < height; j++)
            {
                for (uint k = 0; k < this->getSampleNumber(); k++)
                {
                    this->_buffers[i][j][k].depth = 1.0;
                }
            }
        }
    }
};

void FrameBuffer::updateBufferByMicropolygon(Micropolygon &mp,gl::mat4 mvp){
    auto bbox = mp.getProjectedBoundingBox(mvp, this->width, this->height);
    auto projected_mp_pos = mp.getProjectedRectPos(mvp, this->width, this->height);

    //for each pixel touched by the bounding box
    for (uint i = (uint)bbox[0]; i < (uint)(ceil(bbox[1])); i++)
    {
        for (uint j = (uint)bbox[2]; j < (uint)(ceil(bbox[3])); j++)
        {   

            if(i>=this->width || j>=this->height ||i<0||j<0)
                continue;
            
            //for each of the sampling point
            for (uint k = 0; k < this->getSampleNumber(); k++){
                //get the offset
                gl::vec2 offset = this->sample_offset[k];
                //get the sample point
                gl::vec2 sample_point = {i+offset.x(),j+offset.y()};
                //test if the sample point is inside the micropolygon
                bool is_inside = gl::is_inside_rect(sample_point,projected_mp_pos[0].xy(),projected_mp_pos[1].xy(),projected_mp_pos[2].xy(),projected_mp_pos[3].xy());
                if(!is_inside)
                    continue;
                
                float sample_depth = 0.25*(projected_mp_pos[0].z()+projected_mp_pos[1].z()+projected_mp_pos[2].z()+projected_mp_pos[3].z());
                if(isfinite(sample_depth)&&sample_depth<=1.0&&sample_depth>=0.0){
                    //if sample point if further than zbuffer, skip
                    if(sample_depth>=_buffers[i][j][k].depth)
                        continue;

                    //suceessfully sample a point,update the buffer
                    _buffers[i][j][k].depth = sample_depth;
                    _buffers[i][j][k].color = mp.v0.position;
                }
            }
        }
    }

    return;
};

cv::Mat FrameBuffer::to_cv_mat()
{
    cv::Mat image(getWidth(), getHeight(), CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
    image.convertTo(image, CV_8UC3, 1.0f);
    for (uint i = 0; i < getWidth(); i++)
    {
        for (uint j = 0; j < getHeight(); j++)
        {
            auto color = getPixelColor(i, j);
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x() * 255, color.y() * 255, color.z() * 255);
        }
    }
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    return image;
}