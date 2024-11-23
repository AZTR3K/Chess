#pragma once
#ifndef MYHEADER_H
#define MYHEADER_H

int board[8][8] =
{ -2,-3,-4,-6,-5,-4,-3,-2,
 -1,-1,-1,-1,-1,-1,-1,-1,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  2, 3, 4, 6, 5, 4, 3, 2,
};

const int boardSize = 8;
bool isWhiteKingInCheck = false;
bool isBlackKingInCheck = false;
bool hasKingMovedW = false;
bool hasKingMovedB = false;
bool whiteLeftRookMoved = false;
bool whiteRightRookMoved = false;
bool blackLeftRookMoved = false;
bool blackRightRookMoved = false;

// Contents of the header file
bool validPawnW(int x1, int y1, int x2, int y2)
{
    bool isFirstMove = (y1 == 6); // Assuming pawns start at row 6 for white

    // Check if the destination cell is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by an opponent's piece diagonally
    if ((x2 == x1 - 1 || x2 == x1 + 1) && y2 == y1 - 1 && board[y2][x2] < 0)
    {
        // Capture move
        return true;
    }

    // Check for regular pawn move
    if (isFirstMove)
    {
        if (x1 == x2 && (y2 == y1 - 1 || y2 == y1 - 2) && board[y2][x2] == 0) // Pawn can move one or two cells forward
        {
            return true;
        }
    }
    else
    {
        if (x1 == x2 && y2 == y1 - 1 && board[y2][x2] == 0) // Pawn can move only one cell forward
        {
            return true;
        }
    }

    return false; // Invalid move
}

bool validPawnB(int x1, int y1, int x2, int y2)
{
    bool isFirstMove = (y1 == 1); // Assuming pawns start at row 1 for black

    // Check if the destination cell is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by an opponent's piece diagonally
    if ((x2 == x1 + 1 || x2 == x1 - 1) && y2 == y1 + 1 && board[y2][x2] > 0)
    {
        // Capture move
        return true;
    }

    // Check for regular pawn move
    if (isFirstMove)
    {
        if (x1 == x2 && (y2 == y1 + 1 || y2 == y1 + 2) && board[y2][x2] == 0) // Pawn can move one or two cells forward
        {
            return true;
        }
    }
    else
    {
        if (x1 == x2 && y2 == y1 + 1 && board[y2][x2] == 0) // Pawn can move only one cell forward
        {
            return true;
        }
    }

    return false; // Invalid move
}

bool validKingW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is not occupied by a friendly piece
    if (board[y2][x2] > 0) // Assuming positive values represent white pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Calculate absolute values manually
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    // Check if the move is only one square away (including diagonals)
    if (dx <= 1 && dy <= 1)
        return true;

    return false;
}

bool validKingB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is not occupied by a friendly piece
    if (board[y2][x2] < 0) // Assuming negative values represent black pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Calculate absolute values manually
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    // Check if the move is only one square away (including diagonals)
    if (dx <= 1 && dy <= 1)
        return true;

    return false;
}

bool validBishopW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] > 0) // Assuming positive values represent white pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is on diagonals
    if (dx == dy || dx == -dy)
    {
        // Determine the direction of movement along each axis
        int dxSign, dySign;
        if (dx > 0) dxSign = 1; else dxSign = -1;
        if (dy > 0) dySign = 1; else dySign = -1;

        // Check if there are any pieces blocking the path diagonally
        int x = x1 + dxSign;
        int y = y1 + dySign;
        while (x != x2 && y != y2)
        {
            if (board[y][x] != 0)
                return false; // Path is blocked
            x += dxSign;
            y += dySign;
        }

        return true; // Path is clear, valid move
    }

    return false; // If not diagonal, return false
}

bool validBishopB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] < 0) // Assuming negative values represent black pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is on diagonals
    if (dx == dy || dx == -dy)
    {
        // Determine the direction of movement along each axis
        int dxSign, dySign;
        if (dx > 0) dxSign = 1; else dxSign = -1;
        if (dy > 0) dySign = 1; else dySign = -1;

        // Check if there are any pieces blocking the path diagonally
        int x = x1 + dxSign;
        int y = y1 + dySign;
        while (x != x2 && y != y2)
        {
            if (board[y][x] != 0)
                return false; // Path is blocked
            x += dxSign;
            y += dySign;
        }

        return true; // Path is clear, valid move
    }

    return false; // If not diagonal, return false
}

