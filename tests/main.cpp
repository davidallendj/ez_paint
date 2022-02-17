// for tests
#include "config.hpp"
#include "app.hpp"
#include "image.hpp"
#include "canvas.hpp"
#include "catch.hpp"

namespace ez_paint::tests
{
	TEST_CASE("Initializing testing framework.", "[Catch2]")
	{
		// Tests requirement that must be true to be consider valid
		REQUIRE(true);
	}

	TEST_CASE("Test App::createObjects method", "[app]")
	{
		App app;
		sf::Vector2u windowSize = config::WindowSize;
		sf::Vector2u maxResolution = sf::Vector2u(1920, 1080);
		sf::Vector2u minResolution = sf::Vector2u(640, 480);

		SECTION("Test default window resolution"){
			REQUIRE((windowSize.x < maxResolution.x && windowSize.y < maxResolution.y));
			REQUIRE((windowSize.x > minResolution.x && windowSize.y > minResolution.y));
			REQUIRE(false);
		}
		SECTION("Test object creation"){
			// auto objects = app.createObjects(
			// 	Image("../res/hidethepainharold.jpg"),
			// 	Canvas(nullptr, sf::Vector2u(800, 600))
			// );
			// auto image = app.createObject(
			// 	make<Image>("../res/hidethepainharold.jpg")
			// );

			// // Check for objects
			// REQUIRE(!objects.empty());
		}
	}
}