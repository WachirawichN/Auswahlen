#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace auswahlen
{
    class pipeline
    {
        private:
            static std::vector<char> readFile(const std::string& path);
        public:
            pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    };
}