//modification of http://paste4btc.com/Lu9Cvpd9


#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
//using namespace std;
bool gameOver;

const int width = 20;
const int height = 20;

int x, y, score;


enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (T x: v) {
        os << x << " ";
    }
    return os;
}  

struct Point {
    int x;
    int y;

};

bool operator==(const Point& rhs, const Point& lhs) {
    return rhs.x == lhs.x && rhs.y == lhs.y;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

Point makeRandPoint() {
    return {rand() % width, rand() % height};
}


struct Tail {
    std::vector<Point> coords;
    void move(Point pos) {
        if (!coords.size()) {
            return;
        }
        Point prev = coords[0];
        Point prev2;
        coords[0] = pos;
        for (int i = 1; i < coords.size(); ++i) {
            prev2 = coords[i];
            coords[i] = prev;
            prev = prev2;
        }
    }
    bool has(Point p) {
        for (const auto& t: coords) {
            if (t == p) {
                return true;
            }
        }
        return false;
    }
    void increase() {
        coords.push_back({0, 0});
    }
};

Tail tail;

struct Object {
    Point pos_ = {0, 0};
    Object() {}
    Object(int x, int y): pos_{x, y} {

    }
    void move(Point pos) {
        pos_ = pos;
    }    
    bool at(int x, int y) {
        return pos_.x == x && pos_.y == y;
    }
};

struct Fruit: public Object {
};

Fruit fruit;

/*struct Head: public Object {
};

Head head;*/


void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    fruit.move(makeRandPoint());
    score = 0;
}
void Draw()
{
    system("cls"); //system("clear");
    for (int i = 0; i < width+2; i++)
        std::cout << "#";
    std::cout << std::endl;
 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                std::cout << "#";
            if (i == y && j == x)
                std::cout << "O";
            else if (fruit.at(j, i))
                std::cout << "F";
            else
            {
                if (tail.has({j, i})) {
                    std::cout << "o";
                }
                else {
                    std::cout << " ";
                }

            }
                 
            if (j == width - 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }
 
    for (int i = 0; i < width+2; i++)
        std::cout << "#";
    std::cout << std::endl;
    std::cout << "Score:" << score << std::endl;
}
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}
void Logic()
{

    tail.move({x, y});

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }
    //if (x > width || x < 0 || y > height || y < 0)
    //  gameOver = true;
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;
    

    if (tail.has({x, y})) {
        gameOver = true;
    }
 
    if (fruit.at(x, y))
    {
        score += 10;
        fruit.move(makeRandPoint());
        tail.increase();
    }
}
int main()
{
#if 1
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(10); //sleep(10);
    }
#endif 

#if 0
    Tail tail;
    tail.increase();
    tail.move({1, 1});
    std::cout << tail.coords << std::endl;
    tail.increase();
    tail.move({2, 2});
    std::cout << tail.coords << std::endl;
    tail.move({3, 2});
    std::cout << tail.coords << std::endl;
    tail.move({4, 2});
    std::cout << tail.coords << std::endl;
    srand(time(0));
    Fruit fruit1;
    fruit.move(makeRandPoint());
    std::cout << fruit.pos_;
#endif

    return 0;
}