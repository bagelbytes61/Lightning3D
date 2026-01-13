#include "Cell.hh"


#include "../../Components/Sprite.hh"
#include "../../D3D12Renderer.hh"
#include "../../Material.hh"
#include "../../PipelineState.hh"

using namespace Lightning3D::Games::Filler;

Cell::Cell(const Vec2I &location, float width, float height, Color initialColor, Material *material)
    : m_location(location)
    , m_initialColor(initialColor) {

    rect = AttachComponent(Rectangle(width, height, COLORS[static_cast<int>(initialColor)]));
    rect->material = material;

    rect->material->SetVec4FVariable("Color", COLORS[static_cast<int>(initialColor)]);

    auto transform = QueryComponent<Transform>();
    transform->position.x = m_location.x * width;
    transform->position.y = m_location.y * height;
}

auto Cell::Update(void) -> void {
    if (m_player != nullptr) {
        rect->material->SetVec4FVariable("Color", COLORS[static_cast<size_t>(m_player->GetColor())]);
    }
}

auto Cell::Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
    auto staticMesh = QueryComponent<StaticMesh>();
    const auto transform = QueryComponent<Transform>();

    const auto worldTransform = transform->LocalToWorld();

    const auto worldTransformMat = MatrixScale(worldTransform.scale.x, worldTransform.scale.y, 1.0f) * MatrixTranslation(worldTransform.position.x, worldTransform.position.y, 0.0f);

    renderer.Draw(staticMesh->vertices, worldTransformMat, staticMesh->material, pso.Get());
}
