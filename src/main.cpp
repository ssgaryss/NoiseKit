#include "noise.h"

#include <charconv>
#include <cstdint>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::string_view(argv[1]) == "--help") {
            std::cout << "Usage: noisekit blue [--width N] [--height N] [--frame N] [--output FILE]\n";
            return 0;
        }
        if (argc < 2 || std::string_view(argv[1]) != "blue")
            throw std::runtime_error("Usage: noisekit blue [--width N] [--height N] [--frame N] [--output FILE]");
        std::uint32_t width = 128, height = 128, frame = 0;
        std::string output = "blue.dds";
        for (int i = 2; i < argc; ++i) {
            const std::string_view option = argv[i];
            if (option != "--width" && option != "--height" && option != "--frame" && option != "--output")
                throw std::runtime_error("Unknown option: " + std::string(option));
            if (++i == argc) throw std::runtime_error("Missing value for " + std::string(option));
            if (option == "--output") { output = argv[i]; continue; }
            const std::string_view value = argv[i];
            std::uint32_t n = 0;
            const auto result = std::from_chars(value.data(), value.data() + value.size(), n);
            if (value.empty() || result.ec != std::errc{} || result.ptr != value.data() + value.size())
                throw std::runtime_error("Invalid value for " + std::string(option));
            if (option == "--width") width = n;
            else if (option == "--height") height = n;
            else frame = n;
        }
        if (!width || !height || width > 16384 || height > 16384)
            throw std::runtime_error("Width and height must be between 1 and 16384");
        if (output.empty()) throw std::runtime_error("Output path is empty");
        write_noise_dds(output, width, height, frame);
        std::cout << "Wrote " << output << " (" << width << "x" << height << ")\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
