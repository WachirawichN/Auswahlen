#pragma once

#include <graphics/window.h>
#include <graphics/pipeline.h>

namespace auswahlen
{
    class simulation
    {
        private:
        public:
            simulation();
            ~simulation();
            void stepSimulation();
    };
}
