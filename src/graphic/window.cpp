#include <graphic/window.h>

namespace auswahlen
{
    namespace graphic
    {
        window::window(int width, int height, std::string name)
            : wWidth(width), wHeight(height), wName(name)
        {
        }
    
        window& window::operator=(const window& window)
        {
            // Check if the window for this class is already created, and check if it try to use "=" operator on itself.
            if (!(wWidth || wHeight) && (window.getWidth() && window.getHeight()) && (this != &window))
            {
                wWidth = window.getWidth();
                wHeight = window.getHeight();
                wName = window.getName();
            }
            return *this;
        }
    
        void window::createWindow()
        {
            std::cout << "Creating window." << std::endl;
            guiWindow = glfwCreateWindow(wWidth, wHeight, wName.c_str(), nullptr, nullptr);
            std::cout << "\t- Window creating completed." << std::endl;
        }
        void window::cleanUp()
        {
            std::cout << "Cleaning up window." << std::endl;
            glfwDestroyWindow(guiWindow);
        }
    }
}