bool validRookW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] > 0) // Assuming positive values represent white pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is along a horizontal or vertical line
    if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0))
    {
        // Check if there are any pieces blocking the path
        int x, y;
        if (dx == 0) // Move is along the y-axis
        {
            int ySign = (dy > 0) ? 1 : -1;
            y = y1 + ySign;
            while (y != y2)
            {
                if (board[y][x1] != 0)
                    return false; // Path is blocked
                y += ySign;
            }
        }
        else // Move is along the x-axis
        {
            int xSign = (dx > 0) ? 1 : -1;
            x = x1 + xSign;
            while (x != x2)
            {
                if (board[y1][x] != 0)
                    return false; // Path is blocked
                x += xSign;
            }
        }
        return true; // Valid move
    }
    return false; // Invalid move
}

bool validRookB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] < 0) // Assuming negative values represent black pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is along a horizontal or vertical line
    if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0))
    {
        // Check if there are any pieces blocking the path
        int x, y;
        if (dx == 0) // Move is along the y-axis
        {
            int ySign = (dy > 0) ? 1 : -1;
            y = y1 + ySign;
            while (y != y2)
            {
                if (board[y][x1] != 0)
                    return false; // Path is blocked
                y += ySign;
            }
        }
        else // Move is along the x-axis
        {
            int xSign = (dx > 0) ? 1 : -1;
            x = x1 + xSign;
            while (x != x2)
            {
                if (board[y1][x] != 0)
                    return false; // Path is blocked
                x += xSign;
            }
        }
        return true; // Valid move
    }
    return false; // Invalid move
}

bool validQueenW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] > 0) // Assuming positive values represent white pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is along a horizontal, vertical, or diagonal line
    if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0) || (dx == dy || dx == -dy))
    {
        // Check if the move is along a straight line (rook move)
        if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0))
        {
            // Use the validRookW function to check the path for rook moves
            return validRookW(x1, y1, x2, y2);
        }
        else // Otherwise, the move is diagonal (bishop move)
        {
            // Use the validBishopW function to check the path for bishop moves
            return validBishopW(x1, y1, x2, y2);
        }
    }

    return false; // If not along a straight line or diagonal, return false
}

bool validQueenB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] < 0) // Assuming negative values represent black pieces
        return false;

    // Calculate the difference in coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Check if the move is along a horizontal, vertical, or diagonal line
    if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0) || (dx == dy || dx == -dy))
    {
        // Check if the move is along a straight line (rook move)
        if ((dx == 0 && dy != 0) || (dx != 0 && dy == 0))
        {
            // Use the validRookB function to check the path for rook moves
            return validRookB(x1, y1, x2, y2);
        }
        else // Otherwise, the move is diagonal (bishop move)
        {
            // Use the validBishopB function to check the path for bishop moves
            return validBishopB(x1, y1, x2, y2);
        }
    }

    return false; // If not along a straight line or diagonal, return false
}

bool validKnightW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] > 0) // Assuming positive values represent white pieces
        return false;

    // Calculate the absolute differences in x and y coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    // A knight's move consists of moving two squares horizontally and one square vertically,
    // or moving two squares vertically and one square horizontally.

    // Check if the move is valid for a knight
    if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1))
        return true;

    return false; // If not a valid knight move, return false
}

bool validKnightB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x2 < 0 || x2 >= boardSize || y2 < 0 || y2 >= boardSize)
        return false;

    // Check if the destination cell is occupied by a friendly piece
    if (board[y2][x2] < 0) // Assuming negative values represent black pieces
        return false;

    // Calculate the absolute differences in x and y coordinates
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    // A knight's move consists of moving two squares horizontally and one square vertically,
    // or moving two squares vertically and one square horizontally.

    // Check if the move is valid for a knight
    if ((dx == 1 && dy == 2) || (dx == 2 && dy == 1))
        return true;

    return false; // If not a valid knight move, return false
}

// Function to update the status of whether a rook has moved
void updateRookStatus(int x, int y)
{
    // Check if the piece at the given coordinates is a rook
    if (board[y][x] == 3 || board[y][x] == -3)
    {
        // Determine the color of the rook
        bool isWhite = board[y][x] > 0;

        // Set the corresponding rook's status to true
        if (isWhite)
        {
            if ((x == 0 && y == 7) || (x == 7 && y == 7))
            {
                whiteLeftRookMoved = true; // White rook on the left side
            }
            else if ((x == 0 && y == 0) || (x == 7 && y == 0))
            {
                whiteRightRookMoved = true; // White rook on the right side
            }
        }
        else
        {
            if ((x == 0 && y == 7) || (x == 7 && y == 7))
            {
                blackLeftRookMoved = true; // Black rook on the left side
            }
            else if ((x == 0 && y == 0) || (x == 7 && y == 0))
            {
                blackRightRookMoved = true; // Black rook on the right side
            }
        }
    }
}

