#include "second.h"
#include "ui_second.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QSqlQuery>

Second::Second(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Second)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->textEdit->installEventFilter(this);
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

    QString name = ui->lineEdit->text().trimmed();
    int age = ui->spinBox->value();
    QString hobbies = ui->textEdit->toPlainText().trimmed();
    QString city = ui->lineEdit_4->text().trimmed();
    QString sex = ui->comboBox->currentText();
    QString photo = m_photoPath;

    if (name.isEmpty() || age == 0 || hobbies.isEmpty() || city.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
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



    if (saveUserData(login, password, name, sex, age, hobbies, city, photo))
    {
        QMessageBox::information(this, "Успех", "Данные успешно сохранены!");
    } else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить данные!");
    }
}



void Second::setUserCredentials(const QString &login, const QString &password)
{
    this->login = login;
    this->password = password;
}




bool Second::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        QLineEdit *edit = qobject_cast<QLineEdit*>(watched);
        if (edit && (edit == ui->lineEdit || edit == ui->lineEdit_4))
        {
            if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
            {
                return false;
            }

            if (!keyEvent->text().isEmpty() &&
                !QRegularExpression("[a-zA-Zа-яА-ЯёЁ\\s-]").match(keyEvent->text()).hasMatch())
            {
                return true;
            }
        }

        QTextEdit *textEditt = qobject_cast<QTextEdit *>(watched);
        if (textEditt && textEditt == ui->textEdit)
        {
            if (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
            {
                return false;
            }

            // if (!keyEvent->text().isEmpty() &&
            //     !QRegularExpression("[a-zA-Zа-яА-ЯёЁ0-9\\s-]").match(keyEvent->text()).hasMatch())
            // {
            //     return true;
            // }
        }
    }

    return QDialog::eventFilter(watched, event);
}






bool Second::saveUserData(const QString &login, const QString &password,
                  const QString &name, const QString &gender, int age,
                  const QString &hobbies, const QString &city, const QString &photoPath)
{
    QSqlQuery query;


    // Сохранение в users
    query.prepare("INSERT INTO users (login, password, name, gender, age, hobbies, city) "
                  "VALUES (:login, :password, :name, :gender, :age, :hobbies, :city)");

    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":age", age);
    query.bindValue(":hobbies", hobbies);
    query.bindValue(":city", city);

    if (!query.exec()) {
        qDebug() << "Ошибка записи данных пользователя:";
        return false;
    }

    int userId = query.lastInsertId().toInt();


    // Сохранение в photos
    QSqlQuery photoQuery;
    photoQuery.prepare("INSERT INTO photos (user_id, photo_path) VALUES (:user_id, :photo_path)");
    photoQuery.bindValue(":user_id", userId);
    photoQuery.bindValue(":photo_path", photoPath);

    if (!photoQuery.exec()) {
        qDebug() << "Ошибка записи изображения:";
        return false;
    }

    return true;
}
