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

    ui->labelBild->setPixmap(kugelBild);

    schuettelnTimer = new QTimer(this);
    connect(schuettelnTimer, SIGNAL(timeout()), this, SLOT(Anzeigen()));

    rotationTimer = new QTimer(this);
    connect(rotationTimer, SIGNAL(timeout()), this, SLOT(Rotieren()));

    dateiName = QCoreApplication::applicationDirPath() + "/antworten.txt";
    antwortenErstellen(dateiName);

    if (!LadeAntworten(dateiName)) {
        QMessageBox::critical(this, "Fehler", "Die Antworten-Datei konnte nicht geladen werden.");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->labelKugel->setText("");
    ui->labelBild->setPixmap(kugelBild);

    if (!schuettelnTimer->isActive()) {
        schuettelnTimer->start(3000);
    }

    if (!rotationTimer->isActive()) {
        rotationTimer->start(25);
    }

    ui->pushButton->setEnabled(false);
    ui->pushButton->setText("*sinnier*");

}


void MainWindow::Anzeigen(){

    if (antworten.isEmpty()) {
        ui->labelKugel->setText("Keine Antworten verfügbar.");
        return;
    }

    ui->labelBild->setPixmap(kugelBildDreieck);

    std::random_device generator;
    std::mt19937 engine(generator());
    std::uniform_int_distribution<int> distribution(0, antworten.size() - 1);
    int zufall = distribution(engine);
    rotationTimer->stop();
    schuettelnTimer->stop();

    QString labelText = QString("<span style='background-color: #274294; color: white;'>" + antworten[zufall] + "</span>").toUpper();

    ui->labelKugel->setText(labelText);
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText("Schütteln");
}

void MainWindow::Rotieren(){

    static int angle = 0;
    QTransform transform;
    transform.rotate(angle);
    angle = (angle + 10) % 360;

    QPixmap rotatedPixmap = kugelBild.transformed(transform, Qt::SmoothTransformation);
    ui->labelBild->setPixmap(rotatedPixmap);
}

bool MainWindow::LadeAntworten(const QString &dateiName)
{
    QFile file(dateiName);
    qDebug() << "Versuche zu laden aus:" << QFileInfo(file).absoluteFilePath();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fehler beim Öffnen der Datei:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            antworten.append(line); //Füge jede Zeile zur Liste hinzu
        }
    }

    file.close();
    return !antworten.isEmpty(); //Erfolgreich, wenn Liste nicht leer ist
}

void MainWindow::antwortenErstellen(const QString &dateiName)
{
    QFile file(dateiName);

    // Prüfen, ob Datei existiert
    if (!file.exists()) {
        qDebug() << "Datei existiert nicht. Sie wird erstellt:" << dateiName;

        //Datei öffnen (Schreibmodus)
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            //Standard-Antworten in die Datei schreiben
            QStringList standardAntworten = StandardAntworten();

            for (const QString &antwort : standardAntworten) {
                out << antwort << "\n"; //Jede Antwort in eine neue Zeile schreiben
            }

            file.close();
            qDebug() << "Datei erfolgreich erstellt.";
        } else {
            qDebug() << "Fehler: Datei konnte nicht erstellt werden:" << file.errorString();
        }
    } else {
        qDebug() << "Datei existiert bereits:" << dateiName;
    }
}

QStringList MainWindow::StandardAntworten()
{
    return QStringList{
        "Es ist gewiss.", "Es ist eindeutig so.", "Ohne Zweifel.",
        "Ja – mit Sicherheit.", "Darauf könnt Ihr vertrauen.",
        "Meiner Ansicht nach ja.", "In höchstem Maße wahrscheinlich.",
        "Die Aussichten stehen gut.", "Jawohl.",
        "Die Zeichen deuten auf Ja.", "Antwort verschwommen, versucht es erneut.",
        "Fragt später abermals.", "Es ist besser, Euch dies noch nicht zu sagen.",
        "Kann gerade nicht vorausgesagt werden.",
        "Konzentriert Euch und fragt abermals.", "Verlasst Euch nicht darauf.",
        "Meine Antwort lautet nein.", "Meine Quellen verneinen dies.",
        "Die Aussichten stehen nicht so gut.", "Sehr zweifelhaft."
    };
}
