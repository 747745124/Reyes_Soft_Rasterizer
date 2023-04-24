#include "./texture.hpp"
// Probably TODO: mipmap & tri-linear interpolation
Texture2D::Texture2D(const std::string path, int FORMAT) : _path(path)
{
    // read image from given path
    auto _image = cv::imread(_path.c_str(), cv::IMREAD_COLOR);
    if (_image.empty())
        throw std::runtime_error("Failed to load texture");
    _image.convertTo(_image, FORMAT, 1.0 / 255.0);
    cv::cvtColor(_image, _image, cv::COLOR_BGR2RGB);

    // get the width and height of the image
    this->_width = _image.cols;
    this->_height = _image.rows;

    if (FORMAT == CV_32FC3)
    {
        // read the image into the texels
        // note: it's row major in opencv
        for (uint j = 0; j < _width; j++)
        {
            std::vector<gl::vec3> row;
            for (uint i = 0; i < _height; i++)
            {
                cv::Vec3f color = _image.at<cv::Vec3f>(j, i);
                row.push_back(gl::vec3(color[2], color[1], color[0]));
            }
            texels.push_back(row);
        }
    }
    else if (FORMAT == CV_8UC3)
    {
        // read the image into the texels
        for (uint j = 0; j < _width; j++)
        {
            std::vector<gl::vec3> row;
            for (uint i = 0; i < _height; i++)
            {
                cv::Vec3b color = _image.at<cv::Vec3b>(j, i);
                row.push_back(gl::vec3(color[0], color[1], color[2]));
            }
        }
    }
    else
    {
        throw std::runtime_error("Unsupported format");
    }

    _image.release();
    return;
};

gl::vec3 Texture2D::getTexelColor(float u, float v, LERP_MODE mode)
{
    uint x = 0;
    uint y = 0;

    if (mode == LERP_MODE::NEAREST)
    {
        // get the texel coordinate
        x = (uint)(u * _width);
        y = (uint)(v * _height);
        // clamp the texel coordinate
        x = std::clamp(x, 0u, _width - 1);
        y = std::clamp(y, 0u, _height - 1);

        return texels[x][y];
    }
    else if (mode == LERP_MODE::BILINEAR)
    {
        // get the texel coordinate
        x = (uint)(u * _width);
        y = (uint)(v * _height);

        // get the texel coordinate
        float x1 = (u * _width) - x;
        float y1 = (v * _height) - y;

        x = std::clamp(x, 0u, _width - 2);
        y = std::clamp(y, 0u, _height - 2);

        // get the texel coordinate
        float x2 = 1.0 - x1;
        float y2 = 1.0 - y1;

        // get the texel coordinate
        gl::vec3 c1 = texels[x][y];
        gl::vec3 c2 = texels[x + 1][y];
        gl::vec3 c3 = texels[x][y + 1];
        gl::vec3 c4 = texels[x + 1][y + 1];

        // get the texel coordinate
        gl::vec3 c12 = c1 * x2 + c2 * x1;
        gl::vec3 c34 = c3 * x2 + c4 * x1;

        // get the texel coordinate
        gl::vec3 c1234 = c12 * y2 + c34 * y1;

        return c1234;
    }
    else
    {
        throw std::runtime_error("Unsupported lerp mode");
    }

    return gl::vec3(0.0f);
};

TextureShadow::TextureShadow(uint width, uint height) : _width(width), _height(height)
{
    // initialize the texels
    for (uint i = 0; i < _width; i++)
    {
        std::vector<float> row;
        for (uint j = 0; j < _height; j++)
        {
            row.push_back(1.0f);
        }
        texels.push_back(row);
    }
};

void TextureShadow::setTexelDepth(uint x, uint y, float depth)
{
    assert(x >= 0 && x < _width);
    assert(y >= 0 && y < _height);
    texels[x][y] = depth;
};

