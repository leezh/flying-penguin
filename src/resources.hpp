
#include <string>
#include <SFML/Graphics.hpp>
#define imgDefaultExt ".png"
#define fntDefaultExt ".ttf"
#define fntDefaultSize 20

namespace res {
    extern std::string imgDir; // = "images/"
    
    extern sf::RenderWindow window;
    extern sf::Clock clock;
    
    sf::Image& img(std::string name, std::string extension = imgDefaultExt);
    sf::Font& fnt(std::string name, int size = fntDefaultSize, std::string extension = fntDefaultExt);
}
