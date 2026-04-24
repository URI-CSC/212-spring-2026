#ifndef SVG_H
#define SVG_H

// ============================================================
// Assignment 07 - Part 2: SVG helpers
// svg.h  --  provided header-only, do not modify
// ============================================================

#include <ostream>
#include <string>

// Returns a hex color string (e.g. "#e6194b") for category c.
// The same category always maps to the same color.
inline std::string category_color(int c) {
    static const char* PALETTE[] = {
        "#e6194b", "#3cb44b", "#ffe119", "#4363d8", "#f58231",
        "#911eb4", "#42d4f4", "#f032e6", "#bfef45", "#fabed4",
        "#469990", "#dcbeff", "#9a6324", "#800000", "#aaffc3",
        "#808000", "#ffd8b1", "#000075", "#a9a9a9", "#ffffff"
    };
    static const int N = 20;
    return PALETTE[((c % N) + N) % N];
}

// Writes the SVG opening tag with the given viewport dimensions.
inline void svg_begin(std::ostream& out, float width, float height) {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<svg width=\"" << width << "\" height=\"" << height
        << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
}

// Draws a rectangle.
inline void svg_rect(std::ostream& out,
                     float x, float y, float w, float h,
                     const std::string& fill,
                     const std::string& stroke,
                     float stroke_width,
                     float opacity) {
    out << "  <rect"
        << " x=\""            << x            << "\""
        << " y=\""            << y            << "\""
        << " width=\""        << w            << "\""
        << " height=\""       << h            << "\""
        << " fill=\""         << fill         << "\""
        << " stroke=\""       << stroke       << "\""
        << " stroke-width=\"" << stroke_width << "\""
        << " opacity=\""      << opacity      << "\"/>\n";
}

// Draws a text label at (x, y).
inline void svg_text(std::ostream& out,
                     float x, float y,
                     const std::string& label,
                     const std::string& fill,
                     float font_size) {
    out << "  <text"
        << " x=\""           << x         << "\""
        << " y=\""           << y         << "\""
        << " fill=\""        << fill      << "\""
        << " font-size=\""   << font_size << "\""
        << " font-family=\"monospace\">"
        << label << "</text>\n";
}

// Embeds a raster image (JPEG/PNG) at (x, y) with given dimensions.
inline void svg_image(std::ostream& out,
                      float x, float y, float w, float h,
                      const std::string& href) {
    out << "  <image"
        << " x=\""      << x    << "\""
        << " y=\""      << y    << "\""
        << " width=\""  << w    << "\""
        << " height=\"" << h    << "\""
        << " href=\""   << href << "\"/>\n";
}

// Writes the SVG closing tag.
inline void svg_end(std::ostream& out) {
    out << "</svg>\n";
}

#endif // SVG_H
