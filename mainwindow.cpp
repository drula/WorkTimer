#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _timeLine.setCurveShape(QTimeLine::LinearCurve);

    bool connected = false;

    connected = connect(_ui->startFinishButton, &QPushButton::clicked, this, &MainWindow::onStartFinishButton);
    Q_ASSERT(connected);

    connected = connect(&_timeLine, &QTimeLine::frameChanged, this, &MainWindow::onTimeLineFrameChanged);
    Q_ASSERT(connected);

    setState(State::Initial);
}

void MainWindow::setState(State state)
{
    switch (state) {
    case State::Initial:
        _ui->totalTimeEdit->setEnabled(true);
        _ui->workTimeEdit->setEnabled(true);
        _ui->restTimeEdit->setEnabled(true);

        _ui->startFinishButton->setEnabled(true);
        _ui->startFinishButton->setText("Start");

        _ui->sessionTimeEdit->setEnabled(false);
        _ui->workRestButton->setEnabled(false);
        _ui->workRestButton->setText("Work");
        _ui->nextSessionTimeEdit->setEnabled(true);

        setDefaultValues();

        break;

    case State::Work: {
        _ui->startFinishButton->setText("Finish");

        _ui->sessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());
        _ui->sessionTimeEdit->setEnabled(true);
        _ui->sessionTimeEdit->setReadOnly(true);

        _ui->workRestButton->setText("Rest");
        _ui->workRestButton->setEnabled(true);

        _ui->nextSessionLabel->setText("Next rest session time");
        _ui->nextSessionTimeEdit->setTime(defaultRestSessionTime);

        int msec = - _ui->sessionTimeEdit->time().msecsTo(QTime(0, 0));
        _timeLine.setDuration(msec);
        _timeLine.setFrameRange(0, msec / 1000);
        _timeLine.setUpdateInterval(1000);
        _timeLine.start();
    }

        break;

    default:
        break; //! message or exception in debug
    }

    _state = state;
}

void MainWindow::setDefaultValues()
{
    //! get values from settings
    _ui->totalTimeEdit->setTime(defaultTotalTime);
    _ui->workTimeEdit->setTime(defaultWorkTime);
    _ui->restTimeEdit->setTime(defaultRestTime);

    _ui->nextSessionTimeEdit->setTime(defaultWorkSessionTime);
}

void MainWindow::onStartFinishButton()
{
    if (_state == State::Initial)
        setState(State::Work);
}

void MainWindow::onTimeLineFrameChanged(int frame)
{
    _ui->sessionTimeEdit->setTime(_ui->sessionTimeEdit->time().addSecs(-1));
}

MainWindow::~MainWindow()
{
    delete _ui;
}
