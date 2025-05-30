#include "second.h"
#include "ui_second.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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
        m_photoPath = photoPath;

        QMessageBox::information(this, "Фото добавлено",
                                 "Фото успешно выбрано: " + photoPath);
    }
}


void Second::on_onSaveData_clicked()
{
    QString name = ui->lineEdit->text();
    QString age = ui->lineEdit_2->text();
    QString hobbies = ui->lineEdit_3->text();
    QString city = ui->lineEdit_4->text();

    if (name.isEmpty() || age.isEmpty() || hobbies.isEmpty() || city.isEmpty())
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
            stream << "Имя, пол: " << name << "\n";
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
}

