#ifndef LIGHTNING3D_VIEWPORT_HH_
#define LIGHTNING3D_VIEWPORT_HH_

#include "Camera.hh"

namespace Lightning3D {

    //class Viewport {
    //public:
    //    Viewport(const Vec2F &size, float fov, float nearZ, float farZ)
    //        : m_size(size)
    //        , m_aspectRatio(m_size.x / m_size.y)
    //        , m_fov(fov)
    //        , m_nearZ(nearZ)
    //        , m_farZ(farZ) {

    //    }

    //    auto SetCamera(Camera *camera) -> void {
    //        m_camera = camera;
    //    }

    //    auto GetProjectionMatrix(void) const -> Mat4x4 {
    //        return PerspectiveFovLH(m_fov, m_aspectRatio, m_nearZ, m_farZ);
    //    }

    //    auto GetSize(void) const -> const Vec2F & {
    //        return m_size;
    //    }

    //    auto GetNearClip(void) const -> float {
    //        return m_nearZ;
    //    }

    //    auto GetFarClip(void) const -> float {
    //        return m_farZ;
    //    }

    //    auto GetCamera(void) const -> const Camera * {
    //        return m_camera;
    //    }

    //private:
    //    Vec2F m_size;

    //    float m_aspectRatio;
    //    float m_fov;
    //    float m_nearZ;
    //    float m_farZ;

    //    Camera *m_camera{ nullptr };
    //};

} // namespace Lightning3D


#endif