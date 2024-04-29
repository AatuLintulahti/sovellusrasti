#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      networkManager(new QNetworkAccessManager(this)) {
    ui->setupUi(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);


    connect(ui->carsTableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::on_carsTableWidget_itemSelectionChanged);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fetchCarsButton_clicked()
{
    fetchCars();
}

void MainWindow::on_carsTableWidget_itemSelectionChanged() {
    int currentRow = ui->carsTableWidget->currentRow();
    if (currentRow != -1) {
        QTableWidgetItem *item = ui->carsTableWidget->item(currentRow, 0);
        if (item != nullptr) {
            selectedCarId = item->text();
            qDebug() << "Selected car ID:" << selectedCarId;
        }
    } else {
        selectedCarId.clear();
    }
}


void MainWindow::on_addCarButton_clicked()
{
    QString branch = ui->branchLineEdit->text();
    QString model = ui->modelLineEdit->text();
    createCar(branch, model);
}

void MainWindow::on_deleteCarButton_clicked() {
    if (!selectedCarId.isEmpty()) {
        deleteCar(selectedCarId);
        qDebug() << "Attempting to delete car with ID:" << selectedCarId;
    } else {
        qDebug() << "No car selected for deletion.";

    }
}


void MainWindow::on_updateCarButton_clicked() {
    if (!selectedCarId.isEmpty()) {
        QString branch = ui->branchLineEdit->text();
        QString model = ui->modelLineEdit->text();
        updateCar(selectedCarId, branch, model);
        qDebug() << "Attempting to update car with ID:" << selectedCarId;
    } else {
        qDebug() << "No car selected for update.";

    }
}



void MainWindow::fetchCars()
{
    QNetworkRequest request(QUrl("http://localhost:3000/car"));
    networkManager->get(request);
}

void MainWindow::createCar(const QString &branch, const QString &model)
{
    QJsonObject json;
    json["branch"] = branch;
    json["model"] = model;
    QNetworkRequest request(QUrl("http://localhost:3000/car"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        networkManager->post(request, QJsonDocument(json).toJson());
    }

void MainWindow::updateCar(const QString &id, const QString &branch, const QString &model) {
    QJsonObject json;
    json["branch"] = branch;
    json["model"] = model;
    QNetworkRequest request(QUrl(QString("http://localhost:3000/car/%1").arg(id)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager->put(request, QJsonDocument(json).toJson());
}


void MainWindow::deleteCar(const QString &id) {
    if (id.isEmpty()) {

        return;
    }
    QNetworkRequest request(QUrl(QString("http://localhost:3000/car/%1").arg(id)));
    networkManager->deleteResource(request);
}



void MainWindow::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        ui->statusLabel->setText("Error: " + reply->errorString());
        return;
    }



    auto data = reply->readAll();

    if (reply->url().toString().endsWith("/car")) {
        displayCars(data);
    }


}

void MainWindow::displayCars(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();
    ui->carsTableWidget->clearContents();
    ui->carsTableWidget->setRowCount(array.size());

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        ui->carsTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
        ui->carsTableWidget->setItem(i, 1, new QTableWidgetItem(obj["branch"].toString()));
        ui->carsTableWidget->setItem(i, 2, new QTableWidgetItem(obj["model"].toString()));
    }
}



