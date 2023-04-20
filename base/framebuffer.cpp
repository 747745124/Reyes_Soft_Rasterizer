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

void FrameBuffer::initBuffers()
{
    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            this->_buffers[i][j] = std::vector<std::vector<Sample>>(this->getSampleNumber(), std::vector<Sample>(1, Sample()));
        }
    }
};

// it's more like a dump color buffer to pixel color, since color buffer is cleared
void FrameBuffer::setPixelColorFromBuffer()
{

    clearColor();
    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            for (uint k = 0; k < this->getSampleNumber(); k++)
            {

                sort(this->_buffers[i][j][k].begin(), this->_buffers[i][j][k].end(), [](Sample &a, Sample &b)
                     { return a.depth > b.depth; });

                gl::vec3 sample_color = gl::vec3(0.0);
                // sorted from far to near
                for (auto &sample : this->_buffers[i][j][k])
                {

                    if (sample.color.a() == 1.0)
                        sample_color = sample.color.rgb();

                    // alpha blending
                    sample_color = sample_color * (1.0f - sample.color.a()) + sample.color.rgb() * sample.color.a();
                }

                this->_pixel_color[i][j] += sample_color.rgb();
            }
            this->_pixel_color[i][j] /= (float)this->getSampleNumber();
        }
    }
};

// it's more like a dump depth buffer to pixel color
void FrameBuffer::setPixelDepthFromBuffer(float znear, float zfar)
{
    clearColor();
    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {   
            float sample_depth_sum = 0.0f;
            for (uint k = 0; k < this->getSampleNumber(); k++)
            {
                // sorted from near to far
                sort(this->_buffers[i][j][k].begin(), this->_buffers[i][j][k].end(), [](Sample &a, Sample &b)
                     { return a.depth < b.depth; });
                sample_depth_sum += this->_buffers[i][j][k][0].depth;
            }

            //linearize step
            float pixel_depth = sample_depth_sum/(float)this->getSampleNumber();
            float z = 2.0f * pixel_depth - 1.0f; // back to ndc
            float z_n = (2.0f * znear) / (zfar + znear - z * (zfar - znear));
            this->_pixel_color[i][j] = gl::vec3(z_n);
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

void FrameBuffer::clearBuffer(float bg_depth, gl::vec4 bg_color)
{
    for (uint i = 0; i < width; i++)
    {
        for (uint j = 0; j < height; j++)
        {
            for (uint k = 0; k < this->getSampleNumber(); k++)
            {
                this->_buffers[i][j][k].clear();
                this->_buffers[i][j][k].push_back(Sample(1.0, bg_color));
            }
        }
    }
};

// this is the place where the shading happens
void FrameBuffer::updateBufferByMicropolygon(Micropolygon &mp, gl::mat4 mvp, LERP_MODE mode)
{
    auto bbox = mp.getProjectedBoundingBox(mvp, this->width, this->height);
    auto proj_mp_pos = mp.getProjectedRectPos(mvp, this->width, this->height);

    // for each pixel touched by the bounding box
    for (uint i = (uint)bbox[0]; i < (uint)(ceil(bbox[1])); i++)
    {
        for (uint j = (uint)bbox[2]; j < (uint)(ceil(bbox[3])); j++)
        {

            if (i >= this->width || j >= this->height || i < 0 || j < 0)
                continue;

            // for each of the sampling point
            for (uint k = 0; k < this->getSampleNumber(); k++)
            {
                // get the offset
                gl::vec2 offset = this->sample_offset[k];
                // get the sample point
                gl::vec2 sample_point = {i + offset.x(), j + offset.y()};
                // test if the sample point is inside the micropolygon
                bool is_inside = gl::is_inside_rect(sample_point, proj_mp_pos[0].xy(), proj_mp_pos[1].xy(), proj_mp_pos[2].xy(), proj_mp_pos[3].xy());
                if (!is_inside)
                    continue;

                // it's on a plane, so we can use the plane equation to get the depth
                Plane p(proj_mp_pos[0].xyz(), proj_mp_pos[1].xyz(), proj_mp_pos[2].xyz());
                // get the depth of the sample point
                float sample_depth = p.get_z(sample_point.x(), sample_point.y());

                if (isfinite(sample_depth) && sample_depth <= 1.0 && sample_depth >= 0.0)
                {
                    if (LERP_MODE::CORNER == mode)
                        // push the cornor color to the buffer
                        _buffers[i][j][k].push_back(Sample(sample_depth, mp.v0.baseColor));
                    else if (LERP_MODE::BILINEAR == mode)
                    {
                        auto uv = gl::inverseBilinear(sample_point, proj_mp_pos[0].xy(), proj_mp_pos[1].xy(), proj_mp_pos[2].xy(), proj_mp_pos[3].xy());
                        auto color = gl::bilinear(uv, mp.v0.baseColor, mp.v1.baseColor, mp.v2.baseColor, mp.v3.baseColor);
                        _buffers[i][j][k].push_back(Sample(sample_depth, color));
                    }
                    else
                    {
                        throw std::runtime_error("Invalid LERP_MODE");
                        return;
                    }
                }
            }
        }
    }

    return;
};

cv::Mat FrameBuffer::to_cv_mat(int FORMAT)
{
    cv::Mat image(getWidth(), getHeight(), CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));

    for (uint i = 0; i < getWidth(); i++)
    {
        for (uint j = 0; j < getHeight(); j++)
        {
            auto color = getPixelColor(i, j);
            image.at<cv::Vec3f>(j, i) = cv::Vec3f(color.x(), color.y(), color.z());
        }
    }

    if (FORMAT == CV_8UC3)
        image.convertTo(image, FORMAT, 255.0);
    else if (FORMAT == CV_32FC3)
        image.convertTo(image, FORMAT);
    else
        std::cout << "Unsupported format" << std::endl;

    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    return image;
}