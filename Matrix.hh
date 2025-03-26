#ifndef LIGHTNING3D_MATRIX_HH_
#define LIGHTNING3D_MATRIX_HH_

#include <cstddef>
#include <cstring>
#include <numbers>

#include "Vector.hh"

namespace Lightning3D {
    template <size_t M, size_t N>
    struct MatrixStorage {
        Vector<float, N> data[M];
    };

    template <size_t M, size_t N, typename TMatrix>
    class MatrixBase : public MatrixStorage<M, N> {
    public:
        auto Transpose(void) const -> TMatrix requires (N == M) {
            return {
                this->data[0].x, this->data[1].x, this->data[2].x, this->data[3].x,
                this->data[0].y, this->data[1].y, this->data[2].y, this->data[3].y,
                this->data[0].z, this->data[1].z, this->data[2].z, this->data[3].z,
                this->data[0].w, this->data[1].w, this->data[2].w, this->data[3].w
            };
        }

        static auto Identity(void) -> TMatrix requires (M == N) {
            return {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
    };

    template <size_t M, size_t N>
    struct Matrix : public MatrixBase<M, N, Matrix<M, N>> {
        Matrix()
            : MatrixStorage<M, N>{ } {
        }

        template <typename ...U>
        Matrix(U ...values)
            : MatrixBase<M, N, Matrix<M, N>>{ } {

            std::memcpy(
                this->data,
                std::array<float, M *N>{ values... }.data(),
                sizeof(float) * sizeof...(values)
            );
        }
    };
    //
    using Mat3x3 = Matrix<3u, 3u>;
    using Mat3x4 = Matrix<3u, 4u>;
    using Mat4x4 = Matrix<4u, 4u>;

    inline auto operator*(const Mat4x4 &lhs, const Mat4x4 &rhs) -> Mat4x4 {
        return {
            Dot(lhs.data[0], Vec4F(rhs.data[0].x, rhs.data[1].x, rhs.data[2].x, rhs.data[3].x)),
            Dot(lhs.data[0], Vec4F(rhs.data[0].y, rhs.data[1].y, rhs.data[2].y, rhs.data[3].y)),
            Dot(lhs.data[0], Vec4F(rhs.data[0].z, rhs.data[1].z, rhs.data[2].z, rhs.data[3].z)),
            Dot(lhs.data[0], Vec4F(rhs.data[0].w, rhs.data[1].w, rhs.data[2].w, rhs.data[3].w)),

            Dot(lhs.data[1], Vec4F(rhs.data[0].x, rhs.data[1].x, rhs.data[2].x, rhs.data[3].x)),
            Dot(lhs.data[1], Vec4F(rhs.data[0].y, rhs.data[1].y, rhs.data[2].y, rhs.data[3].y)),
            Dot(lhs.data[1], Vec4F(rhs.data[0].z, rhs.data[1].z, rhs.data[2].z, rhs.data[3].z)),
            Dot(lhs.data[1], Vec4F(rhs.data[0].w, rhs.data[1].w, rhs.data[2].w, rhs.data[3].w)),

            Dot(lhs.data[2], Vec4F(rhs.data[0].x, rhs.data[1].x, rhs.data[2].x, rhs.data[3].x)),
            Dot(lhs.data[2], Vec4F(rhs.data[0].y, rhs.data[1].y, rhs.data[2].y, rhs.data[3].y)),
            Dot(lhs.data[2], Vec4F(rhs.data[0].z, rhs.data[1].z, rhs.data[2].z, rhs.data[3].z)),
            Dot(lhs.data[2], Vec4F(rhs.data[0].w, rhs.data[1].w, rhs.data[2].w, rhs.data[3].w)),

            Dot(lhs.data[3], Vec4F(rhs.data[0].x, rhs.data[1].x, rhs.data[2].x, rhs.data[3].x)),
            Dot(lhs.data[3], Vec4F(rhs.data[0].y, rhs.data[1].y, rhs.data[2].y, rhs.data[3].y)),
            Dot(lhs.data[3], Vec4F(rhs.data[0].z, rhs.data[1].z, rhs.data[2].z, rhs.data[3].z)),
            Dot(lhs.data[3], Vec4F(rhs.data[0].w, rhs.data[1].w, rhs.data[2].w, rhs.data[3].w))
        };
    }

    //inline auto operator*(const Vec3F &vec, const Mat4x4 &mat) -> Vec3F {
    //    return {
    //        mat.data[0].x * vec.x + mat.data[0].y * vec.x + mat.data[0].z * vec.x + mat.data[0].w * vec.x,
    //        mat.data[1].x * vec.y + mat.data[1].y * vec.y + mat.data[1].z * vec.y + mat.data[1].w * vec.y,
    //        mat.data[2].x * vec.z + mat.data[2].y * vec.z + mat.data[2].z * vec.z + mat.data[2].w * vec.z
    //    };
    //}

    inline auto operator*(const Vec4F &vec, const Mat4x4 &mat) -> Vec4F {
        return {
            Dot(vec, Vec4F(mat.data[0].x, mat.data[1].x, mat.data[2].x, mat.data[3].x)),
            Dot(vec, Vec4F(mat.data[0].y, mat.data[1].y, mat.data[2].y, mat.data[3].y)),
            Dot(vec, Vec4F(mat.data[0].z, mat.data[1].z, mat.data[2].z, mat.data[3].z)),
            Dot(vec, Vec4F(mat.data[0].w, mat.data[1].w, mat.data[2].w, mat.data[3].w)),
        };
    }

    //inline auto operator*=(Vec4F &vec, const Mat4x4 &mat) -> Vec4F & {
    //    return vec = vec * mat;
    //}

    inline auto MatrixLookAtLH(Vec3F position, Vec3F target, Vec3F up) noexcept -> Mat4x4 {
        const auto zAxis = Normalize(target - position);
        const auto xAxis = Normalize(Cross(up, zAxis));
        const auto yAxis = Cross(zAxis, xAxis);

        return {
            xAxis.x,                yAxis.x,               zAxis.x,              0.0f,
            xAxis.y,                yAxis.y,               zAxis.y,              0.0f,
            xAxis.z,                yAxis.z,               zAxis.z,              0.0f,
           -Dot(xAxis, position),  -Dot(yAxis, position), -Dot(zAxis, position), 1.0f
        };
    }

    inline auto MatrixOrthographicLH(float width, float height, float nearZ, float farZ) noexcept -> Mat4x4 {
        return {
            2.0f / width, 0.0f,           0.0f,                            0.0f,
            0.0f,         2.0f / height,  0.0f,                            0.0f,
            0.0f,         0.0f,           1.0f / (farZ - nearZ),           0.0f,
            0.0f,         0.0f,            nearZ / (nearZ - farZ),          1.0f
        };
    }

    //inline auto OrthographicOffcenterLH(float width, float height, float nearZ, float farZ) noexcept -> Mat4x4 {
    inline auto MatrixOrthographicOffCenterLH(float l, float r, float b, float t, float nearZ, float farZ) noexcept -> Mat4x4 {
        return {
            2.0f / (r - l),   0.0f,            0.0f,                  0.0f,
            0.0f,             2.0f / (t - b),  0.0f,                  0.0f,
            0.0f,             0.0f,            1.0f / (farZ - nearZ), 0.0f,
           (l + r) / (l - r), (t + b) / (b - t),            nearZ / (nearZ - farZ),          1.0f
        };
    }

    inline auto MatrixPerspectiveFovLH(float fov, float aspectRatio, float nearZ, float farZ) noexcept -> Mat4x4 {
        const auto scale = 1.0f / std::tanf((fov / 2.0f) * (std::numbers::pi / 180.0f));

        return {
            scale, 0.0f,  0.0f, 0.0f,
            0.0f,  scale, 0.0f,                            0.0f,
            0.0f,  0.0f, -farZ / (farZ - nearZ),          -1.0f,
            0.0f,  0.0f, -(farZ * nearZ) / (farZ - nearZ), 0.0f
        };
    }

    inline auto MatrixTranslation(float x, float y, float z) -> Mat4x4 {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            x,    y,    z,    1.0f
        };
    }

    inline auto MatrixRotationAxis(const Vec3F &axis, float angle) -> Mat3x3 {
        const auto cos = std::cosf(angle);
        const auto sin = std::sinf(angle);

        return {
            cos + Square(axis.x) * (1.0f - cos), axis.x * axis.y * (1 - cos) - axis.z * sin, axis.x * axis.z * (1 - cos) - axis.y * sin,
            axis.y * axis.x * (1.0f - cos) + axis.z * sin, cos + Square(axis.y) * (1.0f - cos), axis.y * axis.x * (1.0f - cos) - axis.x * cos,
            axis.z * axis.x * (1.0f - cos) - axis.y * sin, axis.z * axis.y * (1.0f - cos) + axis.x * sin, cos + Square(axis.z) * (1.0f - cos)
        };
    }

    inline auto MatrixScale(float x, float y, float z) -> Mat4x4 {
        return {
            x,    0.0f, 0.0f, 0.0f,
            0.0f, y,    0.0f, 0.0f,
            0.0f, 0.0f, z,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
}

#endif