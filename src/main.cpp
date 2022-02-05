
#include "app.hpp"
#include "config.hpp"
#include "canvas.hpp"
#include "utils.hpp"
#include "log.hpp"

#include <backward.hpp>


int main(int argc, char **argv)
{
    using namespace fu;

#ifdef X_PROTOCOL
	XInitThreads();
#endif

	/* Create app window */
	
	/* Print configuration information */
	config::showConfig();

	/* Create an object app to handle object loops */
	App app(argc, argv);
    return app.runLoop();
}