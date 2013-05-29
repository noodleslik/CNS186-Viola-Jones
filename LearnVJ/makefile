CC = g++
CPPFLAGS = `pkg-config --cflags opencv` -Wall -lpthread
LIBS = `pkg-config --libs opencv` -lpthread

all: adaboost_main conv build_cascade verify

adaboost_main.o: adaboost_main.cpp

adaboost.o: adaboost.cpp adaboost.h feature_type.h ../Array/array.hpp

features.o: features.cpp features.h feature_type.h

BOOST_OBJS = adaboost.o features.o ../IntegralImage/integral_image.o

adaboost_main: adaboost_main.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || $(MAKE) -C $(@D) $(@F) 
##########
conv: conv.cpp
	$(CC) $< $(LIBS) $(CPPFLAGS) -o $@

build_cascade.o: build_cascade.cpp

build_cascade: build_cascade.o verify.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@
##########
verify_main.o: verify_main.cpp

verify_cascade.o: verify_cascade.cpp

verify.o: verify.cpp

verify_main: verify_main.o verify.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

verify_cascade: verify_cascade.o verify.o $(BOOST_OBJS)
	$(CC) $+ $(LIBS) -o $@

.PHONY: verify
verify: verify_main verify_cascade

clean:
	rm -rf *.o adaboost_main conv build_cascade
	rm -rf verify_main verify_cascade 

dist_clean: clean
	rm -rf stage0*.txt

