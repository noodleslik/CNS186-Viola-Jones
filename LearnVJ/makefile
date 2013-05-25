CC = g++
CPPFLAGS = `pkg-config --cflags opencv` -Wall
LIBS = `pkg-config --libs opencv`

all: adaboost_main conv build_cascade verify

adaboost_main.o: adaboost_main.cpp

adaboost.o: adaboost.cpp adaboost.cpp feature_type.h

features.o: features.cpp features.h feature_type.h

BOOST_OBJS = adaboost.o features.o ../IntegralImage/integral_image.o

adaboost_main: adaboost_main.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || $(MAKE) -C $(@D) $(@F) 

conv: conv.cpp
	$(CC) $< $(LIBS) $(CPPFLAGS) -o $@

build_cascade.o: build_cascade.cpp

build_cascade: build_cascade.o verify.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

verify_main.o: verify_main.cpp

verify.o: verify.cpp

verify: verify_main.o verify.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

clean:
	rm -rf *.o adaboost_main conv build_cascade verify

