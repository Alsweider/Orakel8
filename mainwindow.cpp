#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kugelBild = QPixmap(":/bilder/Kugel8.png");
    kugelBildDreieck = QPixmap(":/bilder/KugelDreieck.png");
    kugelBild = kugelBild.scaled(ui->labelBild->size(), Qt::KeepAspectRatio);
    kugelBildDreieck = kugelBildDreieck.scaled(ui->labelBild->size(), Qt::KeepAspectRatio);

    // QPixmap kugelBildSkaliert = kugelBild.scaled(ui->labelBild->size(), Qt::KeepAspectRatio);
    ui->labelBild->setPixmap(kugelBild);

    schuettelnTimer = new QTimer(this);
    connect(schuettelnTimer, SIGNAL(timeout()), this, SLOT(Anzeigen()));

    rotationTimer = new QTimer(this);
    connect(rotationTimer, SIGNAL(timeout()), this, SLOT(Rotieren()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->labelKugel->setText("");
    ui->labelBild->setPixmap(kugelBild);

    schuettelnTimer->start(3000);
    rotationTimer->start(25);
    ui->pushButton->setEnabled(false);
    ui->pushButton->setText("*sinnier*");

}


void MainWindow::Anzeigen(){
    ui->labelBild->setPixmap(kugelBildDreieck);

    QStringList antworten;
    antworten << "Es ist gewiss."
              << "Es ist eindeutig so."
              << "Ohne Zweifel."
              << "Ja – mit Sicherheit."
              << "Darauf könnt Ihr vertrauen."
              << "Meiner Ansicht nach ja."
              << "In höchstem Maße wahrscheinlich."
              << "Die Aussichten stehen gut."
              << "Jawohl."
              << "Die Zeichen deuten auf Ja."
              << "Antwort verschwommen, versucht es erneut."
              << "Fragt später abermals."
              << "Es ist besser, Euch dies noch nicht zu sagen."
              << "Kann gerade nicht vorausgesagt werden."
              << "Konzentriert Euch und fragt abermals."
              << "Verlasst Euch nicht darauf."
              << "Meine Antwort lautet nein."
              << "Meine Quellen verneinen dies."
              << "Die Aussichten stehen nicht so gut."
              << "Sehr zweifelhaft.";

    std::random_device generator;
    std::mt19937 engine(generator());
    std::uniform_int_distribution<int> distribution(0, antworten.size() - 1);
    int zufall = distribution(engine);
    rotationTimer->stop();
    schuettelnTimer->stop();

    QString labelText = QString("<span style='background-color: #274294; color: white;'>" + antworten[zufall] + "</span>");
    ui->labelKugel->setText(labelText);
    //ui->labelKugel->setText(antworten[zufall]);
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText("Schütteln");

}

void MainWindow::Rotieren(){

    static int angle = 0;
    QTransform transform;
    transform.rotate(angle);
    angle = (angle + 10) % 360;

    QPixmap rotatedPixmap = kugelBild.transformed(transform, Qt::SmoothTransformation); // Transformation anwenden
    ui->labelBild->setPixmap(rotatedPixmap); // Rotiertes Pixmap anzeigen
}
