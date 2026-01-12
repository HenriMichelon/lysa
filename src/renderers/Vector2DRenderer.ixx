/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.renderers.vector_2d;

import vireo;
import lysa.context;
import lysa.math;
import lysa.rect;
import lysa.renderers.configuration;
import lysa.renderers.vector_3d;
import lysa.resources.font;
import lysa.resources.image;

export namespace lysa {

    constexpr float VECTOR_2D_SCREEN_SIZE{1000.0f};

    class Vector2DRenderer : public Vector3DRenderer {
    public:
        Vector2DRenderer(
            const Context& ctx,
            const RendererConfiguration& config);

        void resize(const vireo::Extent& extent);

        auto getAspectRatio() const { return vectorRatio; }

        // auto getExtent() const { return vectorExtent; }

        // Draw a 1-fragment width line
        void drawLine(const float2& start, const float2& end);

        // Draw a filled rectangle
        void drawFilledRect(const Rect &rect);

        // Draw a filled rectangle with an image
        void drawFilledRect(
            const Rect &rect,
            unique_id texture);

        // Draw a filled rectangle
        void drawFilledRect(
            float x, float y,
            float w, float h,
            unique_id = INVALID_ID);

        void drawText(
            const std::string& text,
            Font& font,
            float fontScale,
            float x, float y);

        // Change the color of the fragments for the next drawing commands
        auto setPenColor(const float4& color) { penColor = color; }

        // Change the [x,y] translation for the next drawing commands
        auto setTranslate(const float2& t) { translate = t; }

        // Change the global transparency for the next drawing commands. Value is subtracted from the vertex alpha
        auto setTransparency(const float a) { transparency = a; }

    private:
        struct PushConstants {
            int   textureIndex;
        };

        ImageManager& imageManager;

        // Fragment color for the next drawing commands
        float4 penColor{1.0f, 1.0f, 1.0f, 1.0f};
        // [x,y] translation for the next drawing commands
        float2 translate{0.0f, 0.0f};
        // Global transparency for the next drawing commands. Value is subtracted from the vertex alpha
        float transparency{0.0f};

        // float2 vectorExtent{};
        float vectorRatio{};
    };
}