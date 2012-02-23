CFLAGS= -O2 `pkg-config --cflags --libs opencv`

all: integral_image_test

integral_image_test: integral_image_test.o integral_image.o
	g++ $(CFLAGS) integral_image.o integral_image_test.o -o integral_image_test

integral_image.o: integral_image.cpp
	g++ $(CFLAGS) -c integral_image.cpp

integral_image_test.o: integral_image_test.cpp
	g++ $(CFLAGS) -c integral_image_test.cpp

clean: 
	rm *.o integral_image_test
