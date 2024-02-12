# Модель приложения для бронирования билетов в театр/кинотеатр
---
## Описание 
В данном приложении реализован простейший интерфейс для бронирования билета в театр/кинотеатр. Пользователь, открывший приложение может зайти в свой аккаунт или зарегистрироваться. Затем, он получает доступ к расписанию всех ближайших спектаклей с полной информацией о каждом.<br><br>
После нажатия на кнопку *"Места"*, открывается новое окно, на котором представлена схема зала. Зеленым подсвечены свободные места, красным - занятые. По нажатию на любое место пользователь либо получает предложение о добавлении билета на данное место в корзину, либо получает информацию о том, что место занято.<br><br>
Также, пользователь может просматривать билеты в своей корзине и удалять их. Пользователь также имеет возможность изменять личные данные и пароль от своей учетной записи.<br><br>
Данный проект был выполнен в ходе прохождения курса "Базы данных" на 3ем курсе РТУ МИРЭА по направлению *09.03.02 Информационные системы и технологии*.
## Технологический стек
Приложение написано на языке C++ на фреймфорке QtCreator версии 5.0.1 *(версия Qt 5.12.2)*. База данных для данного приложения была создана с помощью приложения pgAdmin 4 версии 7.8.<br>
## Особенности реализации
Каждое окно, является отдельным классом, отнаследованным от `QDialog`. Все запросы к БД осуществляются в `mainwindow.cpp`, несмотря на то в каком окне происходит то, или иное действие.<br><br>
Создан также отдельный класс, отнаследованный от `QPushButton`, в котором переопределено событие нажатие на кнопку левой кнопкой мыши. Сигнал `clicked()` передает также наименование объекта *(то есть кнопки)*. Так, можно определить по наименованию объекта, какая именно кнопка в списке была нажата, так как при создании кнопки в списке, каждой присвается уникальное имя.<br><br>
Создан отдельный класс, отнаследованный от `QObject`, с переопределнным методом `eventFilter()`. Данный объект после вешается на каждый `QLabel` в схеме зала, для того чтобы сделать данные объкеты кликабельными.<br><br>
Также существует пользователь с особыми правами - "администратор" приложения. Он может добавлять, удалять и изменять каждый спектакль. При удалении спектакля, на который есть билеты, автоматически удаляются все билеты.(удаление происходит при помощи триггера в самой СУБД)<br>
## Особенности работы приложения
Для запуска и корректной работы приложения подразумевается, что на ПК пользователя должна быть открыта БД с определенными параметрами. Корректный бэкап БД представлен файлом с расширением `.backup` в репозитории.
