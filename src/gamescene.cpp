#include "gamescene.h"
#include "resources.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>
#include <QFontDatabase>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
      m_deltaTime(0.0f),
      m_loopTime(0.0f),
      m_loopSpeed(int(1000.0f/Resources::FPS) )
{
    srand(time(0));
    loadPixmap();
    setSceneRect(0,0, Resources::RESOLUTION.width(), Resources::RESOLUTION.height());
    setBackgroundBrush(QBrush(Resources::BG_COLOR));
    initLabyrinth();
    initGUI();
    initPackman();


    renderLabyrinth();
    renderPacman();
    renderGUI();


    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/Resources::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    if( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        if (pacmanCanMove())
        {
            m_pacman->move();
            m_pacman->setPos(m_pacman->getScreenPosX(), m_pacman->getScreenPosY());        }
        else
        {
            m_pacman->stop();
        }

        if(m_labyrinthObj.isIntersection(m_pacman->getTileX(), m_pacman->getTileY()))
        {
            m_pacman->stop();
        }
        if(m_labyrinthObj.removeDot(m_pacman, m_score))

        renderLabyrinth();
        teleportTunnels(m_pacman);



    }
}


void GameScene::loadPixmap()
{
    if( m_labyrinthPixmap.load(Resources::PATH_TO_LABYRINTH_PIXMAP) )
    {
        qDebug() << "Labyrinth is loaded SUCCESSFULLY";
    }
    else
    {
        qFatal("Labyrinth is loaded SUCCESSFULLY");
    }





}



void GameScene::initLabyrinth()
{
    int index = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_labyrinthPixmaps[index] = m_labyrinthPixmap.copy(i * Resources::LABYRINTH_TILE_SIZE, j * Resources::LABYRINTH_TILE_SIZE, Resources::LABYRINTH_TILE_SIZE, Resources::LABYRINTH_TILE_SIZE);
            index++;
        }
    }
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        for (int j = 0; j < int(Labyrinth::LABYRINTH_HEIGHT); j++)
        {
            m_labyrinthPixmapItems[i][j] =  new QGraphicsPixmapItem();
            m_labyrinthPixmapItems[i][j]->setPixmap(m_labyrinthPixmaps[ m_labyrinthObj.tiles(i, j) ]);
            m_labyrinthPixmapItems[i][j]->setPos(i*Resources::LABYRINTH_TILE_SIZE, j*Resources::LABYRINTH_TILE_SIZE);
            addItem(m_labyrinthPixmapItems[i][j]);
        }
    }

#ifdef QT_DEBUG
    QPen linePen((QColor(Qt::white)));
    linePen.setWidth(1);
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem();
        lineItem->setLine(i*Resources::LABYRINTH_TILE_SIZE, 0,
                          i*Resources::LABYRINTH_TILE_SIZE, Resources::RESOLUTION.height());
        lineItem->setPen(linePen);
        addItem(lineItem);
    }
    for (int i = 0; i < int(Labyrinth::LABYRINTH_HEIGHT); i++)
    {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem();
        lineItem->setLine(0, i * Resources::LABYRINTH_TILE_SIZE,
                          Resources::RESOLUTION.width(), i * Resources::LABYRINTH_TILE_SIZE);
        lineItem->setPen(linePen);
        addItem(lineItem);
    }

#endif
}

void GameScene::initPackman()
{
    m_pacman = new Pacman();
    m_pacman->setFocus();
    m_pacman->setPos(m_pacman->getScreenPosX(), m_pacman->getScreenPosY());
}





void GameScene::initGUI()
{
    int id = QFontDatabase::addApplicationFont(Resources::PATH_TO_FONT);
    m_basicFont = QFont(QFontDatabase::applicationFontFamilies(id).at(0), 22 , 0);

    m_scoreTextItem = new QGraphicsSimpleTextItem();
    m_scoreTextItem->setBrush(QBrush(Resources::FONT_COLOR));
    m_scoreTextItem->setPen(QPen(Resources::FONT_COLOR));
    m_scoreTextItem->setPos(12*Resources::LABYRINTH_TILE_SIZE, 1*Resources::LABYRINTH_TILE_SIZE);
    m_scoreTextItem->setFont(m_basicFont);
    m_scoreTextItem->setText("Score: " + QString::number(m_score).right(5));


}

void GameScene::renderLabyrinth()
{
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        for (int j = 0; j < int(Labyrinth::LABYRINTH_HEIGHT); j++)
        {
            m_labyrinthPixmapItems[i][j]->setPixmap(m_labyrinthPixmaps[ m_labyrinthObj.tiles(i, j) ]);
        }
    }
}

void GameScene::renderPacman()
{
    addItem(m_pacman);
}

void GameScene::renderGUI()
{
    addItem(m_scoreTextItem);



}






void GameScene::saveScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

bool GameScene::pacmanCanMove()
{
    if (!m_pacman->getDirections().empty())
    {
        switch (m_pacman->getDirections().front())
        {
        case Resources::Direction::Up:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() - 1);
            break;
        case Resources::Direction::Down:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() + 1);
            break;
        case Resources::Direction::Left:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() - 1, m_pacman->getTileY());
            break;
        case Resources::Direction::Right:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() + 1, m_pacman->getTileY());
            break;
        }
    }
    return true;
}

void GameScene::teleportTunnels(Entity* entity)
{
    if (entity->getTileX() == 0 && entity->getTileY() == 17)
        entity->teleport(26, 17);
    else if (entity->getTileX() == 27 && entity->getTileY() == 17)
        entity->teleport(1, 17);
}

void GameScene::addPoints(int n)
{
    m_score += n;
}

void GameScene::updateGUI()
{
    m_scoreTextItem->setText("Score: " + QString::number(m_score).right(5));


}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Z:
        //saveScene();
        break;
    }

    QGraphicsScene::keyPressEvent(event);
}