float TextureShadow::getTexelDepth(float u, float v, LERP_MODE mode)
{
    assert(u >= 0 && u < _width);
    assert(v >= 0 && v < _height);

    if (mode == LERP_MODE::NEAREST)
    {
        // get the texel coordinate
        uint x = (uint)(u * _width);
        uint y = (uint)(v * _height);

        // clamp the texel coordinate
        x = std::clamp(x, 0u, _width - 1);
        y = std::clamp(y, 0u, _height - 1);

        return texels[x][y];
    }
    else if (mode == LERP_MODE::BILINEAR)
    {
        // get the texel coordinate
        uint x = (uint)(u * _width);
        uint y = (uint)(v * _height);

        // get the texel coordinate
        float x1 = (u * _width) - x;
        float y1 = (v * _height) - y;

        x = std::clamp(x, 0u, _width - 2);
        y = std::clamp(y, 0u, _height - 2);

        // get the texel coordinate
        float x2 = 1.0 - x1;
        float y2 = 1.0 - y1;

        // get the texel coordinate
        float c1 = texels[x][y];
        float c2 = texels[x + 1][y];
        float c3 = texels[x][y + 1];
        float c4 = texels[x + 1][y + 1];

        // get the texel coordinate
        float c12 = c1 * x2 + c2 * x1;
        float c34 = c3 * x2 + c4 * x1;

        // get the texel coordinate
        float c1234 = c12 * y2 + c34 * y1;

        return c1234;
    }
    else
    {
        throw std::runtime_error("Unsupported lerp mode");
    }
};

cv::Mat TextureShadow::to_cv_mat()
{
    cv::Mat image(_width, _height, CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));

    for (uint i = 0; i < _width; i++)
    {
        for (uint j = 0; j < _height; j++)
        {
            auto color = texels[i][j];
            image.at<cv::Vec3f>(_height - j - 1, i) = cv::Vec3f(color, color, color);
        }
    }

    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    return image;
}

void TextureShadow::updateByMicropolygon(Micropolygon &mp, gl::mat4 lightmodel)
{
    auto bbox = mp.getProjectedBoundingBox(lightmodel, this->_width, this->_height);
    auto proj_mp_pos = mp.getProjectedRectPos(lightmodel, this->_width, this->_height);

    // for each pixel touched by the bounding box
    for (uint i = (uint)bbox[0]; i < (uint)(ceil(bbox[1])); i++)
    {
        for (uint j = (uint)bbox[2]; j < (uint)(ceil(bbox[3])); j++)
        {

            if (i >= this->_width || j >= this->_height || i < 0 || j < 0)
                continue;

            // get the sample point
            gl::vec2 sample_point = {(float)i + 0.5f, (float)j + 0.5f};
            // test if the sample point is inside the micropolygon
            bool is_inside = gl::is_inside_rect(sample_point, proj_mp_pos[0].xy(), proj_mp_pos[1].xy(), proj_mp_pos[2].xy(), proj_mp_pos[3].xy());
            if (!is_inside)
                continue;

            // it's on a plane, so we can use the plane equation to get the depth
            Plane p(proj_mp_pos[0].xyz(), proj_mp_pos[1].xyz(), proj_mp_pos[2].xyz());
            // get the depth of the sample point
            float sample_depth = p.get_z(sample_point.x(), sample_point.y()) * 0.5 + 0.5;

            if (isfinite(sample_depth) && sample_depth <= 1.0 && sample_depth >= 0.0)
            {
                texels[i][j] = std::min(sample_depth, texels[i][j]);
            }
        }
    }
};

void TextureShadow::renderToTextureShadow(Mesh &mesh, gl::mat4 lightmatrix)
{
    gl::mat4 lightmodel = lightmatrix * mesh.getModelMat();
    const auto [w, h] = mesh.getResolution();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            auto mp = mesh.getMicropolygon(i, j);
            updateByMicropolygon(mp, lightmodel);
        }
    }

    return;
};