#include <graphic/pipeline.h>

namespace auswahlen
{
    namespace graphic
    {
        /*------------------------------------------------------------
            Helper functions.
        ------------------------------------------------------------*/
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
        
        /*------------------------------------------------------------
            Public functions.
        ------------------------------------------------------------*/
        pipeline::pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            vertexCode = readFile(vertexShaderPath);
            fragmentCode = readFile(fragmentShaderPath);
        }

        pipeline& pipeline::operator=(const pipeline& pipeline)
        {
            // Check if there are already vertex / fragment code assign to this variable yet, and check if it try to use "=" operator on itself.
            if (!(vertexCode.size() || fragmentCode.size()) && (pipeline.vertexCode.size() && pipeline.fragmentCode.size()) && this != &pipeline)
            {
                vertexCode = pipeline.vertexCode;
                fragmentCode = pipeline.fragmentCode;
            }
            return *this;
        }
    
        void pipeline::init()
        {
            std::cout << "Initializing graphic pipeline." << std::endl;
        }
        void pipeline::cleanUp()
        {
            std::cout << "Cleaning up graphic pipeline." << std::endl;
        }
    }
}