#include "RayTracer/VectorOperations.h"

namespace RayTracer
{
    Vector3f Reflect(const Vector3f& target, const Vector3f& normal)
    {
        const Vector3f normalized = normal.normalized();
        Vector3f res = target - 2 * normalized * (target.dot(normalized));
        res.normalize();
        return res;
    }

    Vector3f Refract(const Vector3f& target,
        Vector3f normal, const Float& n1,
        Float n2, Float& quitN)
    {
        normal.normalize();
        if (normal.dot(target) > 0)
        {
            normal -= normal;
            n2 = quitN;
        }
        quitN = n2;
        const Float r = n1 / n2;
        const Float cosi = -(normal.dot(target));

        const Float sini = std::min<Float>(1, r * r * (1 - cosi * cosi));
        Vector3f res = r * target + (r * cosi - std::sqrt(1 - sini)) * normal;
        res.normalize();

        return res;
    }
}