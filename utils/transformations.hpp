#pragma once
#include "./matrix.hpp"
#include "./quat.hpp"

namespace gl
{
    // convert degree to radians
    static float to_radian(float degree)
    {
        return degree * M_PI / 180;
    };
    // 2D rotation matrix given an angle in radians
    static mat2 rotation2D(float angle)
    {
        return mat2{cos(angle), -sin(angle), sin(angle), cos(angle)};
    };

    // 3D rotation matrix given an angle in radians and an axis
    static mat4 rotation3D_axis_angle(float angle, vec3 axis)
    {
        axis = axis.normalize();
        float x = axis[0];
        float y = axis[1];
        float z = axis[2];
        float c = cos(angle);
        float s = sin(angle);
        float C = 1 - c;
        return mat4{
            x * x * C + c, x * y * C - z * s, x * z * C + y * s, 0,
            y * x * C + z * s, y * y * C + c, y * z * C - x * s, 0,
            x * z * C - y * s, y * z * C + x * s, z * z * C + c, 0,
            0, 0, 0, 1};
    };

    // 3D rotation matrix given an yaw pitch and roll
    static mat4 rotation3D_ypr(float yaw, float pitch, float roll)
    {
        return mat4{
            cos(yaw) * cos(pitch), cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll), cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll), 0,
            sin(yaw) * cos(pitch), sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll), sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll), 0,
            -sin(pitch), cos(pitch) * sin(roll), cos(pitch) * cos(roll), 0,
            0, 0, 0, 1};
    };

    // 3D translation matrix given a translation vector
    static mat4 translation3D(vec3 translation)
    {
        return mat4{
            1, 0, 0, translation[0],
            0, 1, 0, translation[1],
            0, 0, 1, translation[2],
            0, 0, 0, 1};
    };

    // 3D scaling matrix given a scaling vector
    static mat4 scale3D(vec3 scaling)
    {
        return mat4{
            scaling[0], 0, 0, 0,
            0, scaling[1], 0, 0,
            0, 0, scaling[2], 0,
            0, 0, 0, 1};
    };

    // 3D perspective matrix given a field of view, aspect ratio, near and far planes
    // Renderman is a little bit different (LH)
    // -1  ～ 1 for x & y, but 0 ～ 1 for z
    static mat4 getPerspectiveMatLH(float fov, float aspect, float hither, float yon)
    {

        return mat4{
            1 / tan(fov / 2), 0, 0, 0,
            0, 1 / (aspect * tan(fov / 2)), 0, 0,
            0, 0, yon / (yon - hither), -yon * hither / (yon - hither),
            0, 0, 1, 0};
    };

    // opengl style perspective matrix (RH)
    static mat4 getPerspectiveMatRH(float fov, float aspect, float znear, float zfar)
    {

        return mat4{
            1 / tan(fov / 2), 0, 0, 0,
            0, 1 / tan(fov / (2 * aspect)), 0, 0,
            0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear),
            0, 0, -1, 0};
    };

    // 3D orthographic matrix given a left, right, bottom, top, near and far planes
    static mat4 getOrthoMatRH(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        return mat4{
            2 / (right - left), 0, 0, -(right + left) / (right - left),
            0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
            0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
            0, 0, 0, 1};
    };

    //LH_ZO according to glm
    static mat4 getOrthoMatLH(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        return mat4{
            2 / (right - left), 0, 0, -(right + left) / (right - left),
            0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
            0, 0, 1 / (zFar - zNear), -zNear / (zFar - zNear),
            0, 0, 0, 1};
    };


    // 3D view matrix given position, center (camera lookat position) and up vector
    static mat4 getViewMat(vec3 position, vec3 center, vec3 up)
    {
        vec3 f = (center - position).normalize();
        vec3 s = cross(up, f).normalize();
        vec3 u = cross(f, s);
        return mat4{
            s[0], u[0], f[0], 0,
            s[1], u[1], f[1], 0,
            s[2], u[2], f[2], 0,
            -dot(s, position), -dot(u, position), -dot(f, position), 1};
    };

    // 3D model matrix given position, rotation and scale
    static mat4 getModelMat(vec3 position, Quat rotation, vec3 scale)
    {
        return translation3D(position) * rotation.to_mat4() * scale3D(scale);
    }

    // returns the converted screen coord ranging from 0 to max_width and 0 to max_height
    // fp32 precision
    static gl::vec2 getScreenCoord(gl::vec3 worldCoord, gl::mat4 mvp, int width, int height)
    {
        gl::vec4 clipCoord = mvp * gl::vec4(worldCoord, 1.0f);
        gl::vec4 ndcCoord = gl::vec4(clipCoord) / (clipCoord.w()+1e-6);
        gl::vec2 screenCoord = gl::vec2((ndcCoord[0] + 1.0f) / 2.0f * width, (ndcCoord[1] + 1.0f) / 2.0f * height);
        return screenCoord;
    }

    static gl::vec3 getScreenCoordAndDepth(gl::vec3 worldCoord, gl::mat4 mvp, int width, int height)
    {
        gl::vec4 clipCoord = mvp * gl::vec4(worldCoord, 1.0f);
        gl::vec4 ndcCoord = gl::vec4(clipCoord) / (clipCoord.w()+1e-6);
        gl::vec3 screenCoord = gl::vec3((ndcCoord[0] + 1.0f) / 2.0f * width, (ndcCoord[1] + 1.0f) / 2.0f * height, ndcCoord[2]);
        // std::cout<<screenCoord<<std::endl;
        return screenCoord;
    }
};
