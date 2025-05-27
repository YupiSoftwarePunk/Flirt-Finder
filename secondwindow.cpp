#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow),
    userPhoto(QPixmap())
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText("Имя, пол");
    ui->lineEdit_2->setPlaceholderText("Возраст");
    ui->lineEdit_3->setPlaceholderText("Увлечения");
    ui->lineEdit_4->setPlaceholderText("Город проживания");

    connect(ui->onLoadPhoto, &QPushButton::clicked, this, &SecondWindow::on_addPhotoButton_clicked);
    connect(ui->onSaveData, &QPushButton::clicked, this, &SecondWindow::on_saveButton_clicked);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::on_addPhotoButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Выберите фотографию"), "",
                                                    tr("Изображения (*.png *.jpg *.jpeg *.bmp);;Все файлы (*)"));

    if (!fileName.isEmpty()) {
        QPixmap photo(fileName);
        if (!photo.isNull()) {
            userPhoto = photo;
            photoPath = fileName;

            QIcon icon(QPixmap(fileName).scaled(80, 80, Qt::KeepAspectRatio));
            ui->pushButton_2->setIcon(icon);
            ui->pushButton_2->setText("");
            ui->pushButton_2->setIconSize(QSize(80, 80));
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение");
        }
    }
}

void SecondWindow::on_saveButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString age = ui->lineEdit_2->text();
    QString hobbies = ui->lineEdit_3->text();
    QString city = ui->lineEdit_4->text();

    if (name.isEmpty() || age.isEmpty() || hobbies.isEmpty() || city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QByteArray photoData;
    if (!userPhoto.isNull()) {
        QBuffer buffer(&photoData);
        buffer.open(QIODevice::WriteOnly);
        userPhoto.save(&buffer, "JPG");
    }

    // Здесь можно сохранять данные в бд
    QString userData = QString(
                           "Данные сохранены:\n"
                           "Имя: %1\n"
                           "Возраст: %2\n"
                           "Увлечения: %3\n"
                           "Город: %4\n"
                           "Фото: %5")
                           .arg(name)
                           .arg(age)
                           .arg(hobbies)
                           .arg(city)
                           .arg(photoPath.isEmpty() ? "не добавлено" : photoPath);

    QMessageBox::information(this, "Успешно", userData);

    this->close();
}
