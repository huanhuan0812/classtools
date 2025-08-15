#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <random>
#include <QPainter>
#include <QStyleOption>

// 圆形按钮类
class RoundButton : public QPushButton {
    Q_OBJECT
public:
    RoundButton(const QString &text, QWidget *parent = nullptr) 
        : QPushButton(text, parent) {
        setFixedSize(60, 60);  // 固定大小
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制圆形背景
        QStyleOptionButton option;
        initStyleOption(&option);
        painter.setBrush(option.palette.button());
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect());

        // 绘制文字
        painter.setPen(option.palette.buttonText().color());
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
};

class RandomNumberApp : public QWidget {
    Q_OBJECT

public:
    RandomNumberApp(QWidget *parent = nullptr) : QWidget(parent) {
        // 初始化随机数引擎
        m_rng.seed(QTime::currentTime().msec());
        setupUI();
    }

private slots:
    void generateRandomNumber() {
        // 生成1到47的随机数
        std::uniform_int_distribution<int> dist(1, 47);
        int randomNumber = dist(m_rng);
        label->setText(QString::number(randomNumber));
    }

private:
    void setupUI() {
        // 创建圆形按钮和标签
        RoundButton *button = new RoundButton("生成", this);
        label = new QLabel("点击按钮生成随机数", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-size: 18px;");

        // 设置横向布局
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(label, 1);  // 标签占更多空间
        layout->addWidget(button);
        layout->setSpacing(20);
        layout->setContentsMargins(20, 20, 20, 20);

        // 连接信号和槽
        connect(button, &QPushButton::clicked, this, &RandomNumberApp::generateRandomNumber);

        // 设置窗口属性
        setWindowTitle("随机数生成器 (MT19937)");
        setFixedSize(300, 100);  // 固定窗口大小
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);  // 保持在最上层
    }

    QLabel *label;
    std::mt19937 m_rng;  // Mersenne Twister 19937
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RandomNumberApp window;
    window.show();
    return app.exec();
}

#include "main.moc"