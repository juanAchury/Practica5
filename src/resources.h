#ifndef RESOURCES_H
#define RESOURCES_H
#include <QString>
#include <QPoint>
#include <QSize>
#include <QColor>
#include <QUrl>

class Resources
{
public:
    Resources();

    enum class Direction
    {
        Up = 1,
        Down = -1,
        Left = 2,
        Right = -2,
        Unset = 0
    };

    constexpr static const QSize RESOLUTION = QSize(448, 596);
    constexpr static const QColor BG_COLOR = QColor(0, 0, 0);
    constexpr static const QColor FONT_COLOR = QColor(255, 255, 255);

    static void loadLabyrinth();

    static const QString PATH_TO_LABYRINTH_PIXMAP;   // caminos del laberinto
    constexpr static const unsigned int LABYRINTH_TILE_SIZE = 16;

    static const QString PATH_TO_THINGS_PIXMAP;
    constexpr static const unsigned int THINGS_TILE_SIZE = 30;

    static const QString PATH_TO_FONT;

    constexpr static const unsigned int PACMAN_COUNT_ANIM_FRAMES = 3;
    constexpr static const QPoint PACMAN_UP = QPoint(0, 0);
    constexpr static const QPoint PACMAN_DOWN = QPoint(90, 0);
    constexpr static const QPoint PACMAN_LEFT = QPoint(180, 0);
    constexpr static const QPoint PACMAN_RIGHT = QPoint(270, 0);






    constexpr static int FPS = 120;



};

#endif // RESOURCES_H
