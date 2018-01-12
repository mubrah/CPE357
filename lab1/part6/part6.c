#include "part6.h"

struct point create_point(double x, double y) {
   struct point p = {x, y};
   return p;
}

int is_a_square(struct rectangle rect) {
	double width = rect.bottomRight.x - rect.topLeft.x;
	double height = rect.topLeft.y - rect.bottomRight.y;
	
	if (width == height) {
		return 1;
	} else {
		return 0;
	}	
}
