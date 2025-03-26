#ifndef LIGHTNING3D_VECTOR_HH_
#define LIGHTNING3D_VECTOR_HH_

#include <array>
#include <cmath>
#include <cstdlib>

template <typename T, size_t N>
struct VectorStorage;

template <typename T>
struct VectorStorage<T, 1u> {
    T x;
};

template <typename T>
struct VectorStorage<T, 2u> {
    T x;
    T y;
};

template <typename T>
struct VectorStorage<T, 3u> {
    T x;
    T y;
    T z;
};

template <typename T>
struct VectorStorage<T, 4u> {
    T x;
    T y;
    T z;
    T w;
};

template <typename T, size_t N>
struct Vector : VectorStorage<T, N> {
    constexpr Vector()
        : VectorStorage<T, N>{ } {
    }

    template <typename ...U>
    constexpr Vector(U ...values)
        : VectorStorage<T, N>{ static_cast<T>(values)... } {
    }

    constexpr Vector(const Vector &other)
        : VectorStorage<T, N>{ other } {

    }
};

using Vec2F = Vector<float, 2u>;
using Vec3F = Vector<float, 3u>;
using Vec4F = Vector<float, 4u>;

using Vec2I = Vector<int, 2u>;
using Vec3I = Vector<int, 3u>;
using Vec4I = Vector<int, 4u>;

template<typename T, typename U>
inline auto operator+(const T &lhs, const U &rhs) -> std::enable_if_t<std::is_base_of_v<Vec3F, T>, T> {
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    };
}

template<typename T, typename U>
inline auto operator+=(T &lhs, const U &rhs) -> std::enable_if_t<std::is_base_of_v<Vec3F, T>, T &> {
    lhs = lhs + rhs;

    return lhs;
}

inline auto operator-(const Vec3F &lhs, const Vec3F &rhs) -> Vec3F {
    return {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    };
}

inline auto operator==(const Vec2I &lhs, const Vec2I &rhs) -> bool {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
constexpr auto Square(const T &value) -> T {
    return value * value;
}

inline auto Magnitude(const Vec3F &vec) -> float {
    return std::sqrtf(Square(vec.x) + Square(vec.y) + Square(vec.z));
}

inline auto Normalize(const Vec3F &vec) -> Vec3F {
    const auto m = Magnitude(vec);

    return {
        vec.x / m,
        vec.y / m,
        vec.z / m
    };
}

inline auto Cross(const Vec3F &lhs, const Vec3F &rhs) -> Vec3F {
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

inline auto Dot(const Vec3F &lhs, const Vec3F &rhs) -> float {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline auto Dot(const Vec4F &lhs, const Vec4F &rhs) -> float {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

#endif