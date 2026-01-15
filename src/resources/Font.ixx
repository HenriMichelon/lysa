/*
 * Copyright (c) 2024-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <hb.h>
#include <hb-ft.h>
#include FT_FREETYPE_H
export module lysa.resources.font;

import lysa.context;
import lysa.math;
import lysa.resources;
import lysa.resources.image;

export namespace lysa {

    /**
     * Parameters controlling font rendering and outline.
     * @details These parameters influence SDF/MSDF sampling and outline appearance when rendering text.
     */
    struct FontParams {
        /** Pixel range used by the SDF/MSDF in the atlas (x = lower, y = upper). */
        float2 pxRange{FLOAT2ZERO};
        /** RGBA outline color. */
        float4 outlineColor{0.0f, 0.0f, 0.0f, 1.0f};
        /** Threshold used to classify inside/outside for the distance field. */
        float threshold{0.5f};
        /** Bias applied to the outline distance. */
        float outlineBias{1.0f/4.0f};
        /** Absolute outline width in pixels. */
        float outlineWidthAbsolute{1.0f/16.0f};
        /** Relative outline width with respect to font size. */
        float outlineWidthRelative{1.0f/50.0f};
        /** Outline blur factor for soft edges. */
        float outlineBlur{0.1f};
        /** Gamma correction applied during sampling. */
        float gamma{1.0f};
    };

    /**
     * Font resource used to render text.
     * @details A font is defined by a font file and a size. Glyphs are packed into an atlas
     *          and common metrics (ascender, descender, line height) are exposed for text layout.
     */
    class Font : public UnmanagedResource {
    public:
        /**
         * Bounds of a glyph in the font plane (em units relative to font size).
         */
        struct GlyphBounds {
            float left{0.0f};
            float bottom{0.0f};
            float right{0.0f};
            float top{0.0f};
        };

        /**
         * Information for a single glyph packed in the atlas.
         * @param index Glyph index in the font (HB/FreeType index).
         * @param advance Advance width in pixels for the current font size.
         * @param planeBounds Bounds in font plane units.
         * @param uv0 Top-left texture coordinate in the atlas.
         * @param uv1 Bottom-right texture coordinate in the atlas.
         */
        struct GlyphInfo {
            uint32 index{0};
            float advance{0.0f};
            GlyphBounds planeBounds{};
            float2 uv0{0.0f};
            float2 uv1{0.0f};
        };

        /**
         * Construct a font resource from a font file.
         * @param ctx Application context used to access resources and GPU.
         * @param path Font file path, relative to the application working directory.
         */
        Font(const Context& ctx, const std::string &path);

        Font(const Font &font);

        ~Font() override;

        /**
         * Compute the size in pixels for a UTF-8 string.
         * @param text Input text.
         * @param fontScale Scale factor applied to the base font size (1.0 = atlas size).
         * @param width Output total width in pixels.
         * @param height Output total height in pixels.
         */
        void getSize(const std::string &text, float fontScale, float &width, float &height);

        /**
         * Compute the size in pixels for a UTF-8 string.
         * @param text Input text.
         * @param fontScale Scale factor applied to the base font size (1.0 = atlas size).
         * @return Width (x) and height (y) in pixels.
         */
        float2 getSize(const std::string &text, float fontScale);

        float getWidth(char c, float fontScale);

        /**
         * Get the font size used to build the atlas (in pixels).
         * @return Atlas font size in pixels.
         */
        uint32 getFontSize() const { return size; }

        /** Line height relative to the font size. */
        float getLineHeight() const { return lineHeight; }

        /** Ascender in pixels (above the baseline). */
        float getAscender() const { return ascender; }

        /** Descender in pixels (below the baseline). */
        float getDescender() const { return descender; }

        /**
         * Retrieve glyph information by glyph index.
         * @param index Glyph index as returned by the shaper.
         */
        const GlyphInfo& getGlyphInfo(uint32 index) const;

        /** Get the underlying glyph atlas image. */
        const Image& getAtlas() const { return atlas; }

        /** Get current font rendering parameters. */
        const FontParams& getFontParams() const { return params; }

        /** Set the outline color. */
        void setOutlineColor(const float4 &color) {
            params.outlineColor = color;
        }

        /** Set the outline bias. */
        void setOutlineBias(const float bias) {
            params.outlineBias = bias;
        }

        /** Set the absolute outline width (in pixels). */
        void setOutlineWidthAbsolute(const float width) {
            params.outlineWidthAbsolute = width;
        }

        /** Set the relative outline width (scaled by font size). */
        void setOutlineWidthRelative(const float width) {
            params.outlineWidthRelative = width;
        }

        /** Set the outline blur factor. */
        void setOutlineBlur(const float blur) {
            params.outlineBlur = blur;
        }

        /** Set the SDF/MSDF threshold. */
        void setOutlineThreshold(const float threshold) {
            params.threshold = threshold;
        }

        /** Access the underlying HarfBuzz font handle. */
        auto getHarfBuzzFont() const { return hbFont; }

    private:
        const Context& ctx;
        const std::string path;
        const Image& atlas;
        uint32 size;
        float ascender;
        float descender;
        float lineHeight;
        FontParams params;
        std::unordered_map<uint32, GlyphInfo> glyphs;

        static FT_Library ftLibrary;
        FT_Face ftFace{nullptr};
        hb_font_t* hbFont{nullptr};
    };

}
