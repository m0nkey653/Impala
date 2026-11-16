#include "renderer.h"
#include "assets/asset_database.h"
#include "impala/math.h"
#include "impala/sprite.h"
#include "impala/transform.h"
#include "impala_scene.h"
#include "texture.h"

namespace impala {

    void Renderer::DrawLine(impala::math::Vec2 start, impala::math::Vec2 end, impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderLine(_renderer.get(), start.x, start.y, end.x, end.y);
    }

    void Renderer::DrawCircle(impala::math::Vec2 position, float radius, impala::Color color,
                              float startAngle, float endAngle) {
        const int segments = 60;
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);

        SDL_FRect rects[segments];
        math::Vec2 point{0, -1};
        math::Vec2 prev = point;
        for (int i = 1; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * (i / static_cast<float>(segments));
            impala::math::Vec2 current = math::RotateDegrees(point, angle);
            rects[i - 1] = {position.x + current.x * radius - 1,
                            position.y + current.y * radius - 1, 2, 2};
            prev = current;
        }
        SDL_RenderFillRects(_renderer.get(), rects, segments);
    }

    void Renderer::DrawRectangle(impala::math::Vec2 topLeft, float width, float height,
                                 impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_FRect rect = {topLeft.x, topLeft.y, width, height};
        SDL_RenderRect(_renderer.get(), &rect);
    }

    void Renderer::FillRectangle(impala::math::Vec2 topLeft, float width, float height,
                                 impala::Color color) {
        SDL_SetRenderDrawColorFloat(_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_FRect rect = {topLeft.x, topLeft.y, width, height};
        SDL_RenderFillRect(_renderer.get(), &rect);
    }

    void Renderer::RenderFrame(impala::ImpalaScene* scene, impala::AssetDatabase* assetDatabase,
                               float alpha) {
        SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(_renderer.get());

        auto entities =
            scene->GetEntitiesWithComponents<impala::SpriteRenderer, impala::Transform2d>();
        for (const auto& entity : entities) {
            auto r = scene->GetComponent<impala::SpriteRenderer>(
                impala::EntityHandle{entity.id, entity.generation});
            auto t = scene->GetComponent<impala::Transform2d>(
                impala::EntityHandle{entity.id, entity.generation});

            auto sprite = assetDatabase->GetSprite(r->sprite);
            auto texture = assetDatabase->GetTexture(sprite.textureHandle);

            SDL_FRect srcRect = sprite.rect.ToSDLFRect();
            auto anchor = r->anchor.value_or(sprite.anchor);
            SDL_FPoint pivot = {(1 + anchor.x) * sprite.rect.width * 0.5f,
                                (1 + anchor.y) * sprite.rect.height * 0.5f};
            impala::math::Vec2 topLeft = t->position - impala::math::Vec2{pivot.x, pivot.y};
            SDL_FRect dstRect = {topLeft.x, topLeft.y, srcRect.w, srcRect.h};

            bool result = false;
            if (t->rotation != 0) {
                result = SDL_RenderTextureRotated(_renderer.get(), texture->GetTexture(), &srcRect,
                                                  &dstRect, t->rotation, &pivot, SDL_FLIP_NONE);
            } else {
                result =
                    SDL_RenderTexture(_renderer.get(), texture->GetTexture(), &srcRect, &dstRect);
            }
            if (!result) {
                impala::log::Error(impala::log::LogCategory::Engine, "Failed to render texture: {}",
                                   SDL_GetError());
            }

            float px = std::round(t->position.x);
            float py = std::round(t->position.y);
            FillRectangle({px - 2.0f, py - 2.0f}, 4.0f, 4.0f, impala::color::Red());
            DrawRectangle({px - 4.0f, py - 4.0f}, 8.0f, 8.0f, impala::color::White());
            DrawCircle(t->position, 20.0f, impala::color::Red());
        }
    }

} // namespace impala
