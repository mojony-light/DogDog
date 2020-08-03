#include "widget.h"

Widget::Widget(QWidget *parent)
    : QLabel(parent)
{
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
//    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QImage image = QImage(QString("://img/shime%1.png").arg(count, 2, 10, QChar('0')));
    image = image.mirrored(true, false);
    image = image.scaled(image.size()*0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(QPixmap::fromImage(image));
    startTimer(300);
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
            int sduration = getRandNum(3000);
            childTimerDurationSum = sduration;

            mStayTimer->start();
            break;
        }

        case StayLying: {
            mStayType = StayLying;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            int lduration = getRandNum(3000);
            mStayTimer->start(lduration);
            break;
        }

        case StayMusic: {
            mStayType = StayMusic;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            int mduration = getRandNum(3000);
            mStayTimer->start(mduration);
            break;
        }
        case StayLove: {
            mStayType = StayLove;
            mStateType = Stay;

            // 设置发呆时间不超过3秒钟
            int lduration = getRandNum(3000);
            mStayTimer->start(lduration);
            break;
        }
        default:
            break;
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



        mRuningTimer->start();
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
        mMoveTimer->start();
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
            if (pos() != moveTargetPos) {
                // 想要移动到窗口边缘进行攀爬
                mClimbingType = ClimbingMove;
                moveTargetPos = QPoint(availableRect.x(), availableRect.height()-height());
            }

            mClimbingTimer->start()
        }

        }
        break;
    }

    case Falling:
        // 当前正在掉落
        mFallingTimer->start();
        return;
    default:
        break;
    }
}

Widget::~Widget()
{

}
