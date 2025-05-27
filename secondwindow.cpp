#include "secondwindow.h"
#include "ui_secondWindow.h"
#include <QLabel>
#include <QVBoxLayout>

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);

    setWindowTitle("Анкета");

    connect(ui->loadPhotoButton, &QPushButton::clicked,
            this, &SecondWindow::onLoadPhoto);

    ui->photoLabel->setScaledContents(true);

    // Можно добавить кнопку Сохранить
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::onSaveData()
{
    QString name = ui->lineEdit->text();
    QString age = ui->lineEdit_2->text();
    QString hobbies = ui->lineEdit_3->text();

    QMessageBox::information(this, "Сохранено",
                             "Данные сохранены!\n"
                             "Имя: " + name + "\n" +
                                 "Возраст: " + age + "\n" +
                                 "Увлечения: " + hobbies);

}


void SecondWindow::onLoadPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Выберите изображение"), "",
                                                    tr("Изображения (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!fileName.isEmpty()) {
        QPixmap photo(fileName);
        if (!photo.isNull()) {
            ui->photoLabel->setPixmap(photo);
            ui->photoLabel->setText("");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение");
        }
    }
}
