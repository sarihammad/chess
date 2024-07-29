CXX = g++
CXXFLAGS = -I/opt/homebrew/Cellar/sfml/2.6.1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRCS = main.cpp board.cpp king.cpp queen.cpp pawn.cpp rook.cpp bishop.cpp knight.cpp promotionsquare.cpp chesspiece.cpp chessgame.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = chess

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
