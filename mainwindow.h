#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimeLine>

namespace Ui
{
    class MainWindow;
}

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
    void onTimeLineFinished();
};

#endif // MAINWINDOW_H
