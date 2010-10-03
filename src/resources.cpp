
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
    sf::RenderWindow window;
    sf::Clock clock;
    
    std::string imgDir = "images/";
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
    
    struct FontDesc {
        std::string name;
        int size;
        FontDesc(std::string n, int s): name(n), size(s) {}
    };
    // Required for use in std::map
    bool operator< (const FontDesc& c1, const FontDesc& c2) {
        if (c1.name < c2.name || (c1.name == c2.name && c1.size < c2.size)) {
            return true;
        }
        return false;
    }
    std::string fntDir = "fonts/";
    std::map<FontDesc, sf::Font> fonts;
    
    sf::Font& fnt(std::string name, int size, std::string extension) {
        FontDesc desc(name, size);
        if (fonts.find(desc) == fonts.end()) {
            fonts[desc] = sf::Font();
            if (!fonts[desc].LoadFromFile(resDir + fntDir + name + extension, size)) {
                // ERROR MESSAGE
            }
        }
        return fonts[desc];
    }
}
