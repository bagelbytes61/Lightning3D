#ifndef LIGHTNING3D_CAMERA_HH_
#define LIGHTNING3D_CAMERA_HH_

#include "Matrix.hh"

#include <d3d12.h>

#include <cassert>
#include <optional>

namespace Lightning3D {
    class Camera {
        static constexpr auto DEFAULT_FOV = 45.0f;
        static constexpr auto DEFAULT_CLIP_PLANES = Vec2F(1.0f, 1000.0f);

        static constexpr auto DEFAULT_UP_VECTOR = Vec3F(0.0f, 1.0f, 0.0f);

    public:
        enum struct ProjectionMode {
            Orthographic,
            Perspective
        };

    public:
        Camera(const Vec3F &position, const Vec3F &target, const Vec3F &up, float aspectRatio)
            : Camera(position, target, up, DEFAULT_FOV, aspectRatio) {
        
            SetProjectionMode(ProjectionMode::Perspective);
        }

        Camera(const Vec2F &position, float aspectRatio, float orthoWidth)
            : Camera(Vec3F(position.x, position.y, -1.0f), Vec3F(position.x, position.y, 0.0f), DEFAULT_UP_VECTOR, DEFAULT_FOV, aspectRatio, orthoWidth) {

            SetProjectionMode(ProjectionMode::Orthographic);
        }

        Camera(const Vec3F &position, const Vec3F &target, const Vec3F &up, float fov, float aspectRatio, std::optional<float> orthoWidth = std::nullopt)
            : m_position(position)
            , m_target(target)
            , m_up(up)
            , m_fov(fov)
            , m_aspectRatio(aspectRatio)
            , m_orthoWidth(orthoWidth) {

        }

        auto SetFoV(float fov) -> void {
            m_fov = fov;
        }

        auto SetAspectRatio(float ar) -> void {
            m_aspectRatio = ar;
        }

        auto SetNearZ(float z) -> void {
            m_clipPlanes.x = z;
        }

        auto SetFarZ(float z) -> void {
            m_clipPlanes.y = z;
        }

        auto SetOrthographicWidth(float width) -> void {
            m_orthoWidth = width;
        }

        auto SetProjectionMode(ProjectionMode mode) -> void {
            m_projectionMode = mode;
        }

        auto GetProjectionMatrix(void) const -> Mat4x4 {
            if (m_projectionMode == ProjectionMode::Orthographic) {
                assert(m_orthoWidth.has_value());

                return MatrixOrthographicOffCenterLH(0.0f, m_orthoWidth.value(), m_orthoWidth.value() / m_aspectRatio, 0.0f, m_clipPlanes.x, m_clipPlanes.y);
            } else {
                return MatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_clipPlanes.x, m_clipPlanes.y);
            }
        }

        auto GetViewMatrix(void) const -> Mat4x4 {
            return MatrixLookAtLH(m_position, m_target, m_up);
        }

    private:
        Vec3F m_position;
        Vec3F m_target;
        Vec3F m_up;

        float m_fov;
        float m_aspectRatio;

        Vec2F m_clipPlanes = DEFAULT_CLIP_PLANES;

        std::optional<float> m_orthoWidth;

        ProjectionMode m_projectionMode;
    };

    class Viewport {
    public:
        Viewport(float x, float y, float width, float height, float minDepth, float maxDepth)
            : m_viewport({ .TopLeftX = x, .TopLeftY = y, .Width = width, .Height = height, .MinDepth = minDepth, .MaxDepth = maxDepth }) {

        }

        auto PreRender(ID3D12GraphicsCommandList *graphicsCommandList) -> void {
            graphicsCommandList->RSSetViewports(1u, &m_viewport);

            // Probably move this somewhere else eventually?
            D3D12_RECT m_scissorRect = {};
            m_scissorRect.right = 1024.0f;
            m_scissorRect.bottom = 768.0f;
            graphicsCommandList->RSSetScissorRects(1U, &m_scissorRect);
        }

        auto SetCamera(Camera *camera) -> void {
            m_camera = camera;
        }

        auto Get(void) const -> const D3D12_VIEWPORT & {
            return m_viewport;
        }

        auto GetCamera(void) const -> const Camera * {
            return m_camera;
        }

    private:
        D3D12_VIEWPORT m_viewport;

        Camera *m_camera = nullptr;
    };
} // namespace Lightning3D

#endif