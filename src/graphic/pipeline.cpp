#include <graphic/pipeline.h>

namespace auswahlen
{
    std::vector<char> pipeline::readFile(const std::string& path)
    {
        // Read at the end of the file.
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file: " + path);
        }

        // The file is last position so tellg will just return the last position aka size.
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        // Find first position, then read up from there.
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
    pipeline::pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        auto vertexCode = readFile(vertexShaderPath);
        auto fragmentCode = readFile(fragmentShaderPath);

        std::cout << "Vertex shader size: " << vertexCode.size() << std::endl;
        std::cout << "Fragment shader size: " << fragmentCode.size() << std::endl;
    }
}