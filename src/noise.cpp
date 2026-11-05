#include "noise.h"

#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace {
// A deterministic digital shift for each pixel and dimension.
std::uint32_t hash(std::uint32_t x) {
    x ^= x >> 16; x *= 0x7feb352dU;
    x ^= x >> 15; x *= 0x846ca68bU;
    return x ^ (x >> 16);
}

// First two Sobol dimensions, evaluated from the Gray-coded index.
std::uint32_t sobol(std::uint32_t index, unsigned dimension) {
    std::uint32_t bits = index ^ (index >> 1), direction = 0x80000000U, value = 0;
    while (bits) {
        if (bits & 1U) value ^= direction;
        bits >>= 1;
        direction = dimension == 0 ? direction >> 1 : direction ^ (direction >> 1);
    }
    return value;
}

void write_u32(std::ostream& out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.put(static_cast<char>(value >> (i * 8)));
}
}

void write_noise_dds(const std::string& path, std::uint32_t width,
                     std::uint32_t height, std::uint32_t frame) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output file: " + path);
    out.write("DDS ", 4);
    write_u32(out, 124); write_u32(out, 0x100f);
    write_u32(out, height); write_u32(out, width); write_u32(out, width * 2);
    write_u32(out, 0); write_u32(out, 0);
    for (int i = 0; i < 11; ++i) write_u32(out, 0);
    write_u32(out, 32); write_u32(out, 4); write_u32(out, 0x30315844); // DX10
    for (int i = 0; i < 5; ++i) write_u32(out, 0);
    write_u32(out, 0x1000);
    for (int i = 0; i < 4; ++i) write_u32(out, 0);
    write_u32(out, 49); write_u32(out, 3); write_u32(out, 0);
    write_u32(out, 1); write_u32(out, 0); // R8G8_UNORM, 2D, one texture

    const std::uint32_t sample_x = sobol(frame, 0), sample_y = sobol(frame, 1);
    for (std::uint32_t y = 0; y < height; ++y) {
        for (std::uint32_t x = 0; x < width; ++x) {
            const std::uint32_t pixel = hash(x) ^ hash(y + 0x9e3779b9U);
            out.put(static_cast<char>((sample_x ^ hash(pixel)) >> 24));
            out.put(static_cast<char>((sample_y ^ hash(pixel ^ 0x68bc21ebU)) >> 24));
        }
    }
    out.close();
    if (!out) throw std::runtime_error("Failed to write output file: " + path);
}
