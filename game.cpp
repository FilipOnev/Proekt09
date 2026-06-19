#include "game.h"
#include <iostream>
#include <cmath>

Game::Game() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "TicTacToe Press R to Restart "),
isXTurn(true), gameState(GameState::PLAYING) {
    board.resize(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::EMPTY));
    initTextures();
    initGridLines();
    initUI();
    updateStatus();
}

void Game::initTextures() {
    int thickness = 20;
    sf::Image xImage;
    xImage.create(CELL_SIZE, CELL_SIZE, sf::Color::Transparent);
    for (int i = 0; i < CELL_SIZE; i++) { //i= номер строки (координата Y) ((это я сам написал))
        for (int j = 0; j < CELL_SIZE; j++) { // j номер стоблка (координата X)
            if (i == j or i + j == CELL_SIZE - 1) {
                xImage.setPixel(i, j, sf::Color::Red);
            }
        }
    }
    xTexture.loadFromImage(xImage);
    
    sf::Image oImage;
    oImage.create(CELL_SIZE, CELL_SIZE, sf::Color::Transparent);
    for (int i = 0; i < CELL_SIZE; i++) {
        for (int j = 0; j < CELL_SIZE; j++) {
            int dx = i - CELL_SIZE / 2; //dx b dy смещение от цента 
            int dy = j - CELL_SIZE / 2;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 65 && dist < 70) {
                oImage.setPixel(i, j, sf::Color::Blue);
            }
        }
    }
    oTexture.loadFromImage(oImage);
}

void Game::initGridLines() {
    for (int i = 0; i < 2; i++) {
        gridLines[i] = sf::RectangleShape(sf::Vector2f(3, WINDOW_SIZE));  // вертикальные линии
        gridLines[i].setPosition((i + 1) * CELL_SIZE - 1.5f, 0);
        gridLines[i].setFillColor(sf::Color::White);

        gridLines[i + 2] = sf::RectangleShape(sf::Vector2f(WINDOW_SIZE, 3));  // горизонтальные линии
        gridLines[i + 2].setPosition(0, (i + 1) * CELL_SIZE - 1.5f);
        gridLines[i + 2].setFillColor(sf::Color::White);
    }
}

void Game::initUI() {
    if (!font.loadFromFile("assets/arial.ttf")) {
        font.loadFromFile("../assets/arial.ttf");
    }

    statusText.setFont(font);
    statusText.setString("Pick X");
    statusText.setCharacterSize(36);
    statusText.setFillColor(sf::Color::Cyan);
    statusText.setPosition(10, WINDOW_SIZE - 60);
}

void Game::handleClick(int ряд, int столбец) {
    if (ряд < 0 or ряд >= BOARD_SIZE || столбец < 0 || столбец >= BOARD_SIZE) return;
    if (gameState != GameState::PLAYING) return;
    if (board[ряд][столбец] != CellState::EMPTY) return;

    board[ряд][столбец] = isXTurn ? CellState::X : CellState::O;
    isXTurn = !isXTurn;

    if (checkWin(CellState::X)) {
        gameState = GameState::X_WINS;
    }
    else if (checkWin(CellState::O)) {
        gameState = GameState::O_WINS;
    }
    else if (checkDraw()) {
        gameState = GameState::DRAW;
    }

    updateStatus();
}

bool Game::checkWin(CellState player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        bool rowWin = true, colWin = true;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != player) rowWin = false;
            if (board[j][i] != player) colWin = false;
        }
        if (rowWin or colWin) return true;
    }

    bool diag1Win = true, diag2Win = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][i] != player) diag1Win = false;
        if (board[i][BOARD_SIZE - 1 - i] != player) diag2Win = false;
    }
    return diag1Win or diag2Win;
}

bool Game::checkDraw() {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == CellState::EMPTY) return false;
        }
    }
    return true;
}

void Game::resetGame() {
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = CellState::EMPTY;
        }
    }
    isXTurn = true;
    gameState = GameState::PLAYING;
    updateStatus();
}

void Game::drawBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            sf::Sprite sprite;
            if (board[row][col] == CellState::X) {
                sprite.setTexture(xTexture);
            }
            else if (board[row][col] == CellState::O) {
                sprite.setTexture(oTexture);
            }
            else {
                continue;
            }
            sprite.setPosition(col * CELL_SIZE, row * CELL_SIZE);
            window.draw(sprite);
        }
    }
}

void Game::updateStatus() {
    std::string status;
    switch (gameState) {
    case GameState::PLAYING:
        status = isXTurn ? "Pick X" : "Pick O";
        break;
    case GameState::X_WINS:
        status = "Win X!";
        break;
    case GameState::O_WINS:
        status = "Win O!";
        break;
    case GameState::DRAW:
        status = "Draw!";
        break;
    }
    statusText.setString(status);
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int col = event.mouseButton.x / CELL_SIZE;
                    int row = event.mouseButton.y / CELL_SIZE;
                    
                    if (gameState != GameState::PLAYING &&
                        event.mouseButton.x > WINDOW_SIZE / 2 - 70 &&
                        event.mouseButton.x < WINDOW_SIZE / 2 + 70 &&
                        event.mouseButton.y > WINDOW_SIZE - 120 &&
                        event.mouseButton.y < WINDOW_SIZE - 60) {
                        resetGame();
                    }
                    else {
                        handleClick(row, col);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetGame();
            }
        }

        window.clear(sf::Color(50, 50, 50));

        for (auto& line : gridLines) {
            window.draw(line);
        }
        drawBoard();
        window.draw(statusText);

        if (gameState != GameState::PLAYING) {
            sf::RectangleShape restartButton(sf::Vector2f(140, 50));
            restartButton.setPosition(WINDOW_SIZE / 2 - 70, WINDOW_SIZE - 120);
            restartButton.setFillColor(sf::Color(70, 70, 200));
            restartButton.setOutlineColor(sf::Color::White);
            restartButton.setOutlineThickness(2);
            window.draw(restartButton);

            sf::Text restartText;
            restartText.setFont(font);
            restartText.setString("New game");
            restartText.setCharacterSize(24);
            restartText.setFillColor(sf::Color::White);

            sf::FloatRect textBounds = restartText.getLocalBounds();
            restartText.setPosition(
                WINDOW_SIZE / 2 - textBounds.width / 2,
                WINDOW_SIZE - 120 + (50 - textBounds.height) / 2 - 5
            );

            window.draw(restartText);
        }
        window.display();
    }
}
