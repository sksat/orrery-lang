CXX	= clang++
CXXFLAGS= -std=c++1z -g

TARGET	= orrery
OBJS	= main.o

RUNFLAGS=

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

default:
	make $(TARGET)

run: $(TARGET)
	./$< $(RUNFLAGS)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
