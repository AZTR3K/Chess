#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "validMoves.h"
#include "menu.h"
using namespace sf;
using namespace std;

enum PlayerState
{
    MENU,
    SELECTING_PIECE,
    CHOOSING_DESTINATION,
    WHITEWINS,
    BLACKWINS,
    EXIT
};

PlayerState currentState = MENU;

const int windowWidth = 3840;
const int windowHeight = 2160;
const float cellSize = windowWidth / 18;

const int whiteKing = 5;
const int whiteQueen = 6;
const int whiteBishop = 4;
const int whiteKnight = 3;
const int whiteRook = 2;
const int whitePawn = 1;
const int blackKing = -5;
const int blackQueen = -6;
const int blackBishop = -4;
const int blackKnight = -3;
const int blackRook = -2;
const int blackPawn = -1;

bool isWhiteTurn = true;
int blackKingX = 0;
int blackKingY = 0;
int whiteKingX = 0;
int whiteKingY = 0;

void findBlackKingPosition(int& x, int& y)
{
    // Iterate through the board to find the position of the black king
    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            if (board[i][j] == -5) // Assuming -5 represents the black king
            {
                x = i;
                y = j;
                return;
            }
        }
    }
    // If the black king is not found, set x and y to -1 (or handle this case differently based on your needs)
    x = 0;
    y = 0;
}

void findWhiteKingPosition(int& x, int& y)
{
    // Iterate through the board to find the position of the white king
    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            if (board[i][j] == 5) // Assuming 5 represents the white king
            {
                x = i;
                y = j;
                return;
            }
        }
    }
    // If the black king is not found, set x and y to -1 (or handle this case differently based on your needs)
    x = 0;
    y = 0;
}

class Chessboard
{
public:
    Texture boardTexture;
    Sprite boardSprite;

    Chessboard()
    {
        // Load the board texture
        if (!boardTexture.loadFromFile("images/board.jpg"))
        {
            cerr << "Failed to load image!" << endl;
        }

        // Set the board texture to the board sprite
        boardSprite.setTexture(boardTexture);

        // Scaling Sprites
        float scaleFactor = 5.2f;
        boardSprite.setScale(scaleFactor, scaleFactor);

        // Positioning Sprites
        float boardPosX = (windowWidth - boardSprite.getGlobalBounds().width) / 2.0f;
        float boardPosY = (windowHeight - boardSprite.getGlobalBounds().height) / 3.0f;
        boardSprite.setPosition(boardPosX, boardPosY);
    }

    void draw(RenderWindow& window) const
    {
        // Draw the board sprite
        window.draw(boardSprite);

        // Draw the board grid
        bool isAlternate = true; // For alternate colors in the grid
        RectangleShape box(Vector2f(cellSize, cellSize));

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (isAlternate)
                {
                    box.setFillColor(Color(245, 222, 179));
                }
                else
                {
                    box.setFillColor(Color(111, 78, 55));
                }
                isAlternate = !isAlternate; // Toggle color for the next box
                // Adjust position to match the board sprite
                box.setPosition(boardSprite.getPosition().x + (j + 0.385) * cellSize, boardSprite.getPosition().y + (i + 0.40) * cellSize);
                window.draw(box);
            }
            // Flip the color for the next row
            isAlternate = !isAlternate;
        }
    }
};

// Defining Piece Parent class
class Piece
{
public:
    bool whiteKingInCheck = false;
    bool blackKingInCheck = false;
    virtual ~Piece() {}

    virtual void draw(RenderWindow& window, const float posX, const float posY) = 0;
    virtual bool isValidMove(int x1, int y1, int x2, int y2) = 0;
    virtual bool isInCheck(int kingX, int kingY) const { return false; }
    virtual bool isCheckmate(int kingX, int kingY) const { return false; }
};

Piece* pieces[8][8];

void swapCells(int x1, int y1, int x2, int y2);

void swapCells(Piece* Pieces[][8], int x1, int y1, int x2, int y2);

