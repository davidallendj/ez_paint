#ifndef SP_HASH_EXPORTER_HPP
#define SP_HASH_EXPORTER_HPP

#include <png.h>
#include <string>

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

namespace sp_hash
{
    class ImageExporter
    {
    public:


    private:
        sf::Vector2u size;
        png_byte colorType;
        png_byte bitDepth;
    };

    class PngExporter : public ImageExporter
    {
    public:
        void readPngFile(const sf::String& filename);
        void writeToFile(const sf::String& filename);
    
    private:
        void processFile();

    };
}
#endif // SP_HASH_EXPORTER_HPP