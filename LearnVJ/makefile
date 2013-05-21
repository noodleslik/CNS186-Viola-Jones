CC=g++
CFLAGS=-O2 `pkg-config --cflags opencv` -Wall
LIBS=`pkg-config --libs opencv`

all: adaboost_main conv

adaboost_main: adaboost_main.o adaboost.o features.o ../IntegralImage/integral_image.o
	$(CC) $+ $(CFLAGS) $(LIBS) -o $@

adaboost_main.o: adaboost_main.cpp
	$(CC) $(CFLAGS) -c $<

adaboost.o: adaboost.cpp
	$(CC) $(CFLAGS) -c $<

features.o: features.cpp
	$(CC) $(CFLAGS) -c $<

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || $(MAKE) -C $(@D) $(@F) 

conv: conv.cpp
	$(CC) $< $(LIBS) $(CFLAGS) -o $@

clean:
	rm -rf *.o adaboost_main conv

