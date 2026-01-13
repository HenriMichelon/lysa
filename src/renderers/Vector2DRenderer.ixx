/*
 * @file Vector2DRenderer.ixx
 * @brief Header for the 2D vector renderer
 * @author Henri Michelon
 * @copyright Copyright (c) 2025-present Henri Michelon
 * @license This software is released under the MIT License. https://opensource.org/licenses/MIT
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

    /**
     * @brief The screen size for the 2D vector renderer
     */
    constexpr float VECTOR_2D_SCREEN_SIZE{1000.0f};

    /**
     * @brief Renderer for 2D vector graphics
     */
    class Vector2DRenderer : public Vector3DRenderer {
    public:
        /**
         * @brief Constructs a new Vector2DRenderer
         * @param ctx The engine context
         * @param config The renderer configuration
         */
        Vector2DRenderer(
            const Context& ctx,
            const RendererConfiguration& config);

        /**
         * @brief Resizes the renderer's viewport
         * @param extent The new extent
         */
        void resize(const vireo::Extent& extent);

        /**
         * @brief Gets the aspect ratio of the renderer
         * @return The current aspect ratio
         */
        auto getAspectRatio() const { return aspectRatio; }

        /**
         * @brief Draws a 1-fragment width line
         * @param start The starting point of the line
         * @param end The ending point of the line
         */
        void drawLine(const float2& start, const float2& end);

        /**
         * @brief Draws a filled rectangle
         * @param rect The rectangle to draw
         */
        void drawFilledRect(const Rect &rect);

        /**
         * @brief Draws a filled rectangle with an image texture
         * @param rect The rectangle to draw
         * @param texture The unique ID of the texture to use
         */
        void drawFilledRect(
            const Rect &rect,
            unique_id texture);

        /**
         * @brief Draws a filled rectangle with an optional texture
         * @param x The x-coordinate of the rectangle
         * @param y The y-coordinate of the rectangle
         * @param w The width of the rectangle
         * @param h The height of the rectangle
         * @param texture The unique ID of the texture to use (defaults to INVALID_ID)
         */
        void drawFilledRect(
            float x, float y,
            float w, float h,
            unique_id texture = INVALID_ID);

        /**
         * @brief Draws text on the screen
         * @param text The text string to draw
         * @param font The font to use for drawing
         * @param fontScale The scale of the font
         * @param x The x-coordinate where the text starts
         * @param y The y-coordinate where the text starts
         */
        void drawText(
            const std::string& text,
            Font& font,
            float fontScale,
            float x,
            float y);

        /**
         * @brief Changes the color of the fragments for the next drawing commands
         * @param color The new pen color
         */
        auto setPenColor(const float4& color) { penColor = color; }

        /**
         * @brief Changes the [x,y] translation for the next drawing commands
         * @param t The translation vector
         */
        auto setTranslate(const float2& t) { translate = t; }

        /**
         * @brief Changes the global transparency for the next drawing commands
         * @details The value is subtracted from the vertex alpha
         * @param a The transparency value
         */
        auto setTransparency(const float a) { transparency = a; }

    private:
        /*
         *  Push constants for the vector renderer
         */
        struct PushConstants {
            /* Index of the texture in the descriptor set */
            int   textureIndex;
        };

        ImageManager& imageManager;

        /*
         * Fragment color for the next drawing commands
         */
        float4 penColor{1.0f, 1.0f, 1.0f, 1.0f};

        /*
         * [x,y] translation for the next drawing commands
         */
        float2 translate{0.0f, 0.0f};

        /*
         * Global transparency for the next drawing commands
         * Value is subtracted from the vertex alpha
         */
        float transparency{0.0f};

        /* The aspect ratio of the viewport */
        float aspectRatio{};
    };
}