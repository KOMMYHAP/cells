#include "world_widget.h"

WorldWidget::WorldWidget(Config&& config)
    : _config(std::move(config))
    , _vertexBuffer(sf::PrimitiveType::TrianglesStrip, sf::VertexBuffer::Static)
{
    // const sf::Vector2u textureSize = _config.renderSystem->GetTextureSize();
    // const bool textureCreated = _texture.create(textureSize.x, textureSize.y);
    // ASSERT(textureCreated);
    //
    // // We need shader only for custom texture now
    // _config.fragmentShader->setUniform("texture", _texture);
    //
    // ASSERT(_vertexBuffer.isAvailable());
    // std::array vertices {
    //     sf::Vertex { { _config.renderTargetSize.x - 1.0f, 0.0f }, { 1.0f, 0.0f } },
    //     sf::Vertex { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
    //     sf::Vertex { { _config.renderTargetSize.x - 1.0f, _config.renderTargetSize.y - 1.0f }, { 1.0f, 1.0f } },
    //     sf::Vertex { { 0.0f, _config.renderTargetSize.y - 1.0f }, { 0.0f, 1.0f } },
    //
    // };
    // const bool vertexBufferCreated = _vertexBuffer.create(vertices.size());
    // ASSERT(vertexBufferCreated);
    //
    // const bool vertexBufferUpdated = _vertexBuffer.update(vertices.data());
    // ASSERT(vertexBufferUpdated);
}

void WorldWidget::Draw(sf::RenderTarget& target)
{
    // _texture.update(reinterpret_cast<const uint8_t*>(_config.renderSystem->GetTextureData().data()));

    sf::RenderStates states;
    states.shader = _config.fragmentShader.get();
    states.transform.translate(NarrowCast<float>(_config.renderTargetOffset.x), NarrowCast<float>(_config.renderTargetOffset.y));
    target.draw(_vertexBuffer, states);
}

void WorldWidget::Update(sf::Time elapsedTime)
{
    // _config.renderSystem->DoSystemUpdate();
}
