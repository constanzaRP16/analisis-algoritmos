CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lpsapi -pthread

TARGET = sorting.exe

SOURCES = sorting.cpp \
          algorithms/mergesort.cpp \
          algorithms/quicksort.cpp \
          algorithms/patiencesort.cpp \
          algorithms/sort.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

plots:
	cd scripts && python plot_generator.py

clean:
	rm -f $(TARGET)

.PHONY: all run plots clean