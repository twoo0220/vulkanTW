#include "MainEntry.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

int main()
{
	MainEntry app(1000, 600);
	app.run();

	return 0;
}
