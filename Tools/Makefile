CC=g++
TARGET1=draw
TARGET2=conv
TARGET3=
LIBS=`pkg-config opencv --libs`
CPPFLAGS=`pkg-config opencv --cflags` -g -Wall


all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).cpp
	$(CC) $(CPPFLAGS) -o $@ $< $(LIBS)

$(TARGET2): $(TARGET2).cpp
	$(CC) $(CPPFLAGS) -o $@ $< $(LIBS)

$(TARGET3): $(TARGET3).cpp ../c/GetHaarCascade.cpp
	$(CC) $(CPPFLAGS) -o $@ $+ $(LIBS)

clean:
	rm -rf $(TARGET1) $(TARGET2) $(TARGET3)

