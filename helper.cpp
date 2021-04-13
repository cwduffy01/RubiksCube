#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include "helper.h"

using std::string; using std::cout; using std::endl; using std::fabs; 
using std::rand; using std::srand; using std::time; using std::swap;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VECTOR IMPLEMENTATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

std::ostream &operator << (std::ostream &os, const Vector &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PIECE IMPLEMENTATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// constructor
Piece::Piece() : targetPos({0, 0, 0}), currentPos({0, 0, 0}), colors("") {}
Piece::Piece(const Vector tPos, Vector cPos, string c) : targetPos(tPos), currentPos(cPos), colors(c) {}

// getters and setters
Vector Piece::getTargetPos() {return targetPos;}
Vector Piece::getCurrentPos() {return currentPos;}
string Piece::getColors() {return colors;}
void Piece::setTargetPos(Vector tPos) {targetPos = tPos;}
void Piece::setCurrentPos(Vector cPos) {currentPos = cPos;}
void Piece::setColors(string c) {colors = c;}

/**
 * The cost of a piece is the amount of moves it would take to 
 * return a piece to its targetPos as a function of the dot 
 * product between currentPos and targetPos (which is somewhat 
 * proportional to the angle between the vectors)
 * @return  the amount of moves to make currentPos equal to targetPos
 */

int Piece::cost() {
    int dotProduct = (targetPos.x * currentPos.x) 
                        + (targetPos.y * currentPos.y) 
                        + (targetPos.z * currentPos.z);
    
    switch (dotProduct) {
        case 1:
            return 1;
        
        case 0:
        case -1:
            return 2;

        case -2:
        case -3:
            return 3;

        default:
            return 0;
    }
}

std::ostream &operator << (std::ostream &os, const Piece &p) {
    os << p.colors;
    return os;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CUBE IMPLEMENTATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * Constructs a solved Cube (cost is 0)
 */
Cube::Cube() : pieceArr(new Piece**[3] {}) {
    // initialize empty 3D array
    for (int i = 0; i < 3; i++) {
        pieceArr[i] = new Piece*[3] {};
        for (int j = 0; j < 3; j++) {
            pieceArr[i][j] = new Piece[3] {};
        }
    }

    // access each piece and set its initial values
    for (int i = -1; i < 3 - 1; i++) {
        for (int j = -1; j < 3 - 1; j++) {
            for (int k = -1; k < 3 - 1; k++) {
                Piece* p = &pieceArr[i+1][j+1][k+1];

                string c = "";
                if (i == -1) {c += 'B';}
                else if (i == 1) {c +=  'G';}
                if (j == -1) {c += 'O';}
                else if (j == 1) {c +=  'R';}
                if (k == -1) {c += 'Y';}
                else if (k == 1) {c +=  'W';}

                p->setTargetPos({i, j, k});
                p->setCurrentPos({i, j, k});
                p->setColors(c);
            }
        }
    }
    srand((unsigned) time(0));  // seed for random integer generator
}

Cube::Cube(const Cube& other) : pieceArr(new Piece**[3] {}) {   // copy constructor
    for (int i = 0; i < 3; i++) {
        pieceArr[i] = new Piece*[3] {};
        for (int j = 0; j < 3; j++) {
            pieceArr[i][j] = new Piece[3] {};
            for (int k = 0; k < 3; k++) {
                pieceArr[i][j][k] = other.pieceArr[i][j][k];
            }
        }
    }
}

Piece*** Cube::getPieceArr() {return pieceArr;}     // getter

Cube::~Cube() {     // destructor
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            delete[] pieceArr[i][j];
        }
        delete[] pieceArr[i];
    }
    delete[] pieceArr;
}

/**
 * Updates the positions of each Piece in pieceArr based on
 * its value of currentPos (called after each rotate function)
 * @param pieces    the Piece array to update
 * @param size      the size of pieces
 */
void Cube::rotate(Piece* pieces, int size) {
    for (int i = 0; i < size; i++) {
        Vector cPos = pieces[i].getCurrentPos();
        pieceArr[cPos.x + 1][cPos.y + 1][cPos.z + 1] = pieces[i];
    }
}

/**
 * Accesses a Piece of the cube based on its current position
 * @param vec   location of the Piece
 * @return      the Piece at location vec
 */
Piece Cube::getPiece(Vector v) {
    return pieceArr[v.x + 1][v.y + 1][v.z + 1];
}

