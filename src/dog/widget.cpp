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

void Widget::timerEvent(QTimerEvent *event)
{
//    qDebug() << getRandNum(10);
    count = count == 1 ? 2 : 1;
    QImage image = QImage(QString("://img/shime%1.png").arg(count, 2, 10, QChar('0')));
    image = image.mirrored(true, false);
    image = image.scaled(image.size()*0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(QPixmap::fromImage(image));
    QRect availableRect = QDesktopWidget().availableGeometry();

    if (availableRect.isEmpty())
        return;

    int m_x = x() + 5;

    m_x = m_x > availableRect.width() ? -width() : m_x;

    move(m_x ,y());


    //
}

int Widget::getRandNum(int randNum)
{
    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    return rand() % randNum;
}

void Widget::setCurrentState()
{
    switch (mStateType) {
    case Stay:
        // 当前时站着不动的状态
        setPixmap(":/img/shime01.png");
        return;

    case Running:
        // 当前正在奔跑
        mRuningTimer->start();
        return;
    case Move:
        // 当前正在走路
        mMoveTimer->start();
        return;
    case Climbing:
        // 当前正在攀爬
        mClimbingTimer->start();
        return;
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
