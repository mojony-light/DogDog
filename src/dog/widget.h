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
        ClimbingMove,
        ClimbingLeft,
        ClimblingRight,
        ClimbingTop
    };

    enum StayType{
        NoStay,
        StayStanding,
        StayLying,
        StayMusic,
        StayLove
    };

    enum CatchType{
        NoCatch,
        leftCatch,
        rightCatch
    };

    Q_ENUM(StateType)
    Q_ENUM(ClimbingType)
    Q_ENUM(StayType)
    Q_ENUM(CatchType)

public:
    Widget(QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *);
    int getRandNum(int randNum);
    bool setCurrentState(StateType state);

    ~Widget();

private:
    int count = 1;
    int childTimerDurationSum = 0;  // 子计时器总共的时间
    int childTimeCount = 0;  // 子计时器叠加的时间
    int timerDuration = 0;  // 主计时器经历的时间
    QRect availableRect;  // 可用的区域
    QPoint moveTargetPos;  // 移动的目标坐标
    StateType mStateType;
    ClimbingType mClimbingType;
    StayType mStayType;
    CatchType mCatchType;

    QTimer *mStayTimer;
    QTimer *mRuningTimer;
    QTimer *mMoveTimer;
    QTimer *mClimbingTimer;
    QTimer *mFallingTimer;

};

#endif // WIDGET_H
