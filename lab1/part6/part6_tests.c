#include <stdio.h>

#include "checkit.h"
#include "part6.h"

/* define testing functions */
void test_create_point1(void) {
   struct point p = create_point(1.9, -2.7);

   checkit_double(p.x, 1.9);
   checkit_double(p.y, -2.7);
}

void test_create_point2(void) {
   struct point p = create_point(0.2, 12.1);

   checkit_double(p.x, 0.2);
   checkit_double(p.y, 12.1);
}

void test_is_a_square1(void) {
	
	struct point p1 = create_point(1.9, -2.7);
	struct point p2 = create_point(0.2, 12.1);
	
	struct rectangle r1 = {p1, p2};
	
	checkit_boolean(is_a_square(r1), 0);
}

void test_is_a_square2(void) {
	
	struct point p1 = create_point(1, 0);
	struct point p2 = create_point(0, 1);
	
	struct rectangle r1 = {p1, p2};
	
	checkit_boolean(is_a_square(r1), 1);
}

void test_create_point(void) {
   test_create_point1();
   test_create_point2();
   test_is_a_square1();
   test_is_a_square2();
}

int main(int arg, char *argv[]) {
   /* call testing function(s) here */
   test_create_point();

   return 0;
}
