TARGET = translator

CXX = g++
CXXFLAGS = -Wall -std=c++17 -MMD -MP

BISON = bison
FLEX = flex

SRCS = SimpleParser.cpp lex.yy.cpp parser.tab.cpp Emitter.cpp ResultPrinter.cpp MixedFraction.cpp Fraction.cpp Rational.cpp Polynom.cpp Monomial.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): parser.tab.cpp lex.yy.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

parser.tab.cpp parser.tab.hpp: parser.y
	$(BISON) -d -o parser.tab.cpp parser.y

lex.yy.cpp: lcl_flex_dependency

lcl_flex_dependency: lcl_lexer_deps
	$(FLEX) -o lex.yy.cpp lexer.l

lex.yy.o: parser.tab.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o *.d lex.yy.cpp parser.tab.cpp parser.tab.hpp

lcl_lexer_deps:
	@touch lcl_lexer_deps

-include $(DEPS)
