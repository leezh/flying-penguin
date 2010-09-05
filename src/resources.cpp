
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
using namespace std;

#include "config.h"
#ifdef RESOURCE_DIR
    const std::string resDir = RESOURCE_DIR;
#else
	const std::string resDir = "";
#endif

namespace res {
    std::string imgDir = "images/";
    sf::RenderWindow window;
    sf::Clock clock;
    
    std::map<string, sf::Image> images;
    
    sf::Image& img(std::string name, std::string extension) {
        if (images.find(name) == images.end()) {
            images[name] = sf::Image();
            if (!images[name].LoadFromFile(resDir + imgDir + name + extension)) {
                // ERROR MESSAGE
            }
        }
        return images[name];
    }
}
