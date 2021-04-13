#ifndef HELPER
#define HELPER

#include <iostream>
#include <string>

struct Vector {
    int x;
    int y;
    int z;
};

std::ostream &operator << (std::ostream &os, const Vector &v);


class Piece {
    private:
        Vector targetPos;
        Vector currentPos;
        std::string colors;
    public:
        Piece();
        Piece(const Vector, Vector, std::string);
        Vector getTargetPos();
        Vector getCurrentPos();
        std::string getColors();
        void setTargetPos(Vector);
        void setCurrentPos(Vector);
        void setColors(std::string c);

        int cost();

        friend std::ostream &operator << (std::ostream &os, const Piece &p);
};


class Cube {
    private:
        Piece*** pieceArr;
        void rotate(Piece*, int);
    public:
        int SIZE = 3;
        Cube();
        Cube(const Cube&);
        Piece*** getPieceArr();
        Piece getPiece(Vector);
        ~Cube();

        void right(bool inverse=false, bool twice=false);
        void left(bool inverse=false, bool twice=false);
        void up(bool inverse=false, bool twice=false);
        void down(bool inverse=false, bool twice=false);
        void front(bool inverse=false, bool twice=false);
        void back(bool inverse=false, bool twice=false);

        int cost();
        std::string scramble(int=20);  // returns the algorithm used to scramble it
        std::string solve();           // returns the algorithm found to solve it

        friend std::ostream &operator << (std::ostream &os, Cube &c);
        Cube& operator= (const Cube&);
};

#endif