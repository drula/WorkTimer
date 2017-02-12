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

    connected = connect(&_timeLine, &QTimeLine::finished, this, &MainWindow::onTimeLineFinished);
    Q_ASSERT(connected);

    connected = connect(_ui->workRadioButton, &QRadioButton::clicked, this, &MainWindow::onWorkRadioButton);
    Q_ASSERT(connected);

    connected = connect(_ui->restRadioButton, &QRadioButton::clicked, this, &MainWindow::onRestRadioButton);
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
        _ui->startFinishButton->setText(Start);

        _ui->currentSessionTimeEdit->setEnabled(false);
        _ui->workRestButton->setEnabled(false);
        _ui->workRestButton->setText(Work);
        _ui->nextSessionTimeEdit->setEnabled(true);

        setDefaultValues();

        break;

    case State::Work:

        break;

    case State::Rest: {
        _ui->currentSessionTimeLabel->setText(CurrentRestSessionTime);
        _ui->currentSessionTimeEdit->setTime(DefaultRestSessionTime);

        _ui->nextSessionLabel->setText(NextWorkSessionTime);
        _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);

        int msec = - _ui->currentSessionTimeEdit->time().msecsTo(QTime(0, 0));
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
    _ui->totalTimeEdit->setTime(DefaultTotalTime);
    _ui->workTimeEdit->setTime(DefaultWorkTime);
    _ui->restTimeEdit->setTime(DefaultRestTime);

    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);
    _ui->workRadioButton->setChecked(true);
}

void MainWindow::InitialToWork()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeEdit->setTime(DefaultWorkSessionTime);
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->workRestButton->setText(Rest);
    _ui->workRestButton->setEnabled(true);

    int msec = - _ui->currentSessionTimeEdit->time().msecsTo(QTime(0, 0));
    _timeLine.setDuration(msec);
    _timeLine.setFrameRange(0, msec / 1000);
    _timeLine.setUpdateInterval(1000);
    _timeLine.start();

    _state = State::Work;
}

void MainWindow::InitialToRest()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeEdit->setTime(DefaultRestSessionTime);
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->workRestButton->setText(Work);
    _ui->workRestButton->setEnabled(true);

    int msec = - _ui->currentSessionTimeEdit->time().msecsTo(QTime(0, 0));
    _timeLine.setDuration(msec);
    _timeLine.setFrameRange(0, msec / 1000);
    _timeLine.setUpdateInterval(1000);
    _timeLine.start();

    _state = State::Rest;
}

void MainWindow::WorkToInitial()
{
    _ui->startFinishButton->setText(Start);
    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);
}

void MainWindow::WorkToRest()
{

}

void MainWindow::RestToInitial()
{
    _ui->startFinishButton->setText(Start);
    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);
}

void MainWindow::RestToWork()
{

}

void MainWindow::onStartFinishButton()
{
    switch (_state) {
    case State::Initial:
        if (_ui->workRadioButton->isChecked())
            InitialToWork();
        else if (_ui->restRadioButton->isChecked())
            InitialToRest();

        break;

    case State::Work:
        WorkToInitial();
        break;

    case State::Rest:
        RestToInitial();
        break;
    }
}

void MainWindow::onWorkRadioButton()
{
    _ui->currentSessionTimeLabel->setText(CurrentWorkSessionTime);
    _ui->currentSessionTimeEdit->setTime(DefaultWorkSessionTime);

    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);
}

void MainWindow::onRestRadioButton()
{
    _ui->currentSessionTimeLabel->setText(CurrentRestSessionTime);
    _ui->currentSessionTimeEdit->setTime(DefaultRestSessionTime);

    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);
}

void MainWindow::onTimeLineFrameChanged()
{
    _ui->currentSessionTimeEdit->setTime(_ui->currentSessionTimeEdit->time().addSecs(-1));
}

void MainWindow::onTimeLineFinished()
{
    if (_state == State::Work)
        setState(State::Rest);
    else if (_state == State::Rest)
        setState(State::Work); //! transition from Work state
}

MainWindow::~MainWindow()
{
    delete _ui;
}
