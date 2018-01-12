#ifndef PART6_H
#define PART6_H

struct point {
   double x;
   double y;
};

struct rectangle {
	struct point bottomRight;
	struct point topLeft;
};

struct point create_point(double x, double y);
int is_a_square(struct rectangle rect);

#endif
