CXX	= clang++
CXXFLAGS= -std=c++1z -g

TARGET	= orrery
OBJS	= main.o token.o parse.o

RUNFLAGS= test/expr.ory

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

default:
	make $(TARGET)

run: $(TARGET)
	./$< $(RUNFLAGS)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
