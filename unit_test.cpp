#include "./unit_test.hpp"
// #define TEST_BILINEAR
#define TEST_SAMPLING
// #define TEST_SAMPLE_VIS
// #define TEST_FRAMEBUFFER
int main()
{

#ifdef TEST_BILINEAR
    {
        using namespace gl;
        vec3 x2y2(1.0f, 1.0f, 1.0f);
        vec3 x2y1(1.0f, 0.0f, 0.0f);
        vec3 x1y2(0.0f, 1.0f, 0.0f);
        vec3 x1y1(0.0f, 0.0f, 1.0f);

        vec2 screen_coord(0.5,0.5);

        std::cout<< get_depth_bilinear(screen_coord, x1y1, x2y1, x2y2, x1y2) << std::endl;

    }
 #endif

#ifdef TEST_SAMPLING
    {   
        //MSAA 16x
        FrameBuffer fb(800, 800, 4, 4);
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 1000, 1000);
        //Hyperboloid sphere(gl::vec3(-1.f,-1.f,-1.f),gl::vec3(1.f,1.f,1.f),gl::to_radian(360.0f), 1000, 1000);

        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(0.0f, 0.0f, 4.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(90.0f));
        sphere.rotation = q * sphere.rotation;

        auto [w, h] = sphere.getResolution();
        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), (float)width/(float)height, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        fb.clearColor();
        fb.clearBuffer(buffer_type::depth);

        // loop over micropolygons
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                auto mpoly = sphere.getMicropolygon(i, j);
                fb.updateBufferByMicropolygon(mpoly,mvp);
            }
        }

        fb.setPixelColorFromBuffer();
        auto img = fb.to_cv_mat();
        cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
        cv::imshow("image", img);
        cv::waitKey();
    }

#endif

    //////////////////////////////////////////
    // Below are general test code for visualizing samples
    //////////////////////////////////////////

#ifdef TEST_SAMPLE_VIS
    {
        FrameBuffer fb(800, 800, 4, 4);
        std::cout << fb.getSampleNumber() << std::endl;
        auto sample_offset = fb.getSampleOffset();
        for (auto &offset : sample_offset)
        {
            std::cout << offset.x() << " " << offset.y() << std::endl;
        }

        int key = 0;
        while (key != 27)
        {

            cv::Mat image(100, 100, CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
            image.convertTo(image, CV_8UC3, 1.0f);
            // set the color of the pixel, of the

            // Vertex v = sphere.getVertex(i);
            //  std::cout << v.position << std::endl;
            for (auto &offset : sample_offset)
            {
                image.at<cv::Vec3b>((int)(offset.x() * 100), (int)(offset.y() * 100)) = cv::Vec3b((int)(255), (int)(offset.y() * 255), 1.0f);
            }

            // show the boundary of the grid
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 100; j++)
                {
                    if ((i + 1) % 25 == 0 || (j + 1) % 25 == 0)
                        image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                }
            }

            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            cv::imshow("image", image);
            auto key = cv::waitKey(1000);
        }
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for custom framebuffer class
    //////////////////////////////////////////

#ifdef TEST_FRAMEBUFFER
    {
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f), 50, 50);
        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(0.0f, 0.0f, 4.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
        sphere.rotation = q * sphere.rotation;
        auto [w, h] = sphere.getResolution();

        uint width = 800;
        uint height = 800;

        PerspectiveCamera p_cam(gl::to_radian(45.0f), 1.0f, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        int key = 0;
        while (key != 27)
        {
            FrameBuffer fb(width, height);

            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto v = sphere.getVertex(i, j);
                    auto v_screen = gl::getScreenCoord(v.position, mvp, width, height);
                    // setting up white color, no sampling
                    fb.setPixelColor((int)v_screen.x(), (int)(v_screen.y()), gl::vec3(1.0f, 1.0f, 1.0f));
                }
            }

            auto img = fb.to_cv_mat();
            cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
            cv::imshow("image", img);
            auto key = cv::waitKey(1000);
        }
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for getScreenCoord function
    //////////////////////////////////////////

