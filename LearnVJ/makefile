CFLAGS=-O2 `pkg-config --cflags opencv` -Wall
LIBS=`pkg-config --libs opencv`

all: adaboost_main conv

adaboost_main: adaboost_main.o adaboost.o features.o ../IntegralImage/integral_image.o
	g++ $+ $(CFLAGS) $(LIBS) -o $@

adaboost_main.o: adaboost_main.cpp
	g++ $(CFLAGS) -c adaboost_main.cpp

adaboost.o: adaboost.cpp
	g++ $(CFLAGS) -c adaboost.cpp

features.o: features.cpp
	g++ $(CFLAGS) -c features.cpp

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || $(MAKE) -C $(@D) $(@F) 

conv: conv.cpp
	g++ $< $(CFLAGS) $(LIBS) -o $@

clean:
	rm -rf *.o adaboost_main