// Function to check whether a rook has moved
bool hasRookMoved(int x, int y)
{
    // Determine the color of the rook
    bool isWhite = board[y][x] > 0;

    // Check the corresponding status boolean variable
    if (isWhite)
    {
        if ((x == 0 && y == 7) || (x == 7 && y == 7))
        {
            return whiteLeftRookMoved; // White rook on the left side
        }
        else if ((x == 0 && y == 0) || (x == 7 && y == 0))
        {
            return whiteRightRookMoved; // White rook on the right side
        }
    }
    else
    {
        if ((x == 0 && y == 7) || (x == 7 && y == 7))
        {
            return blackLeftRookMoved; // Black rook on the left side
        }
        else if ((x == 0 && y == 0) || (x == 7 && y == 0))
        {
            return blackRightRookMoved; // Black rook on the right side
        }
    }

    // Default case: return false if the coordinates do not correspond to a rook
    return false;
}

bool isSquareUnderAttackW(int x, int y)
{
    // Check if the square is under attack by a black pawn
    if ((x > 0 && y < boardSize - 1) && (board[y + 1][x - 1] == -1 || board[y + 1][x + 1] == -1))
        return true;

    // Check if the square is under attack by a black knight
    if (x > 1 && y < boardSize - 2)
    {
        if (board[y + 2][x - 1] == -2 || board[y + 2][x + 1] == -2)
            return true;
    }
    if (x < boardSize - 2 && y < boardSize - 2)
    {
        if (board[y + 2][x - 1] == -2 || board[y + 2][x + 1] == -2)
            return true;
    }
    if (x > 0 && y < boardSize - 3)
    {
        if (board[y + 1][x - 2] == -2 || board[y + 1][x + 2] == -2)
            return true;
    }
    if (x < boardSize - 3 && y < boardSize - 3)
    {
        if (board[y + 1][x - 2] == -2 || board[y + 1][x + 2] == -2)
            return true;
    }

    // Check if the square is under attack by a black king
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            if (x + i >= 0 && x + i < boardSize && y + j >= 0 && y + j < boardSize && board[y + j][x + i] == -6)
                return true;
        }
    }

    // Check if the square is under attack by a black bishop or queen
    for (int i = 1; x + i < boardSize && y + i < boardSize; i++)
    {
        if (board[y + i][x + i] == -5 || board[y + i][x + i] == -4)
            return true;
        if (board[y + i][x + i] != 0)
            break;
    }
    for (int i = 1; x - i >= 0 && y + i < boardSize; i++)
    {
        if (board[y + i][x - i] == -5 || board[y + i][x - i] == -4)
            return true;
        if (board[y + i][x - i] != 0)
            break;
    }

    // Check if the square is under attack by a black rook or queen
    for (int i = x + 1; i < boardSize; i++)
    {
        if (board[y][i] == -3 || board[y][i] == -4)
            return true;
        if (board[y][i] != 0)
            break;
    }
    for (int i = x - 1; i >= 0; i--)
    {
        if (board[y][i] == -3 || board[y][i] == -4)
            return true;
        if (board[y][i] != 0)
            break;
    }
    for (int i = y + 1; i < boardSize; i++)
    {
        if (board[i][x] == -3 || board[i][x] == -4)
            return true;
        if (board[i][x] != 0)
            break;
    }
    for (int i = y - 1; i >= 0; i--)
    {
        if (board[i][x] == -3 || board[i][x] == -4)
            return true;
        if (board[i][x] != 0)
            break;
    }

    // If no attacking pieces were found, return false
    return false;
}

