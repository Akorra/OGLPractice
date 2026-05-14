#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

std::string loadShaderSource(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}