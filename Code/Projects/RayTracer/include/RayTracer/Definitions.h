#pragma once
#include <Eigen\Dense>

namespace RayTracer
{
    using Float = double;
    using Integer = int;

    template<typename T>
    using Vector2 = Eigen::Matrix<T, 2, 1>;

    template<typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;

    using Vector2i = Vector2<Integer>;
    using Vector3i = Vector3<Integer>;
    using Vector2f = Vector2<Float>;
    using Vector3f = Vector3<Float>;
}