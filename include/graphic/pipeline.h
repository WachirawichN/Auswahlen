#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace auswahlen
{
    namespace graphic
    {
        class pipeline
        {
            private:
                /*------------------------------------------------------------
                    Pipeline variables.
                ------------------------------------------------------------*/
                std::vector<char> vertexCode;
                std::vector<char> fragmentCode;

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                static std::vector<char> readFile(const std::string& path);
            public:
                pipeline() {}
                pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

                pipeline(const pipeline&) = delete;
                pipeline& operator=(const pipeline& pipeline);

                void init();
                void cleanUp();
        };
    }
}