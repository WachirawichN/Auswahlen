#pragma once

#include <graphic/window.h>
#include <graphic/pipeline.h>

namespace auswahlen
{
    class simulation
    {
        private:
            window window{width, height, "Auswahlen"};
            pipeline pipeline{"asset/shader/shader.vert.spv", "asset/shader/shader.frag.spv"};
        public:
            static constexpr int width = 800;
            static constexpr int height = 600;
            void run();
    };
}
