#CFLAGS= -O2 `pkg-config --cflags --libs opencv`
CFLAGS= -O0 `pkg-config --cflags --libs opencv` -g

all: adaboost_main

adaboost_main: adaboost_main.o adaboost.o features.o ../IntegralImage/integral_image.o
	g++ $(CFLAGS) adaboost_main.o adaboost.o features.o ../IntegralImage/integral_image.o -o adaboost_main

adaboost_main.o: adaboost_main.cpp
	g++ $(CFLAGS) -c adaboost_main.cpp    

adaboost.o: adaboost.cpp
	g++ $(CFLAGS) -c adaboost.cpp

features.o: features.cpp
	g++ $(CFLAGS) -c features.cpp

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || \
    $(MAKE) -C $(@D) $(@F) 

clean:
	rm *.o adaboost_main
