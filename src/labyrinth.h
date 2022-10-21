#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <QObject>

class Pacman;
class Labyrinth : public QObject
{
    Q_OBJECT
public:
    explicit Labyrinth();  //Constructos del laberinto

    static const uint LABYRINTH_WIDTH  = 28;  // Ancho del laberinto estatico
    static const uint LABYRINTH_HEIGHT = 36;  // Altura del laberinto estatico

    uint tiles(int x, int y) const;
    bool tileBlocksEntity(int x, int y);
    bool isIntersection(int x, int y);
    bool removeDot(Pacman* pacman, int& score);
signals:

private:
    uint m_tiles[LABYRINTH_WIDTH][LABYRINTH_HEIGHT];  // Le doy los valores para el laberinto
    static constexpr int NONE_FIELD = 30;
    static constexpr int SMALL_DOT_FIELD = 26;

};

#endif // LABYRINTH_H
