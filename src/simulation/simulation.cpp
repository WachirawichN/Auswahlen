#include <simulation/simulation.h>

namespace auswahlen
{
    void simulation::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
        }
    }
}