bool isSquareUnderAttackB(int x, int y)
{
    // Check if the square is under attack by a white pawn
    if ((x > 0 && y > 0) && (board[y - 1][x - 1] == 1 || board[y - 1][x + 1] == 1))
        return true;

    // Check if the square is under attack by a white knight
    if (x > 1 && y > 1)
    {
        if (board[y - 2][x - 1] == 2 || board[y - 2][x + 1] == 2)
            return true;
    }
    if (x < boardSize - 2 && y > 1)
    {
        if (board[y - 2][x - 1] == 2 || board[y - 2][x + 1] == 2)
            return true;
    }
    if (x > 0 && y > 2)
    {
        if (board[y - 1][x - 2] == 2 || board[y - 1][x + 2] == 2)
            return true;
    }
    if (x < boardSize - 3 && y > 0)
    {
        if (board[y - 1][x - 2] == 2 || board[y - 1][x + 2] == 2)
            return true;
    }

    // Check if the square is under attack by a white king
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            if (x + i >= 0 && x + i < boardSize && y + j >= 0 && y + j < boardSize && board[y + j][x + i] == 6)
                return true;
        }
    }

    // Check if the square is under attack by a white bishop or queen
    for (int i = 1; x + i < boardSize && y + i < boardSize; i++)
    {
        if (board[y + i][x + i] == 5 || board[y + i][x + i] == 4)
            return true;
        if (board[y + i][x + i] != 0)
            break;
    }
    for (int i = 1; x - i >= 0 && y + i < boardSize; i++)
    {
        if (board[y + i][x - i] == 5 || board[y + i][x - i] == 4)
            return true;
        if (board[y + i][x - i] != 0)
            break;
    }

    // Check if the square is under attack by a white rook or queen
    for (int i = x + 1; i < boardSize; i++)
    {
        if (board[y][i] == 3 || board[y][i] == 4)
            return true;
        if (board[y][i] != 0)
            break;
    }
    for (int i = x - 1; i >= 0; i--)
    {
        if (board[y][i] == 3 || board[y][i] == 4)
            return true;
        if (board[y][i] != 0)
            break;
    }
    for (int i = y + 1; i < boardSize; i++)
    {
        if (board[i][x] == 3 || board[i][x] == 4)
            return true;
        if (board[i][x] != 0)
            break;
    }
    for (int i = y - 1; i >= 0; i--)
    {
        if (board[i][x] == 3 || board[i][x] == 4)
            return true;
        if (board[i][x] != 0)
            break;
    }

    // If no attacking pieces were found, return false
    return false;
}

bool validCastlingW(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x1 != 4 || y1 != 7 || y2 != 7 || (x2 != 6 && x2 != 2))
        return false;

    // Check if the king has already moved
    if (hasKingMovedW)
        return false;

    // Check if the rook has already moved
    if ((x2 == 6 && whiteRightRookMoved) || (x2 == 2 && whiteLeftRookMoved))
        return false;

    // Check if there are any pieces between the king and the rook
    if (x2 == 6)
    {
        for (int i = x1 + 1; i < x2; i++)
        {
            if (board[y1][i] != 0)
                return false; // Path is blocked
        }
    }
    else if (x2 == 2)
    {
        for (int i = x1 - 1; i > x2; i--)
        {
            if (board[y1][i] != 0)
                return false; // Path is blocked
        }
    }

    // Check if the king is not in check, not passing through check, and not landing in check
    if (isWhiteKingInCheck || isSquareUnderAttackW(x1, y1) || isSquareUnderAttackW(x2, y2))
        return false;

    // If all conditions are met, castling is valid
    return true;
}

bool validCastlingB(int x1, int y1, int x2, int y2)
{
    // Ensure the move is within the bounds of the board
    if (x1 != 4 || y1 != 0 || y2 != 0 || (x2 != 6 && x2 != 2))
        return false;

    // Check if the king has already moved
    if (hasKingMovedB)
        return false;

    // Check if the rook has already moved
    if ((x2 == 6 && blackRightRookMoved) || (x2 == 2 && blackLeftRookMoved))
        return false;

    // Check if there are any pieces between the king and the rook
    if (x2 == 6)
    {
        for (int i = x1 + 1; i < x2; i++)
        {
            if (board[y1][i] != 0)
                return false; // Path is blocked
        }
    }
    else if (x2 == 2)
    {
        for (int i = x1 - 1; i > x2; i--)
        {
            if (board[y1][i] != 0)
                return false; // Path is blocked
        }
    }

    // Check if the king is not in check, not passing through check, and not landing in check
    if (isBlackKingInCheck || isSquareUnderAttackB(x1, y1) || isSquareUnderAttackB(x2, y2))
        return false;

    // If all conditions are met, castling is valid
    return true;
}

#endif // MYHEADER_H
