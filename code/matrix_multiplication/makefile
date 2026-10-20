CXX = g++
CXXFLAGS = -std=c++17
LIBS = -lpsapi -pthread

TARGET = matrix_multiplication.exe

SOURCES = matrix_multiplication.cpp \
          algorithms/naive.cpp \
          algorithms/strassen.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

plots:
	cd scripts && python plot_generator.py

clean:
	rm -f $(TARGET)

.PHONY: all run plots clean