#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <random>
#include <QPainter>
#include <QStyleOption>
#include <QPropertyAnimation>
#include <QScreen>
#include <QEvent>
#include <QTimer>

// 圆形按钮类
class RoundButton : public QPushButton {
    Q_OBJECT
public:
    RoundButton(const QString &text, QWidget *parent = nullptr) 
        : QPushButton(text, parent) {
        setFixedSize(60, 60);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QStyleOptionButton option;
        initStyleOption(&option);
        painter.setBrush(option.palette.button());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());
        painter.setPen(option.palette.buttonText().color());
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
};

// 箭头指示器类
class ArrowIndicator : public QWidget {
    Q_OBJECT
public:
    ArrowIndicator(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(20, 60);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
    }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(100, 100, 100, 180));
        painter.setPen(Qt::NoPen);
        
        // 绘制圆角矩形背景
        painter.drawRoundedRect(rect(), 3, 3);
        
        // 绘制箭头
        painter.setBrush(Qt::white);
        QPolygonF arrow;
        arrow << QPointF(5, height()/2) 
              << QPointF(15, height()/2 - 8)
              << QPointF(15, height()/2 + 8);
        painter.drawPolygon(arrow);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
    }
};

class RandomNumberApp : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int hiddenOffset READ hiddenOffset WRITE setHiddenOffset)

public:
    RandomNumberApp(QWidget *parent = nullptr) : QWidget(parent), m_hiddenOffset(0) {
        m_rng.seed(QTime::currentTime().msec());
        setupUI();
        
        // 初始化箭头指示器
        m_arrow = new ArrowIndicator();
        m_arrow->hide();
        connect(m_arrow, &ArrowIndicator::clicked, this, &RandomNumberApp::showFromSide);

        // 初始化隐藏位置
        m_screenWidth = QGuiApplication::primaryScreen()->availableGeometry().width();
    }

    ~RandomNumberApp() {
        delete m_arrow;
    }

    int hiddenOffset() const { return m_hiddenOffset; }
    void setHiddenOffset(int offset) {
        m_hiddenOffset = offset;
        move(m_screenWidth - width() + offset, y());
    }

protected:
    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::WindowStateChange) {
            if (isMinimized()) {
                hideToSide();
                event->ignore();
            }
        }
        QWidget::changeEvent(event);
    }

    void moveEvent(QMoveEvent *event) override {
        // 更新箭头位置
        if (m_arrow->isVisible()) {
            m_arrow->move(m_screenWidth - m_arrow->width(), y() + (height() - m_arrow->height())/2);
        }
        QWidget::moveEvent(event);
    }

    void hideEvent(QHideEvent *event) override {
        if (!isHiddenToSide) {
            m_arrow->hide();
        }
        QWidget::hideEvent(event);
    }

private slots:
    void generateRandomNumber() {
        std::uniform_int_distribution<int> dist(1, 47);
        label->setText(QString::number(dist(m_rng)));
    }

    void showFromSide() {
        showNormal();
        animateShow();
    }

private:
    void setupUI() {
        RoundButton *button = new RoundButton("生成", this);
        label = new QLabel("点击按钮生成随机数", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px;");

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(label, 1);
        layout->addWidget(button);
        layout->setSpacing(20);
        layout->setContentsMargins(20, 20, 20, 20);

        connect(button, &QPushButton::clicked, this, &RandomNumberApp::generateRandomNumber);

        setWindowTitle("随机数生成器 (MT19937)");
        setFixedSize(300, 100);
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
    }

    void hideToSide() {
        isHiddenToSide = true;
        
        // 设置箭头位置并显示
        m_arrow->move(m_screenWidth - m_arrow->width(), y() + (height() - m_arrow->height())/2);
        m_arrow->show();
        
        QPropertyAnimation *animation = new QPropertyAnimation(this, "hiddenOffset");
        animation->setDuration(300);
        animation->setStartValue(0);
        animation->setEndValue(width() - 2); // 保留2px可见
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void animateShow() {
        isHiddenToSide = false;
        m_arrow->hide();
        
        QPropertyAnimation *animation = new QPropertyAnimation(this, "hiddenOffset");
        animation->setDuration(300);
        animation->setStartValue(hiddenOffset());
        animation->setEndValue(0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QLabel *label;
    ArrowIndicator *m_arrow;
    std::mt19937 m_rng;
    int m_screenWidth;
    int m_hiddenOffset;
    bool isHiddenToSide = false;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RandomNumberApp window;
    window.show();
    return app.exec();
}

#include "main.moc"