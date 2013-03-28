CFLAGS=-O2 `pkg-config --cflags opencv` -Wall
LIBS=`pkg-config --libs opencv`

all: adaboost_main conv mshcd

adaboost_main: adaboost_main.o adaboost.o features.o ../IntegralImage/integral_image.o
	g++ $+ $(CFLAGS) $(LIBS) -o $@

adaboost_main.o: adaboost_main.cpp
	g++ $(CFLAGS) -c $<

adaboost.o: adaboost.cpp
	g++ $(CFLAGS) -c $<

features.o: features.cpp
	g++ $(CFLAGS) -c $<

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || $(MAKE) -C $(@D) $(@F) 

conv: conv.cpp
	g++ $< $(LIBS) $(CFLAGS) -o $@

mshcd: mshcd.cpp mshcd.hpp
	g++ $< $(LIBS) $(CFLAGS) -o $@

clean:
	rm -rf *.o adaboost_main conv mshcd

