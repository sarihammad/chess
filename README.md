# Chess

An implementation of Chess using C++

## How to Run (On Mac)

1. **Install SFML**

   Open a terminal and run the following command to install SFML:

   ```sh
   brew install sfml
   ```

2. **Compile the Code**

   Open a terminal and run the following command to compile the source files:

   ```sh
   g++ main.cpp board.cpp king.cpp queen.cpp pawn.cpp rook.cpp bishop.cpp knight.cpp promotionsquare.cpp chesspiece.cpp chessgame.cpp \
   -I/opt/homebrew/Cellar/sfml/2.6.1/include \
   -L/opt/homebrew/Cellar/sfml/2.6.1/lib \
   -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
   -o chess
   ```

3. **Run the Executable**

  After compiling, execute the program with the following command:

```sh
  ./chess
```



## Controls

* The mouse drags the pieces on the board to a valid square
* The "f" key flips the board
* The "r" key resets the board
* The "u" key undos the last move

## Preview

![preview](https://user-images.githubusercontent.com/63919507/188940128-b0916b22-a747-4e29-83c7-4596eb01ab9a.gif)








