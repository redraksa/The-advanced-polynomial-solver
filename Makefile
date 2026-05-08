TARGET = translator

CXX = g++
CXXFLAGS = -Wall -std=c++17 -MMD -MP

BISON = bison
FLEX = flex

SRPC = SimpleParser.cpp lex.yy.cpp parser.tab.cpp Emitter.cpp ResultPrinter.cpp MixedFraction.cpp Fraction.cpp Rational.cpp
OBJS = $(SRPC:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): parser.tab.cpp lex.yy.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

parser.tab.cpp parser.tab.hpp: parser.y
	$(BISON) -d -o parser.tab.cpp parser.y

lex.yy.cpp: lexer.l parser.tab.hpp
	$(FLEX) -o lex.yy.cpp lexer.l

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o *.d lex.yy.cpp parser.tab.cpp parser.tab.hpp

-include $(DEPS)
