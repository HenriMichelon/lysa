/*
 * Copyright (c) 2024-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#include <json.hpp>
#include <hb.h>
#include <hb-ft.h>
#include <ft2build.h>
#include FT_FREETYPE_H
module lysa.resources.font;

import vireo;
import lysa.exception;
import lysa.log;

namespace lysa {

    FT_Library Font::ftLibrary{nullptr};

    float2 Font::getSize(const std::string &text, const float fontScale) {
        float w, h;
        getSize(text, fontScale, w, h);
        return {w, h};
    }

    void Font::getSize(const std::string &text, const float fontScale, float &width, float &height) {
        const auto scale = fontScale * size;
        width = 0.0f;
        height = 0.0f;

        hb_buffer_t* hb_buffer = hb_buffer_create();
        hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
        hb_buffer_guess_segment_properties(hb_buffer);
        hb_shape(hbFont, hb_buffer, nullptr, 0);

        unsigned int glyph_count = 0;
        const hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(hb_buffer, &glyph_count);

        float maxTop = -std::numeric_limits<float>::infinity();
        float minBottom = std::numeric_limits<float>::infinity();
        for (unsigned int i = 0; i < glyph_count; i++) {
            const auto gi = glyphs[glyph_info[i].codepoint];
            width += gi.advance * scale;
            // Track actual top and bottom bounds among all glyphs to compute tight text height
            if (gi.planeBounds.right != 0.0f || gi.planeBounds.left != 0.0f ||
                gi.planeBounds.top != 0.0f || gi.planeBounds.bottom != 0.0f) {
                if (gi.planeBounds.top > maxTop) maxTop = gi.planeBounds.top;
                if (gi.planeBounds.bottom < minBottom) minBottom = gi.planeBounds.bottom;
            }
        }
        // If we found valid bounds, use them; otherwise fall back to line height
        if (glyph_count > 0 && maxTop > -std::numeric_limits<float>::infinity() &&
            minBottom < std::numeric_limits<float>::infinity() && maxTop > minBottom) {
            height = (maxTop - minBottom) * scale;
        } else if (glyph_count > 0) {
            height = fontScale * lineHeight;
        }

        hb_buffer_destroy(hb_buffer);
    }

    Font::Font(const Font &font):
        ctx(font.ctx),
        path{font.path},
        atlas{font.atlas},
        size{font.size},
        ascender{font.ascender},
        descender{font.descender},
        lineHeight{font.lineHeight},
        params{font.params},
        glyphs{font.glyphs}  {
        if (FT_New_Face(ftLibrary, ctx.fs.getPath(path + ".ttf").c_str(), 0, &ftFace)) {
            if (FT_New_Face(ftLibrary, ctx.fs.getPath(path + ".otf").c_str(), 0, &ftFace)) {
                throw Exception("Error loading font ", path);
            }
        }
        FT_Set_Char_Size(ftFace, 0, size * 64, 0, 0);
        hbFont = hb_ft_font_create(ftFace, nullptr);
    }

    Font::Font(const Context& ctx, const std::string &path):
        ctx(ctx),
        path(path),
        atlas(ctx.res.get<ImageManager>().load(path + ".png", vireo::ImageFormat::R8G8B8A8_SRGB)) {
        if (!ftLibrary) {
            if (FT_Init_FreeType(&ftLibrary)) {
                throw Exception("Error initializing FreeType");
            }
        }

        auto json = nlohmann::ordered_json::parse(ctx.fs.openReadStream(path + ".json"));
        const auto& atlas = json["atlas"];
        // assert([&]{ return atlas["type"].get<std::string>() == "mtsdf"; }, "Only MTSDF font atlas are supported");
        atlas["size"].get_to(size);
        uint32 atlasWidth, atlasHeight;
        atlas["width"].get_to(atlasWidth);
        atlas["height"].get_to(atlasHeight);
        const auto pixelRange = atlas["distanceRange"].get<float>();
        params.pxRange = { pixelRange / atlasWidth, pixelRange / atlasHeight };

        if (FT_New_Face(ftLibrary, ctx.fs.getPath(path + ".ttf").c_str(), 0, &ftFace)) {
            if (FT_New_Face(ftLibrary, ctx.fs.getPath(path + ".otf").c_str(), 0, &ftFace)) {
                throw Exception("Error loading font ", path);
            }
        }
        FT_Set_Char_Size(ftFace, 0, size * 64, 0, 0);
        hbFont = hb_ft_font_create(ftFace, nullptr);

        const auto& metrics = json["metrics"];
        lineHeight = metrics["lineHeight"].get<float>() * size;
        ascender = metrics["ascender"].get<float>() * size;
        descender = metrics["descender"].get<float>() * size;

        for (const auto& glyph : json["glyphs"]) {
            auto glyphInfo = GlyphInfo {
                .index = glyph["index"].get<uint32>(),
                .advance = glyph["advance"].get<float>(),
            };
            if (glyph.contains("planeBounds") && glyph.contains("atlasBounds")) {
                glyphInfo.planeBounds.left = glyph["planeBounds"]["left"].get<float>();
                glyphInfo.planeBounds.right = glyph["planeBounds"]["right"].get<float>();
                glyphInfo.planeBounds.top = glyph["planeBounds"]["top"].get<float>();
                glyphInfo.planeBounds.bottom = glyph["planeBounds"]["bottom"].get<float>();

                const auto atlasLeft = glyph["atlasBounds"]["left"].get<float>();
                const auto atlasRight = glyph["atlasBounds"]["right"].get<float>();
                const auto atlasTop = glyph["atlasBounds"]["top"].get<float>();
                const auto atlasBottom = glyph["atlasBounds"]["bottom"].get<float>();
                glyphInfo.uv0 = { atlasLeft / atlasWidth, atlasTop / atlasHeight };
                glyphInfo.uv1 = { atlasRight / atlasWidth, atlasBottom / atlasHeight };
            }
            glyphs[glyphInfo.index] = glyphInfo;
        }
        Log::info("Loaded ", glyphs.size(), " glyphs from ", path);
    }

    const Font::GlyphInfo& Font::getGlyphInfo(const uint32 index) const {
        if (!glyphs.contains(index)) {
            return glyphs.at(0);
        }
        return glyphs.at(index);
    }

    Font::~Font() {
        hb_font_destroy(hbFont);
        FT_Done_Face(ftFace);
    }
}
