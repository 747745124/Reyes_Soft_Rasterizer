#include "./unit_test.hpp"

int main()
{
    // below are unit test for vec class
    // gl::vec2 v2(1.0f, 1.0f);
    // gl::vec3 v3(1.0f, 2.0f, 3.0f);
    // gl::vec4 v4(1.0f, 1.0f, 1.0f, 1.0f);
    // gl::vec4 v4_2{2.0f, 2.0f, 2.0f, 2.0f};

    // std::cout << v2 + 1.f << std::endl;
    // std::cout << v3 - 1.f << std::endl;
    // std::cout << v2 * 1.f << std::endl;
    // std::cout << 1.f * v2 << std::endl;

    // std::cout << v2.normalized() << std::endl;
    // std::cout << v3.normalized() << std::endl;
    // std::cout << v4.normalized() << std::endl;
    // std::cout << v4_2.normalized() << std::endl;

    // std::cout << gl::dot(v4, v4_2) << std::endl;
    // std::cout << gl::cross(v3, v3) << std::endl;

    // below are test cases for matrix calculation
    // gl::mat4 m4(1.0f);
    // gl::mat4 m4_2(2.0f);
    // gl::mat4 m4_3(3.0f);

    // gl::mat2 m2{1.0f, 2.0f, 3.0f, 4.0f};
    // gl::mat2 m2_2{gl::vec2(1.0f), gl::vec2(2.0f)};
    // gl::mat2 m2_3{{1, 2.0}, {2.0, 1.0}};

    // gl::mat3 m3{6, 1, 1, 4, -2, 5, 2, 8, 7};
    // gl::mat4 m4_4{13, 2, 0, 0, 2, 2, 0, 0, 1, 1, 4, 1, 1, 1, 1, 4};

    // std::cout << m2 << std::endl;
    // std::cout << m2_2 << std::endl;
    // std::cout << m2_3 << std::endl;

    // std::cout << m4_4 << std::endl;

    // std::cout << m4 << std::endl;
    // std::cout << m4_2 << std::endl;

    // std::cout << m4 * m4_2 << std::endl;
    // std::cout << m4 * m4_2 * m4_3 << std::endl;

    // // matrix * vector
    // std::cout << m4_2 * v4 << std::endl;

    // // matrix * scalar
    // std::cout << m4_2 * 2.0f << std::endl;

    // // matrix transpose
    // std::cout << m2.transposed() << std::endl;

    // // matrix determinant
    // std::cout << m3.determinant() << std::endl;
    // std::cout << m4_4.determinant() << std::endl;

    // // matrix inverse
    // std::cout << m4_4.inversed() << std::endl;

    Object3D obj;
    std::cout << obj.getFront() << std::endl;
    std::cout << obj.getUp() << std::endl;
    std::cout << obj.getRight() << std::endl;
    // std::cout << obj.getModelMat() << std::endl;

    std::cout << std::endl;
    // rotate the object around the y axis, 45 degrees to radian
    obj.rotation = gl::Quat::fromAxisAngle({0.0f, 1.0f, 0.0f}, gl::to_radian(45.0f));
    std::cout << obj.getFront() << std::endl;
    std::cout << obj.getUp() << std::endl;
    std::cout << obj.getRight() << std::endl;
    // std::cout << obj.getModelMat() << std::endl;
    return 0;
}