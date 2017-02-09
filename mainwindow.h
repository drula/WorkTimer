#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimeLine>
#include <QTime>

namespace Ui
{
    class MainWindow;
}

//! temporary values, should be taken from settings
const QTime defaultTotalTime = QTime(8, 0);
const QTime defaultWorkTime = QTime(6, 0);
const QTime defaultRestTime = QTime(2, 0);
const QTime defaultWorkSessionTime = QTime(0, 25);
const QTime defaultRestSessionTime = QTime(0, 5);

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class State
    {
        Initial,
        Work,
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *_ui;
    State _state;
    QTimeLine _timeLine;

    void setState(State state);
    void setDefaultValues();

private slots:
    void onStartFinishButton();
    void onTimeLineFrameChanged(int frame);
};

#endif // MAINWINDOW_H
