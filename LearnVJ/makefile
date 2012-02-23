adaboost.o: adaboost.cpp
	g++ -O2 `pkg-config --cflags --libs opencv` -c adaboost.cpp

features.o: features.cpp
	g++ -O2 `pkg-config --cflags --libs opencv` -c features.cpp

../IntegralImage/integral_image.o::
	@$(MAKE) -C $(@D) $(@F) -q || \
    $(MAKE) -C $(@D) $(@F) 
