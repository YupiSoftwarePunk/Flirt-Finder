#include "second.h"
#include "ui_second.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

Second::Second(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Second)
{
    ui->setupUi(this);
}

Second::~Second()
{
    delete ui;
}

void Second::on_onLoadPhoto_clicked()
{
    QString photoPath = QFileDialog::getOpenFileName(this,
                                                     tr("Выберите фото"),
                                                     QDir::homePath(),
                                                     tr("Изображения (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!photoPath.isEmpty())
    {
        QPixmap pixmap(photoPath);

        if (pixmap.isNull())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение!");
            return;
        }

        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pixmap.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
        ui->graphicsView->setScene(scene);

        QMessageBox::information(this, "Фото добавлено", "Фото успешно выбрано!");
    }
}


void Second::on_onSaveData_clicked()
{
    // QRegularExpressionValidator *textValidator = new QRegularExpressionValidator(
    //     QRegularExpression(R"(^[a-zA-Zа-яА-ЯёЁ\s-]+$)"), this);


    QString name = ui->lineEdit->text().trimmed();
    int age = ui->spinBox->value();
    QString hobbies = ui->textEdit->toPlainText();
    QString city = ui->lineEdit_4->text().trimmed();
    QString sex = ui->comboBox->currentText();

    // ui->lineEdit->setValidator(textValidator);
    // ui->lineEdit_4->setValidator(textValidator);

    if (name.isEmpty() || age == 0 || hobbies.isEmpty() || city.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить анкету"),
                                                    QDir::homePath() + "/" + name + "_анкета.txt",
                                                    tr("Текстовые файлы (*.txt)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << "Анкета пользователя\n";
            stream << "Имя: " << name << "\n";
            stream << "Пол: "<< sex<<"\n";
            stream << "Возраст: " << age << "\n";
            stream << "Увлечения: " << hobbies << "\n";
            stream << "Город проживания: " << city << "\n";

            if (!m_photoPath.isEmpty())
            {
                stream << "Фото: " << m_photoPath << "\n";
            }

            file.close();
            QMessageBox::information(this, "Сохранено", "Анкета успешно сохранена!");
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл!");
        }
    }



    QRegularExpression validText("^[a-zA-Zа-яА-ЯёЁ\\s-]+$");

    if (!validText.match(name).hasMatch() || name.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Имя содержит недопустимые символы");
        ui->lineEdit->setFocus();
        return;
    }

    if (!validText.match(city).hasMatch() || city.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Город содержит недопустимые символы");
        ui->lineEdit_4->setFocus();
        return;
    }
}

