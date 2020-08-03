#include "widget.h"

#define TIMERINTERVAL 50

Widget::Widget(QWidget *parent)
    : QLabel(parent),
      mStayTimer(new QTimer(this)),
      mRuningTimer(new QTimer(this)),
      mMoveTimer(new QTimer(this)),
      mClimbingTimer(new QTimer(this)),
      mFallingTimer(new QTimer(this))
{
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
//    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QImage image = QImage(QString("://img/shime%1.png").arg(count, 2, 10, QChar('0')));
    image = image.mirrored(true, false);
    image = image.scaled(image.size()*0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(QPixmap::fromImage(image));
    startTimer(300);

    connect(mStayTimer, &QTimer::timeout, this, &Widget::slotStayTimeOut);
    connect(mRuningTimer, &QTimer::timeout, this, &Widget::slotRuningTimeOut);
    connect(mMoveTimer, &QTimer::timeout, this, &Widget::slotMoveTimeOut);
    connect(mClimbingTimer, &QTimer::timeout, this, &Widget::slotClimbingTimeOut);
    connect(mFallingTimer, &QTimer::timeout, this, &Widget::slotFallingTimeOut);
}

// 主计时器
void Widget::timerEvent(QTimerEvent *)
{
//    qDebug() << getRandNum(10);
//    count = count == 1 ? 2 : 1;
//    QImage image = QImage(QString("://img/shime%1.png").arg(count, 2, 10, QChar('0')));
//    image = image.mirrored(true, false);
//    image = image.scaled(image.size()*0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    setPixmap(QPixmap::fromImage(image));
//    QRect availableRect = QDesktopWidget().availableGeometry();

//    if (availableRect.isEmpty())
//        return;

//    int m_x = x() + 5;

//    m_x = m_x > availableRect.width() ? -width() : m_x;

//    move(m_x ,y());

    childTimeCount += timerDuration;

    if (childTimeCount < childTimerDurationSum) {
        // 此时没有超过了当前的计时时间 直接返回
        return;
    }  // end if

    switch (mStateType) {
    case Stay:
        mStayTimer->stop();
        break;
    case Running:
        mRuningTimer->stop();
        break;
    case Move:
        mMoveTimer->stop();
        break;
    case Climbing:
        mClimbingTimer->stop();
        break;
    case Falling:
        mFallingTimer->stop();
        break;
    case Catch:
        break;
    }  // end switch

    // 随机一个当前状态
    while (true) {
        int ret = getRandNum(int(StateType::Catch) + 1);

        if (!setCurrentState(StateType(ret)))
            continue;
        break;
    }  // end while

}

// 获取随机时间
int Widget::getRandNum(int randNum)
{
    srand(quint32(QDateTime::currentDateTime().toMSecsSinceEpoch()));

    return rand() % randNum;
}

// 设置当前状态
bool Widget::setCurrentState(StateType state)
{
    switch (state) {
    case Stay: {
        // 当前是站着不动的状态
        if (mStateType == StateType::Climbing || mStateType == StateType::Falling ||
                mStateType == StateType::Catch)
            return false;

        mClimbingType = NoClimbing;
        mCatchType = NoCatch;
        StayType stayT = StayType(getRandNum(int(StayType::StayLove) + 1));

        switch (stayT) {
        case StayStanding: {
            mStayType = StayStanding;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            childTimerDurationSum = getRandNum(3000);
            mDirectionType = Direction(getRandNum(2));
            mStayTimer->start(TIMERINTERVAL);
            break;
        }

        case StayLying: {
            mStayType = StayLying;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            childTimerDurationSum = getRandNum(3000);
            mDirectionType = Direction(getRandNum(2));
            mStayTimer->start(TIMERINTERVAL);
            break;
        }

        case StayMusic: {
            mStayType = StayMusic;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            childTimerDurationSum = getRandNum(3000);
            mDirectionType = Direction(getRandNum(2));
            mStayTimer->start(TIMERINTERVAL);
            break;
        }
        case StayLove: {
            mStayType = StayLove;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            childTimerDurationSum = getRandNum(3000);
            mDirectionType = Direction(getRandNum(2));
            mStayTimer->start(TIMERINTERVAL);
            break;
        }
        default:
            return false;
    }
        break;
    }

    case Running: {
        if (mStateType == StateType::Climbing || mStateType == StateType::Falling ||
                mStateType == StateType::Catch)
            return false;

        // 当前正在奔跑
        mStateType = Running;
        mClimbingType = NoClimbing;
        mStayType = NoStay;
        mCatchType = NoCatch;

        // 随机奔跑时间
        childTimerDurationSum = getRandNum(3000);
        mDirectionType = Direction(getRandNum(2));
        mRuningTimer->start(TIMERINTERVAL);
        break;
    }
    case Move: {
        if (mStateType == StateType::Climbing || mStateType == StateType::Falling ||
                mStateType == StateType::Catch)
            return false;

        // 当前正在走路
        mStateType = Move;
        mClimbingType = NoClimbing;
        mStayType = NoStay;
        mCatchType = NoCatch;

        childTimerDurationSum = getRandNum(3000);
        mDirectionType = Direction(getRandNum(2));
        mMoveTimer->start(TIMERINTERVAL);
        break;
    }

    case Climbing: {
        if (mStateType == StateType::Falling || mStateType == StateType::Catch)
            return false;

        // 当前正在攀爬
        mStateType = Climbing;
        mStayType = NoStay;
        mCatchType = NoCatch;

        ClimbingType climbingT = ClimbingType(getRandNum(int(ClimbingType::ClimbingTop) + 1));

        switch (climbingT) {
        case ClimbingLeft: {
            // 窗口左边攀爬
            mClimbingType = ClimbingLeft;
            mDirectionType = Direction::Left;
            if (pos().x() != moveTargetPos.x()) {
                // 想要移动到窗口边缘进行攀爬
                mClimbingType = ClimbingMove;
                moveTargetPos = QPoint(availableRect.x(), availableRect.height()-height());
            }

            childTimerDurationSum = -1;
            mClimbingTimer->start(TIMERINTERVAL);
            break;
        }
        case ClimbingTop: {
            // 窗口上面攀爬
            if (mClimbingType == ClimbingLeft) {
                // 上一个状态是左边攀爬
                moveTargetPos = QPoint(availableRect.x(), availableRect.y());
                mDirectionType = Direction::Left;
            } else if (mClimbingType == ClimblingRight) {
                // 上一个状态是右边攀爬
                moveTargetPos = QPoint(availableRect.width()-width(), availableRect.y());
                mDirectionType = Direction::Right;
            } else if (mClimbingType == ClimbingMove) {
                // 上一个状态是正在地下移动
                moveTargetPos = QPoint(availableRect.x(), availableRect.height()-height());
                mDirectionType = Direction::Left;

            } else {
                // 上一个状态没有在攀爬，应该就在地下
                mClimbingType = ClimbingMove;
                moveTargetPos = QPoint(availableRect.x(), availableRect.height()-height());
                mDirectionType = Direction::Left;
            }

            childTimerDurationSum = -1;
            mClimbingTimer->start(TIMERINTERVAL);
            break;
        }
        case ClimblingRight: {
            // 窗口右边攀爬
            mClimbingType = ClimblingRight;
            mDirectionType = Direction::Right;
            if (pos().x() != moveTargetPos.x()) {
                // 想要移动到窗口边缘进行攀爬
                mClimbingType = ClimbingMove;
                moveTargetPos = QPoint(availableRect.width() - width(), availableRect.height()-height());
            }

            childTimerDurationSum = -1;
            mClimbingTimer->start(TIMERINTERVAL);
            break;
        }
        default:
            return false;
        }

        break;
    }
    default:
        return false; // 其他状态不被允许
    }
    return true;
}

Widget::~Widget()
{

}

void Widget::slotStayTimeOut()
{
    // 切换图片
    if (mStateType != Stay)
        return;

    QString curPath;
    QImage curImage;

    switch (mStayType) {
    case StayStanding: {
        // 保持站立姿势
        curPath = "qrc:/img/shime01.png";
        curImage = QImage(curPath);

        if (mDirectionType == Direction::Right)
            curImage.mirrored(true, false);

        break;
    }
    case StayMusic: {


        break;
    }
    case StayLying: {
        // 躺着
        curPath = "qrc:/img/shime20.png";

        if (getRandNum(100) + 1 < 15)  // 五分之一的概率发出感叹的表情
            curPath = "qrc:/img/shime21.png";

        curImage = QImage(curPath);

        if (mDirectionType == Direction::Right)
            curImage.mirrored(true, false);

        break;
    }


    }

    if ((childTimeCount += TIMERINTERVAL) >= childTimerDurationSum) {
        // 重置时间，并关闭计时器
        childTimerDurationSum = 0;
        childTimeCount = 0;
        mStayTimer->stop();
    }
}

void Widget::slotRuningTimeOut()
{

}

void Widget::slotMoveTimeOut()
{

}

void Widget::slotClimbingTimeOut()
{

}

void Widget::slotFallingTimeOut()
{

}
