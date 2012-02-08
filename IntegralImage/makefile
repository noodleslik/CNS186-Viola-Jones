integral_image_test: integral_image_test.o integral_image.o
	g++ -O2 `pkg-config --cflags --libs opencv` integral_image_test.o integral_image.o -o integral_image_test

integral_image.o: integral_image.cpp
	g++ -O2 `pkg-config --cflags --libs opencv` -c integral_image.cpp

integral_image_test.o: integral_image_test.cpp
	g++ -O2 `pkg-config --cflags --libs opencv` -c integral_image_test.cpp

