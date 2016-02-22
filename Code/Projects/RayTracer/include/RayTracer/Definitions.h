#pragma once
#include <Eigen\Dense>

namespace RayTracer
{
    using Type = double;

    template<size_t rows, size_t columns>
    using Matrix = Eigen::Matrix<Type, rows, columns>;

    using Vector2 = Matrix<2, 1>;
    using Vector3 = Matrix<3, 1>;
}