/**
 * Rotate the front side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::front(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
            // update current position of each piece on the front side
            Piece p = getPiece({1, j, k});
            Vector v = p.getCurrentPos();

            if (twice) {v.y = -v.y; v.z = -v.z;}                // negate y and z
            else if (inverse) {v.z = -v.z; swap(v.y, v.z);}     // negate z, swap y and z
            else {v.y = -v.y; swap(v.y, v.z);}                  // negate y, swap y and z
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Rotate the back side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::back(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
            // update current position of each piece on the back side
            Piece p = getPiece({-1, j, k});
            Vector v = p.getCurrentPos();

            if (twice) {v.y = -v.y; v.z = -v.z;}                // negate y and z
            else if (inverse) {v.y = -v.y; swap(v.y, v.z);}     // negate y, swap y and z
            else {v.z = -v.z; swap(v.y, v.z);}                  // negate z, swap y and z
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Rotate the right side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::right(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int i = -1; i < 2; i++) {
        for (int k = -1; k < 2; k++) {
            // update current position of each piece on the right side
            Piece p = getPiece({i, 1, k});
            Vector v = p.getCurrentPos();

            if (twice) {v.x = -v.x; v.z = -v.z;}                // negate x and z
            else if (inverse) {v.x = -v.x; swap(v.x, v.z);}     // negate x, swap x and z
            else {v.z = -v.z; swap(v.x, v.z);}                  // negate z, swap x and z
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Rotate the left side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::left(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int i = -1; i < 2; i++) {
        for (int k = -1; k < 2; k++) {
            // update current position of each piece on the left side
            Piece p = getPiece({i, -1, k});
            Vector v = p.getCurrentPos();

            if (twice) {v.x = -v.x; v.z = -v.z;}                // negate x and z
            else if (inverse) {v.z = -v.z; swap(v.x, v.z);}     // negate z, swap x and z
            else {v.x = -v.x; swap(v.x, v.z);}                  // negate x, swap x and z
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Rotate the up side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::up(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            // update current position of each piece on the up side
            Piece p = getPiece({i, j, 1});
            Vector v = p.getCurrentPos();

            if (twice) {v.x = -v.x; v.y = -v.y;}                // negate x and y
            else if (inverse) {v.y = -v.y; swap(v.x, v.y);}     // negate y, swap x and y
            else {v.x = -v.x; swap(v.x, v.y);}                  // negate x, swap x and y
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Rotate the down side of the cube clockwise
 * @param inverse   true if counterclockwise instead
 * @param twice     true if rotate twice instead
 */
void Cube::down(bool inverse, bool twice) {
    Piece* pieces = new Piece[9] {};
    int ind = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            // update current position of each piece on the down side
            Piece p = getPiece({i, j, -1});
            Vector v = p.getCurrentPos();

            if (twice) {v.x = -v.x; v.y = -v.y;}                // negate x and y
            else if (inverse) {v.x = -v.x; swap(v.x, v.y);}     // negate x, swap x and y
            else {v.y = -v.y; swap(v.x, v.y);}                  // negate y, swap x and y
            p.setCurrentPos(v);

            pieces[ind] = p;
            ind++;
        }
    }  
    
    rotate(pieces, 9);    // update changed pieces in array
    delete[] pieces;
}

/**
 * Calculates the overall cost for the Cube as the sum of the 
 * cost of each individual piece
 * @return  the cost of the Cube
 */
int Cube::cost() {
    int costSum = 0;
    for (int i = -1; i < 3 - 1; i++) {
        for (int j = -1; j < 3 - 1; j++) {
            for (int k = -1; k < 3 - 1; k++) {
                // if edge or corner (centers stay in place)
                if (fabs(i) + fabs(j) + fabs(k) >= 2) {
                    costSum += this->getPiece({i, j, k}).cost();
                }
            }
        }
    }
    return costSum;
}

/**
 * Scrambles the Cube
 * @param moves     the amount of moves in the scramble
 * @return          the algorithm used to scramble
 */
string Cube::scramble(int moves) {
    string algorithm = "";
    int lastMove = -1;          // used to prevent rotating the same side twice in a row
    int lastLastMove = -1;      // used to prevent rotating opposite sides more than twice in a row
    
    for (int i = 0; i < moves; i++) {
        int move;
        do {
            move = rand() % 6;  // generates one of six moves
        } while (move == lastMove || move == lastLastMove);
        
        // if move and lastMove are opposite, prevent either side from moving
        if (move / 2 * 2 == lastMove / 2 * 2) {lastLastMove = lastMove;}
        else {lastLastMove = -1;}   // otherwise only track last move
        lastMove = move;

        bool twice = bool((rand() % 3) / 2);    // 33% chance of rotating a side twice
        bool inverse = bool(rand() % 2);    // 33% chance of rotating a side counter-clockwise (disregarded if twice=true)

        switch (move) {     // rotate side
            case 0:
                front(inverse, twice);
                algorithm += 'F';
                break;

            case 1:
                back(inverse, twice);
                algorithm += 'B';
                break;

            case 2:
                up(inverse, twice);
                algorithm += 'U';
                break;

            case 3:
                down(inverse, twice);
                algorithm += 'D';
                break;

            case 4:
                right(inverse, twice);
                algorithm += 'R';
                break;

            default:
                left(inverse, twice);
                algorithm += 'L';
        }

        if (twice) {algorithm += '2';}
        else if (inverse) {algorithm += '\'';}
        algorithm += ' ';
    }
    return algorithm;
}

string Cube::solve() {
    return "";
}

// overloaded = operator for Cube
Cube& Cube::operator= (const Cube& other) {
    Cube temp(other);
    swap(*this, temp);
    return *this;
}

std::ostream &operator << (std::ostream &os, Cube &c) {
    for (int i = -1; i < 3 - 1; i++) {
        // indicate which side is being printed
        if (i == -1) {cout << "Back: " << endl;}
        else if (i == 0) {cout << "Middle: " << endl;}
        else {cout << "Front: " << endl;}

        for (int k = 3 - 2; k > -2 ; k--) {
            for (int j = -1; j < 3 - 1; j++) {
                // print out Piece's color; string is length 3
                // color is centered if center, padded left if edge
                string colors = c.getPiece({i, j, k}).getColors();
                int leftPad = (3 - colors.length()) / 2;
                int rightPad = 3 - leftPad - colors.length();
                colors.append(rightPad, ' ');
                colors.insert(colors.begin(), leftPad, ' ');
                os << colors << ' ';
            }
            if (k != -1 || i != 1) {os << endl;}
        }
        if (i != 1) {os << endl;}
    }
    return os;
}
