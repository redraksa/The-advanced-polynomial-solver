SHELL = cmd.exe

TARGET = translator.exe

CXX = clang++

CXXFLAGS = -Wall -std=c++17 -MMD -MP -O2

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

DEPS = $(SRCS:.cpp=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /Q /F $(TARGET).exe *.o *.d 2>nul

-include $(DEPS)

.PHONY: all clean