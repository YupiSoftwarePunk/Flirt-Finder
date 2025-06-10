#include "second.h"
#include "third.h"
#include "ui_second.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QSqlError>


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



void Second::initializeUserData()
{
    loadUserData();   // Загружаем данные анкеты
    loadPhotoData(login); // Загружаем картинку пользователя
}






// кнопка загрузки изображения в анкету
void Second::on_onLoadPhoto_clicked()
{
    QString photoPath = QFileDialog::getOpenFileName(this,
                                                     tr("Выберите фото"),
                                                     QDir::homePath(),
                                                     tr("Изображения (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!photoPath.isEmpty())
    {
        QPixmap pixmap(photoPath);

        m_photoPath = photoPath;

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




// кнопка сохранения в анкете
void Second::on_onSaveData_clicked()
{

    QString name = ui->lineEdit->text().trimmed();
    int age = ui->spinBox->value();
    QString hobbies = ui->textEdit->toPlainText().trimmed();
    QString city = ui->lineEdit_4->text().trimmed();
    QString sex = ui->comboBox->currentText();
    QString photo = m_photoPath;


    if (m_photoPath.isEmpty())
    {
        QSqlQuery query;
        query.prepare("SELECT photo_path FROM photos WHERE user_id = (SELECT id FROM users WHERE login = :login)");
        query.bindValue(":login", login);
        if (query.exec() && query.next())
        {
            m_photoPath = query.value(0).toString();
        }
    }


    if (name.isEmpty() || hobbies.isEmpty() || city.isEmpty() || photo.isEmpty())
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
        loadPhotoData(login);
        QMessageBox::information(this, "Успех", "Данные успешно сохранены!");

        auto thirdWindow = new Third();
        thirdWindow->loadProfiles(login); // Передаём login в метод для загрузки профилей
        thirdWindow->show();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить данные!");
    }
}



void Second::setUserCredentials(const QString &login, const QString &password)
{
    this->login = login;
    this->password = password;
}




// сортировка ввода в поля анкеты
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






// сохранение данных в бд
bool Second::saveUserData(const QString &login, const QString &password,
                  const QString &name, const QString &gender, int age,
                  const QString &hobbies, const QString &city, const QString &photoPath)
{


    QSqlQuery query;
    query.prepare("UPDATE users SET name = :name, gender = :gender, age = :age, hobbies = :hobbies, city = :city "
                  "WHERE login = :login AND password = :password");
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":age", age);
    query.bindValue(":hobbies", hobbies);
    query.bindValue(":city", city);
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка сохранения данных пользователя!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return false;
    }



    QSqlQuery getUserIdQuery;
    getUserIdQuery.prepare("SELECT id FROM users WHERE login = :login");
    getUserIdQuery.bindValue(":login", login);

    if (!getUserIdQuery.exec() || !getUserIdQuery.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ID пользователя!");
        qDebug() << "Ошибка SQL:" << getUserIdQuery.lastError().text();
        return false;
    }

    int userId = getUserIdQuery.value(0).toInt();
    qDebug() << "User ID:" << userId;


    if (!photoPath.isEmpty())
    {
        QSqlQuery deletePhotoQuery;
        deletePhotoQuery.prepare("DELETE FROM photos WHERE user_id = :user_id");
        deletePhotoQuery.bindValue(":user_id", userId);

        if (!deletePhotoQuery.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить старую картинку!");
            qDebug() << "Ошибка SQL:" << deletePhotoQuery.lastError().text();
            return false;
        }

        QSqlQuery photoQuery;
        photoQuery.prepare("INSERT INTO photos (user_id, photo_path) VALUES (:user_id, :photo_path)");
        photoQuery.bindValue(":user_id", userId);
        photoQuery.bindValue(":photo_path", photoPath);

        if (!photoQuery.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка сохранения изображения!");
            qDebug() << "Ошибка SQL:" << photoQuery.lastError().text();
            return false;
        }
        qDebug() << "Новый путь к картинке успешно сохранён:" << photoPath;
    }
    return true;
}





// обновление данных пользователя
void Second::loadUserData()
{
    QSqlQuery query;
    query.prepare("SELECT name, gender, age, hobbies, city FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec())
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка загрузки данных!");
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }

    if (query.next())
    {
        ui->lineEdit->setText(query.value("name").toString());
        ui->comboBox->setCurrentText(query.value("gender").toString());
        ui->spinBox->setValue(query.value("age").toInt());
        ui->textEdit->setPlainText(query.value("hobbies").toString());
        ui->lineEdit_4->setText(query.value("city").toString());
    }
}




// обновление картинки пользователя
void Second::loadPhotoData(const QString &login)
{
    QSqlQuery photoQuery;
    photoQuery.prepare("SELECT photo_path FROM photos WHERE user_id = (SELECT id FROM users WHERE login = :login)");
    photoQuery.bindValue(":login", login);

    if (!photoQuery.exec() || !photoQuery.next())
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение!");
        qDebug() << "Ошибка SQL:" << photoQuery.lastError().text();
        return;
    }

    QString photoPath = photoQuery.value(0).toString();
    qDebug() << "Загруженный путь к картинке:" << photoPath;

    if (!photoPath.isEmpty())
    {
        QPixmap pixmap(photoPath);
        if (!pixmap.isNull())
        {
            QGraphicsScene *scene = new QGraphicsScene(this);
            scene->addPixmap(pixmap.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
            ui->graphicsView->setScene(scene);
        } else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение в интерфейс!");
        }
    }
}
