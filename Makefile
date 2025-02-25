CXX = g++
CXXFLAGS = -std=c++17 \
           -I/opt/homebrew/opt/sfml/include \
           -I$(shell python3 -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())")/torch/include \
           -I$(shell python3 -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())")/torch/csrc/api/include

LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system \
          -L$(shell python3 -c "import torch; print(torch.__path__[0])")/lib \
          -ltorch -ltorch_cpu -lc10

# Source files
SRCS = main.cpp ChessGame.cpp ChessPiece.cpp Pawn.cpp
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = chess

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
