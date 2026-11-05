#pragma once

#include <cstdint>
#include <string>

void write_noise_dds(const std::string& path, std::uint32_t width,
                     std::uint32_t height, std::uint32_t frame);
