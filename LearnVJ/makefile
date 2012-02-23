CFLAGS= -O2 `pkg-config --cflags --libs opencv`

adaboost.o: adaboost.cpp
	g++ $(CFLAGS) -c adaboost.cpp

features.o: features.cpp
	g++ $(CFLAGS) -c features.cpp

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || \
    $(MAKE) -C $(@D) $(@F) 

clean:
	rm *.o
