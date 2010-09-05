
#include <string>
#include <SFML/Graphics.hpp>
#define imgDefaultExt ".png"

namespace res {
    extern std::string imgDir; // = "images/"
    
    extern sf::RenderWindow window;
    extern sf::Clock clock;
    
    sf::Image& img(std::string name, std::string extension = imgDefaultExt);
}