#ifdef TEST_MVP
    {
        Sphere sphere(1.0, -1.0, 1.0, gl::to_radian(360.0f));
        sphere.scale = gl::vec3(0.9f);
        sphere.position = gl::vec3(0.0f, 0.0f, 4.0f);
        gl::Quat q = gl::Quat::fromAxisAngle(gl::vec3(0.0f, 1.0f, 0.0f), gl::to_radian(-90.0f));
        sphere.rotation = q * sphere.rotation;
        auto [w, h] = sphere.getResolution();

        PerspectiveCamera p_cam(gl::to_radian(45.0f), 1.0f, 0.1f, 100.0f);
        gl::mat4 mvp = p_cam.getProjectionMat() * p_cam.getViewMat() * sphere.getModelMat();

        uint width = 800;
        uint height = 800;
        int key = 0;
        while (key != 27)
        {
            cv::Mat image(width, height, CV_32FC3, cv::Scalar(0.0f, 0.0f, 0.0f));
            image.convertTo(image, CV_8UC3, 1.0f);

            auto [w, h] = sphere.getResolution();
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    auto v = sphere.getVertex(i, j);
                    auto v_screen = gl::getScreenCoord(v.position, mvp, width, height);
                    image.at<cv::Vec3b>((int)v_screen.x(), (int)(v_screen.y())) = cv::Vec3b((int)(v.position.x() * 255), (int)(v.position.y() * 255), 255);
                }
            }

            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            cv::imshow("image", image);
            auto key = cv::waitKey(1000);
        }
    }
#endif

    //////////////////////////////////////////
    // Below are general test code for vec and mat class
    //////////////////////////////////////////
#ifdef TEST_VEC
    {
        gl::vec2 v2(1.0f, 1.0f);
        gl::vec3 v3(1.0f, 2.0f, 3.0f);
        gl::vec4 v4(1.0f, 1.0f, 1.0f, 1.0f);
        gl::vec4 v4_2{2.0f, 2.0f, 2.0f, 2.0f};

        std::cout << v2 + 1.f << std::endl;
        std::cout << v3 - 1.f << std::endl;
        std::cout << v2 * 1.f << std::endl;
        std::cout << 1.f * v2 << std::endl;

        std::cout << v2.normalized() << std::endl;
        std::cout << v3.normalized() << std::endl;
        std::cout << v4.normalized() << std::endl;
        std::cout << v4_2.normalized() << std::endl;

        std::cout << gl::dot(v4, v4_2) << std::endl;
        std::cout << gl::cross(v3, v3) << std::endl;

        // below are test cases for matrix calculation
        gl::mat4 m4(1.0f);
        gl::mat4 m4_2(2.0f);
        gl::mat4 m4_3(3.0f);

        gl::mat2 m2{1.0f, 2.0f, 3.0f, 4.0f};
        gl::mat2 m2_2{gl::vec2(1.0f), gl::vec2(2.0f)};
        gl::mat2 m2_3{{1, 2.0}, {2.0, 1.0}};

        gl::mat3 m3{6, 1, 1, 4, -2, 5, 2, 8, 7};
        gl::mat4 m4_4{13, 2, 0, 0, 2, 2, 0, 0, 1, 1, 4, 1, 1, 1, 1, 4};

        std::cout << m2 << std::endl;
        std::cout << m2_2 << std::endl;
        std::cout << m2_3 << std::endl;

        std::cout << m4_4 << std::endl;

        std::cout << m4 << std::endl;
        std::cout << m4_2 << std::endl;

        std::cout << m4 * m4_2 << std::endl;
        std::cout << m4 * m4_2 * m4_3 << std::endl;

        // matrix * vector
        std::cout << m4_2 * v4 << std::endl;

        // matrix * scalar
        std::cout << m4_2 * 2.0f << std::endl;

        // matrix transpose
        std::cout << m2.transposed() << std::endl;

        // matrix determinant
        std::cout << m3.determinant() << std::endl;
        std::cout << m4_4.determinant() << std::endl;

        // matrix inverse
        std::cout << m4_4.inversed() << std::endl;

        Object3D obj;
        std::cout << obj.getFront() << std::endl;
        std::cout << obj.getUp() << std::endl;
        std::cout << obj.getRight() << std::endl;
        std::cout << obj.getModelMat() << std::endl;

        std::cout << std::endl;
        // rotate the object around the y axis, 45 degrees to radian
        obj.rotation = gl::Quat::fromAxisAngle({0.0f, 1.0f, 0.0f}, gl::to_radian(45.0f));
        std::cout << obj.getFront() << std::endl;
        std::cout << obj.getUp() << std::endl;
        std::cout << obj.getRight() << std::endl;
        std::cout << obj.getModelMat() << std::endl;

        // this test has to pass the points in order to get the correct result
        std::cout << gl::is_inside_rect({0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}) << std::endl;
    }
#endif

    return 0;
}