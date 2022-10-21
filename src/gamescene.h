#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "labyrinth.h"
#include "pacman.h"
#include <QPixmap>
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QSoundEffect>

class QGraphicsPixmapItem;
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:
private slots:
    void loop();

private:                                //inicializar los valores de objetos y variables.
    void loadPixmap();
    void initLabyrinth();
    void initPackman();
    void initGUI();

    void renderLabyrinth();
    void renderPacman();
    void renderGUI();
    void saveScene();

    bool pacmanCanMove();
    void teleportTunnels(Entity* entity);






    void addPoints(int n);
    void updateGUI();
    //Visualizar elementos del juego
    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];
    QGraphicsPixmapItem* m_labyrinthPixmapItems[Labyrinth::LABYRINTH_WIDTH][Labyrinth::LABYRINTH_HEIGHT];
    Labyrinth m_labyrinthObj;
    Pacman* m_pacman;
    //marco del cuadro
    int currentFrame;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
    //Interfaz grafica
    QFont m_basicFont;
    QGraphicsSimpleTextItem *m_scoreTextItem;
    QPixmap m_lifePacmanPixmap;
    int  m_score;


protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
