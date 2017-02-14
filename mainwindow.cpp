#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>

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

    toInitial();
}

void MainWindow::toInitial()
{
    _ui->totalTimeEdit->setEnabled(true);
    _ui->totalTimeEdit->setTime(DefaultTotalTime);

    _ui->workTimeEdit->setEnabled(true);
    _ui->workTimeEdit->setTime(DefaultWorkTime);

    _ui->restTimeEdit->setEnabled(true);
    _ui->restTimeEdit->setTime(DefaultRestTime);

    _ui->startFinishButton->setText(Start);
    _ui->startFinishButton->setEnabled(true);

    _ui->workRadioButton->setEnabled(true);
    _ui->restRadioButton->setEnabled(true);
    _ui->workRadioButton->setChecked(true);
    onWorkRadioButton();

    _ui->currentSessionTimeLabel->clear();
    _ui->currentSessionTimeEdit->setEnabled(false);
    _ui->currentSessionTimeEdit->clear();

    _ui->nextSessionTimeEdit->setEnabled(true);

    _ui->workRestButton->setEnabled(false);
    _ui->workRestButton->setText(Work);

    _state = State::Initial;
}

void MainWindow::InitialToWork()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeLabel->setText(CurrentWorkSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);

    _ui->workRestButton->setText(Rest);
    _ui->workRestButton->setEnabled(true);

    int secs = - _ui->workTimeEdit->time().secsTo(QTime(0, 0));
    _timeLine.setDuration(secs * 1000);
    _timeLine.setFrameRange(0, secs);
    _timeLine.start();

    _state = State::Work;
}

void MainWindow::InitialToRest()
{
    _ui->startFinishButton->setText(Finish);
    _ui->workRadioButton->setEnabled(false);
    _ui->restRadioButton->setEnabled(false);

    _ui->currentSessionTimeLabel->setText(CurrentRestSessionTime);
    _ui->currentSessionTimeEdit->setTime(_ui->nextSessionTimeEdit->time());
    _ui->currentSessionTimeEdit->setEnabled(true);
    _ui->currentSessionTimeEdit->setReadOnly(true);

    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);

    _ui->workRestButton->setText(Work);
    _ui->workRestButton->setEnabled(true);

    int secs = - _ui->workTimeEdit->time().secsTo(QTime(0, 0));
    _timeLine.setDuration(secs * 1000);
    _timeLine.setFrameRange(0, secs);
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
    _ui->nextSessionLabel->setText(NextWorkSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultWorkSessionTime);
}

void MainWindow::onRestRadioButton()
{
    _ui->nextSessionLabel->setText(NextRestSessionTime);
    _ui->nextSessionTimeEdit->setTime(DefaultRestSessionTime);
}

void MainWindow::onTimeLineFrameChanged()
{
    static QTime currentTime = _ui->currentSessionTimeEdit->time();

    static QTimeEdit *wholeTimeEdit = _state == State::Work
           ? _ui->workTimeEdit
           : _ui->restTimeEdit;

    static QTime wholeTime = wholeTimeEdit->time();

    static int secsToAddToCurrentTime = -1;
    if (currentTime == QTime(0, 0)) {
        secsToAddToCurrentTime = 1;
        QPalette palette(_ui->currentSessionTimeEdit->palette());
        palette.setColor(QPalette::Text, Qt::red);
        _ui->currentSessionTimeEdit->setPalette(palette);
    }

    static int secsToAddToWholeTime = -1;

    currentTime = currentTime.addSecs(secsToAddToCurrentTime);
    wholeTime = wholeTime.addSecs(secsToAddToWholeTime);
    _ui->currentSessionTimeEdit->setTime(currentTime);
    wholeTimeEdit->setTime(wholeTime);
}

void MainWindow::onTimeLineFinished()
{
    if (_state == State::Work)
        WorkToInitial();
    else if (_state == State::Rest)
        RestToInitial();
}

MainWindow::~MainWindow()
{
    delete _ui;
}
