
#include "utils.hpp"
#include "object.hpp"
#include "format.hpp"

#include <any>
#include <unordered_map>

namespace fu::utils
{
    sf::View calcView(const sf::Vector2f& windowSize, float minRatio, float maxRatio)
    {
        sf::Vector2f viewSize = windowSize;

        // clip ratio
        float ratio = viewSize.x / viewSize.y;
        if (ratio < minRatio) // too high
            viewSize.y = viewSize.x / minRatio;
        else if (ratio > maxRatio) // too wide
            viewSize.x = viewSize.y * maxRatio;

        sf::View view(sf::FloatRect(sf::Vector2f(), viewSize));

        sf::FloatRect viewport((windowSize - viewSize) / 2.f, viewSize);
        viewport.left /= windowSize.x;
        viewport.top /= windowSize.y;
        viewport.width /= windowSize.x;
        viewport.height /= windowSize.y;
        view.setViewport(viewport);

        return view;
    }


    // Credits to Haxapia
    void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, float zoom)
    {
        const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
        sf::View view{ window.getView() };
        view.zoom(zoom);
        window.setView(view);
        const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
        const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
        view.move(offsetCoords);
        window.setView(view);
    }

    // Declared in "Object.hpp"
    std::ostream& operator<<(std::ostream& os, const Object& o)
    { return os << o.getName(); }

    std::ostream& operator<<(std::ostream& os, const sf::Color& c){
		// return os << "sf::Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
        return os << fu::format::to_string(c);
	}
    typedef std::unordered_map<std::string, std::any> dict;


}