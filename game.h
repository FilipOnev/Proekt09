#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Game {
private:
    static const int BOARD_SIZE = 3;
    static const int CELL_SIZE = 150;
    static const int WINDOW_SIZE = 450;

    enum class CellState { EMPTY, X, O };
    enum class GameState { PLAYING, X_WINS, O_WINS, DRAW };

    std::vector<std::vector<CellState>> board;
    GameState gameState;
    bool isXTurn;

    sf::Font font;  
    sf::RenderWindow window;
    sf::Texture xTexture;
    sf::Texture oTexture;

    sf::Text statusText;     // Текст статуса игры (без шрифта)

    sf::RectangleShape gridLines[4];

    void initTextures();
    void initGridLines();
    void initUI();
    void handleClick(int row, int col);
    bool checkWin(CellState player);
    bool checkDraw();
    void resetGame();
    void drawBoard();
    void updateStatus();

public:
    Game();
    void run();
};