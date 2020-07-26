#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QApplication>
#include <QWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

class Widget : public QLabel
{
    Q_OBJECT

    enum StateType{
        Stay,
        Running,
        Move,
        Climbing,
        Falling,
        Catch,  // 被鼠标抓住
    };

    enum ClimbingType{
        NoClimbing,
        ClimbingLeft,
        ClimblingRight,
        ClimbingTop
    };

    Q_ENUM(StateType)
    Q_ENUM(ClimbingType)

public:
    Widget(QWidget *parent = 0);
    void timerEvent(QTimerEvent *event);
    int getRandNum(int randNum);
    void setCurrentState();

    ~Widget();

private:
    int count = 1;
    StateType mStateType;
    ClimbingType mClimbingType;
    QTimer *mRuningTimer;
    QTimer *mMoveTimer;
    QTimer *mClimbingTimer;
    QTimer *mFallingTimer;

};

#endif // WIDGET_H