// Define WhiteKing subclass
class WhiteKing : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhiteKing(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    WhiteKing(const WhiteKing& other)
    {
        this->texture = other.texture;
        this->sprite = other.sprite;
        this->position = other.position;
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validKingW(x1, y1, x2, y2))
            {
                if (!isInCheck(y2, x2))
                    return true;
            }
        }

        return false;
    }

    bool isInCheck(int kingX, int kingY) const override
    {
        // Iterate through all black pieces
        for (int i = 0; i < boardSize; ++i)
        {
            for (int j = 0; j < boardSize; ++j)
            {
                // Check if the black piece at position (i, j) can attack the white king
                switch (board[i][j])
                {
                case -1:
                    if (validPawnB(j, i, kingY, kingX))
                    {
                        cout << "Pawn attacking\n";
                        return true;
                    }
                    break;
                case -2:
                    if (validRookB(j, i, kingY, kingX))
                    {
                        cout << "Rook attacking\n";
                        return true;
                    }
                    break;
                case -3:
                    if (validKnightB(j, i, kingY, kingX))
                    {
                        cout << "Knight attacking\n";
                        return true;
                    }
                    break;
                case -4:
                    if (validBishopB(j, i, kingY, kingX))
                    {
                        cout << "Bishop attacking\n";
                        return true;
                    }
                    break;
                case -5:
                    if (validKingB(j, i, kingY, kingX))
                    {
                        cout << "King attacking\n";
                        return true;
                    }
                    break;
                case -6:
                    if (validQueenB(j, i, kingY, kingX))
                    {
                        cout << "Queen attacking\n";
                        return true;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        return false; // White king is not in check
    }

    bool isCheckmate(int kingX, int kingY) const override
    {
        int counter = 0;
        if (isInCheck(kingX, kingY))
        {
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (validKingW(kingY, kingX, j, i))
                    {
                        if (!isInCheck(i, j))
                        {
                            counter++;
                        }
                    }
                }
            }
            cout << "Counter: " << counter;
            if (counter == 0)
            {
                cout << " Checkmate!!!";
                return true;
            }

            else if (counter > 0)
            {
                cout << " Not checkmate...";
                return false;
            }
        }

        return false;
    }
};

// Define BlackKing subclass
class BlackKing : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackKing(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validKingB(x1, y1, x2, y2))
            {
                if (!isInCheck(y2, x2))
                    return true;
            }
        }

        return false;
    }

    bool isInCheck(int kingX, int kingY) const override
    {
        // Iterate through all black pieces
        for (int i = 0; i < boardSize; ++i)
        {
            for (int j = 0; j < boardSize; ++j)
            {
                // Check if the white piece at position (i, j) can attack the black king
                switch (board[i][j])
                {
                case 1:
                    if (validPawnW(j, i, kingY, kingX))
                    {
                        cout << "Pawn attacking\n";
                        return true;
                    }
                    break;
                case 2:
                    if (validRookW(j, i, kingY, kingX))
                    {
                        cout << "Rook attacking\n";
                        return true;
                    }
                    break;
                case 3:
                    if (validKnightW(j, i, kingY, kingX))
                    {
                        cout << "Knight attacking\n";
                        return true;
                    }
                    break;
                case 4:
                    if (validBishopW(j, i, kingY, kingX))
                    {
                        cout << "Bishop attacking\n";
                        return true;
                    }
                    break;
                case 5:
                    if (validKingW(j, i, kingY, kingX))
                    {
                        cout << "King attacking\n";
                        return true;
                    }
                    break;
                case 6:
                    if (validQueenW(j, i, kingY, kingX))
                    {
                        cout << "Queen attacking\n";
                        return true;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        return false; // Black king is not in check
    }

    bool isCheckmate(int kingX, int kingY) const override
    {
        if (isInCheck(kingX, kingY))
        {
            int counter = 0;
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (validKingB(kingY, kingX, j, i))
                    {
                        if (!isInCheck(i, j))
                        {
                            counter++;
                        }
                    }
                }
            }
            cout << "Counter: " << counter << endl;
            if (counter == 0)
            {
                cout << " Checkmate!!!\n";
                return true;
            }

            else if (counter > 0)
            {
                cout << " Not checkmate...\n";
                return false;
            }
        }

        return false;
    }

};

// Define WhiteQueen subclass
class WhiteQueen : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhiteQueen(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validQueenW(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define BlackQueen subclass
class BlackQueen : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackQueen(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validQueenB(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define WhiteRook subclass
class WhiteRook : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhiteRook(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validRookW(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define BlackRook subclass
class BlackRook : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackRook(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validRookB(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define WhiteBishop subclass
class WhiteBishop : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhiteBishop(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validBishopW(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define BlackBishop subclass
class BlackBishop : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackBishop(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validBishopB(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define WhiteKnight subclass
class WhiteKnight : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhiteKnight(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validKnightW(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define BlackKnight subclass
class BlackKnight : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackKnight(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validKnightB(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define WhitePawn subclass
class WhitePawn : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    WhitePawn(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == true)
        {
            if (validPawnW(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

// Define BlackPawn subclass
class BlackPawn : public Piece
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;

public:
    BlackPawn(const Texture& tex, const Vector2f& pos) : texture(tex), position(pos)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window, const float posX, const float posY) override
    {
        sprite.setPosition(posX, posY);
        float scaleFactor = 0.7f;
        sprite.setScale(scaleFactor, scaleFactor);
        window.draw(sprite);
    }

    bool isValidMove(int x1, int y1, int x2, int y2)
    {
        if (isWhiteTurn == false)
        {
            if (validPawnB(x1, y1, x2, y2))
                return true;
        }

        return false;
    }
};

void swapCells(int x1, int y1, int x2, int y2)
{
    int temp = board[y1][x1];
    board[y1][x1] = 0;
    board[y2][x2] = temp;
}

void swapCells(Piece* Pieces[][8], int x1, int y1, int x2, int y2)
{
    Piece* temp = Pieces[y1][x1];
    Pieces[y1][x1] = nullptr;
    Pieces[y2][x2] = temp;
}

int main()
{
    RenderWindow window(VideoMode(windowWidth, windowHeight), "CHESSSS");
    Chessboard chessboard;
    MainMenu mainMenu(window.getSize().x, window.getSize().y);
    Texture piecesTextures[12];

    // Font
    Text whiteText;
    Text blackText;
    Font font;
    if (!font.loadFromFile("fonts/Alexandriaflf.ttf"))
    {
        cout << "No font is here";
    }
    whiteText.setFont(font);
    whiteText.setFillColor(Color::White);
    whiteText.setString("White Won by Checkmate");
    whiteText.setCharacterSize(75);
    whiteText.setPosition(600, 300);

    blackText.setFont(font);
    blackText.setFillColor(Color::White);
    blackText.setString("Black Won by Checkmate");
    blackText.setCharacterSize(75);
    blackText.setPosition(600, 300);

    // Set Background
    RectangleShape background;
    background.setSize(Vector2f(windowWidth, windowHeight));
    Texture MainTexture;
    MainTexture.loadFromFile("images/background3.jpg");
    background.setTexture(&MainTexture);

    Image icon;
    icon.loadFromFile("images/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load textures for each piece
    if (!piecesTextures[0].loadFromFile("images/wk.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[1].loadFromFile("images/wq.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[2].loadFromFile("images/wr.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[3].loadFromFile("images/wb.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[4].loadFromFile("images/wn.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[5].loadFromFile("images/wp.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[6].loadFromFile("images/bk.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[7].loadFromFile("images/bq.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[8].loadFromFile("images/br.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[9].loadFromFile("images/bb.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[10].loadFromFile("images/bn.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }
    if (!piecesTextures[11].loadFromFile("images/bp.png"))
    {
        cerr << "Failed to load image!" << endl;
        return 1;
    }

    // Load sound(s) for each move
    SoundBuffer move, check;
    if (!move.loadFromFile("sounds/move-self.mp3"))
    {
        cerr << "Failed to load sound" << endl;
    }
    if (!check.loadFromFile("sounds/move-check.mp3"))
    {
        cerr << "Failed to load sound" << endl;
    }
    Sound moveSound, checkSound;
    moveSound.setBuffer(move);
    checkSound.setBuffer(check);

    // Scaling Sprites
    float scaleFactor = 5.2f;
    float pieceScale = 0.7f;

    // Positioning Sprites
    float boardPosX = (window.getSize().x - chessboard.boardSprite.getGlobalBounds().width) / 2.0f;
    float boardPosY = (window.getSize().y - chessboard.boardSprite.getGlobalBounds().height) / 3.0f;

    Piece* pieces[8][8];

    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            switch (board[i][j])
            {
            case whiteKing:
                pieces[i][j] = new WhiteKing(piecesTextures[0], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case whiteQueen:
                pieces[i][j] = new WhiteQueen(piecesTextures[1], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case whiteRook:
                pieces[i][j] = new WhiteRook(piecesTextures[2], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case whiteBishop:
                pieces[i][j] = new WhiteBishop(piecesTextures[3], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case whiteKnight:
                pieces[i][j] = new WhiteKnight(piecesTextures[4], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case whitePawn:
                pieces[i][j] = new WhitePawn(piecesTextures[5], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackKing:
                pieces[i][j] = new BlackKing(piecesTextures[6], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackQueen:
                pieces[i][j] = new BlackQueen(piecesTextures[7], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackRook:
                pieces[i][j] = new BlackRook(piecesTextures[8], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackBishop:
                pieces[i][j] = new BlackBishop(piecesTextures[9], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackKnight:
                pieces[i][j] = new BlackKnight(piecesTextures[10], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            case blackPawn:
                pieces[i][j] = new BlackPawn(piecesTextures[11], Vector2f(boardPosX + j * cellSize, boardPosY + i * cellSize));
                break;
            default:
                pieces[i][j] = nullptr;
                break;
            }
        }
    }

    int selectedX = -1;
    int selectedY = -1;
    int firstX = -1;
    int firstY = -1;


    while (window.isOpen())
    {
        // Clear the window
        window.clear();

        // Event handling
        Event event;

        // Draw the final score text in the game over state
        if (currentState == PlayerState::WHITEWINS)
        {
            window.draw(background);
            window.draw(whiteText);
            window.display();
        }

        else if (currentState == PlayerState::BLACKWINS)
        {
            window.draw(background);
            window.draw(blackText);
            window.display();
        }

        else if (currentState == MENU)
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
            }
            // Handle mouse input
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    // Get the mouse position relative to the MENU window
                    Vector2f mousePosition = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

                    // Handle mouse input in the MainMenu
                    mainMenu.handleMouseInput(mousePosition);

                    // Check if a menu item is clicked
                    if (mainMenu.MainMenuPressed() == 0)
                    {
                        // Play option selected
                        currentState = SELECTING_PIECE;
                    }
                    else if (mainMenu.MainMenuPressed() == 1)
                    {
                        // Exit option selected
                        window.close(); // Close the main game window
                    }
                }
            }

            // Clear the MENU window
            window.clear();

            // Draw the main menu
            window.draw(background);
            mainMenu.draw(window);

            // Display the MENU window
            window.display();

        }

        else
        {
            // Draw the chessboard
            chessboard.draw(window);


            // Draw pieces
            for (int i = 0; i < boardSize; ++i)
            {
                for (int j = 0; j < boardSize; ++j)
                {
                    if (pieces[i][j] != nullptr)
                    {
                        pieces[i][j]->draw(window, boardPosX + (j + 0.40f) * cellSize, boardPosY + (i + 0.385f) * cellSize);
                    }
                }
            }

            // Highlight selected square
            if (selectedX != -1 && selectedY != -1)
            {
                RectangleShape highlight(Vector2f(cellSize - 9, cellSize - 9));
                highlight.setFillColor(Color(255, 255, 255, 0));
                highlight.setOutlineThickness(7.0);
                highlight.setOutlineColor(Color::Black);
                highlight.setPosition(boardPosX + (selectedX + 0.40) * (cellSize), boardPosY + (selectedY + 0.385 + 0.05) * (cellSize));
                if (board[selectedY][selectedX] > 0 && isWhiteTurn)
                {
                    window.draw(highlight);
                }
                else if (board[selectedY][selectedX] < 0 && !isWhiteTurn)
                {
                    window.draw(highlight);
                }
            }

            // Display everything drawn
            window.display();

            // Event handling
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                else if (event.type == Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Escape)
                    {
                        window.close();
                    }
                }
                else if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        int x = (event.mouseButton.x - boardPosX) / cellSize - 0.40;
                        int y = (event.mouseButton.y - boardPosY) / cellSize - 0.385;

                        if (x >= 0 && x < boardSize && y >= 0 && y < boardSize)
                        {
                            if (currentState == SELECTING_PIECE)
                            {
                                // Selecting a piece
                                if (pieces[y][x] != nullptr)
                                {
                                    selectedX = x;
                                    selectedY = y;
                                    currentState = CHOOSING_DESTINATION;
                                }
                            }
                            else if (currentState == CHOOSING_DESTINATION)
                            {
                                // Choosing a destination for the selected piece
                                if (selectedX != -1 && selectedY != -1 && pieces[selectedY][selectedX] != nullptr)
                                {
                                    cout << x << " " << y << endl;
                                    if (pieces[selectedY][selectedX]->isValidMove(selectedX, selectedY, x, y))
                                    {
                                        // Perform the move
                                        int backup = board[y][x];
                                        Piece* save = pieces[y][x];
                                        swapCells(selectedX, selectedY, x, y);
                                        swapCells(pieces, selectedX, selectedY, x, y);
                                        findWhiteKingPosition(whiteKingX, whiteKingY);
                                        findBlackKingPosition(blackKingX, blackKingY);
                                        if (isWhiteKingInCheck = pieces[whiteKingX][whiteKingY]->isInCheck(whiteKingX, whiteKingY) && isWhiteTurn)
                                        {
                                            swapCells(x, y, selectedX, selectedY);
                                            swapCells(pieces, x, y, selectedX, selectedY);
                                            board[y][x] = backup;
                                            pieces[y][x] = save;
                                            isWhiteTurn = !isWhiteTurn;
                                        }
                                        if (isBlackKingInCheck = pieces[blackKingX][blackKingY]->isInCheck(blackKingX, blackKingY) && !isWhiteTurn)
                                        {
                                            checkSound.play();
                                            swapCells(x, y, selectedX, selectedY);
                                            swapCells(pieces, x, y, selectedX, selectedY);
                                            board[y][x] = backup;
                                            pieces[y][x] = save;
                                            isWhiteTurn = !isWhiteTurn;
                                        }
                                        if (isWhiteKingInCheck = pieces[whiteKingX][whiteKingY]->isInCheck(whiteKingX, whiteKingY))
                                        {
                                            checkSound.play();
                                        }
                                        if (isBlackKingInCheck = pieces[blackKingX][blackKingY]->isInCheck(blackKingX, blackKingY))
                                        {
                                            checkSound.play();
                                        }
                                        isWhiteTurn = !isWhiteTurn;
                                        moveSound.play();

                                    }
                                }
                                // Reset selection
                                selectedX = -1;
                                selectedY = -1;
                                currentState = SELECTING_PIECE;

                                if (pieces[whiteKingX][whiteKingY]->isCheckmate(whiteKingX, whiteKingY))
                                {
                                    currentState = BLACKWINS;
                                }
                                if (pieces[blackKingX][blackKingY]->isCheckmate(blackKingX, blackKingY))
                                {
                                    currentState = WHITEWINS;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}