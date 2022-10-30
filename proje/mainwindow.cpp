#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTableView>
#include <QLCDNumber>
#include <QString>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include "dbconnector.h" //database connection from C++ dbconnector class
#include <QPixmap>
#include <QInputDialog>

static QDBMLite::DB Rdb; //namespace from C++ dbconnector class


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Rdb.dbstate = Rdb.ConnecT(QCoreApplication::applicationDirPath()+"/project.sqlite");
    qDebug() << QCoreApplication::applicationDirPath();
    QSqlQuery qry(Rdb.db);
        qry.prepare("CREATE TABLE project ( name STRING, proCode STRING, category STRING, quantity INTEGER, brand STRING, price INTEGER)");
        if(qry.exec()){
            qDebug() << "Library Table created successfully";
        }
        else{
            qDebug() << "Library Table already created and database connection is opened";
        }
        QString tempSearch;
        int tempInt;
        tempSearch = ui->lineEditSearch->text();
        tempInt = ui->lineEditSearch->text().toInt();
        qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
        qry.bindValue(":search", QString("%%1%").arg(tempSearch));
        //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
        qry.exec();
        while(qry.next()){
            qDebug()<< qry.value(0) << qry.value(1);
        }
        QSqlQueryModel *modal = new QSqlQueryModel();
        modal->setQuery(qry);
        ui->tableView->setModel(modal);

        int numberOfRows = 0;
        if(qry.last())
        {
            numberOfRows =  qry.at() + 1;
            qry.first();
            qry.previous();
        }

        ui->lcdNumber->display(numberOfRows);
        QString tempSearchProject;
        int tempIntProject;
        tempSearchProject = ui->lineEditSearchProject->text();
        tempIntProject = ui->lineEditSearchProject->text().toInt();
        qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
        qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
        //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
        qry.exec();
        while(qry.next()){
            qDebug()<< qry.value(0) << qry.value(1);
        }
        QSqlQueryModel *modal1 = new QSqlQueryModel();
        modal1->setQuery(qry);
        ui->tableViewProject->setModel(modal1);

        int numberOfRowsProject = 0;
        if(qry.last())
        {
            numberOfRowsProject =  qry.at() + 1;
            qry.first();
            qry.previous();
        }

        ui->lcdNumberProject->display(numberOfRowsProject);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButtonTR_clicked()
{
    ui->btnAdd->setText("Ekle");
        ui->btnChange->setText("Degistir");
        ui->btnDelete->setText("Sil");
        ui->btnAddNew->setText("Yeni Ekle");
        ui->btnSearch->setText("Ara");
        ui->btnDimin->setText("Çıkar");
        ui->btnClear->setText("Temizle");
        ui->labelName->setText("Ad:");
        ui->labelQuantity->setText("Miktar:");
        ui->labelProCode->setText("Ürün Kodu:");
        ui->labelCat->setText("Kategori:");
        ui->labelBrand->setText("Marka:");
        ui->labelProCount->setText("Ürün Sayısı:");
        ui->labelPrice->setText("Fiyat:");
        ui->btnSearchProject->setText("Ara");
        ui->labelProCountProject->setText("Proje Sayısı:");
        ui->labelProCountProject_2->setText("Projede Kullanılan Ürün Sayısı:");
        ui->btnSearchProject_2->setText("Göster");
        ui->btnSearch3->setText("Ara");
}

void MainWindow::on_radioButtonEN_clicked()
{
    ui->btnAdd->setText("Add");
    ui->btnChange->setText("Change");
    ui->btnDelete->setText("Delete");
    ui->btnAddNew->setText("Add New Item");
    ui->btnSearch->setText("Search");
    ui->btnDimin->setText("Decrease");
    ui->btnClear->setText("Clear");
    ui->labelName->setText("Name:");
    ui->labelQuantity->setText("Quantity:");
    ui->labelProCode->setText("Product Code:");
    ui->labelCat->setText("Category:");
    ui->labelBrand->setText("Brand:");
    ui->labelProCount->setText("Product Count:");
    ui->labelPrice->setText("Price:");
    ui->btnSearchProject->setText("Search");
    ui->labelProCountProject->setText("Project Count:");
    ui->labelProCountProject_2->setText("Items Used in Project:");
    ui->btnSearchProject_2->setText("Display");
    ui->btnSearch3->setText("Search");
}

void MainWindow::on_btnAddNew_clicked()
{
    QSqlQuery qry(Rdb.db);
    bool *cond;
    bool condition;
    cond = &condition;
    QString addNewName = ui->lineEditName->text();
    QString addNewProCode = ui->lineEditProCode->text();
    QString addNewCategory = ui->lineEditCat->text();
    int addNewQuantity = ui->lineEditQuantity->text().toInt();
    QString addNewBrand = ui->lineEditBrand->text();
    int addNewPrice = ui->lineEditName->text().toInt();

    QString refProCode = ui->lineEditProCode->text();
    qry.prepare("SELECT proCode FROM project WHERE proCode = :tempProCode");
    qry.bindValue(":tempProCode", refProCode);
    if(qry.exec()){
        if(qry.next()){
            QMessageBox msgBox;
            msgBox.setText("Ürün zaten var");
            msgBox.exec();
            cleanLineEdits();
        }
        else{
            qry.prepare("INSERT INTO project (name, proCode, category, quantity, brand, price) VALUES (:name, :proCode, :category, :quantity, :brand, :price)");
            qry.bindValue(":name", ui->lineEditName->text());
            qry.bindValue(":proCode", ui->lineEditProCode->text());
            qry.bindValue(":category", ui->lineEditCat->text());
            qry.bindValue(":quantity", ui->lineEditQuantity->text().toInt());
            qry.bindValue(":brand", ui->lineEditBrand->text());
            qry.bindValue(":price", ui->lineEditPrice->text().toInt());
            if(qry.exec()){
                qDebug() << "Item added to database";
                cleanLineEdits(); // calling the function to clean the lineEdits
            }
            else{
               qDebug() << "Error in the database";
            }
        }

        }
        QString tempSearch;
        int tempInt;
        tempSearch = ui->lineEditSearch->text();
        tempInt = ui->lineEditSearch->text().toInt();
        qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
        qry.bindValue(":search", QString("%%1%").arg(tempSearch));
        //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
        qry.exec();
        while(qry.next()){
            qDebug()<< qry.value(0) << qry.value(1);
        }
        QSqlQueryModel *modal = new QSqlQueryModel();
        modal->setQuery(qry);
        ui->tableView->setModel(modal);

        int numberOfRows = 0;
        if(qry.last())
        {
            numberOfRows =  qry.at() + 1;
            qry.first();
            qry.previous();
        }

        ui->lcdNumber->display(numberOfRows);
        QString tempSearchProject;
        int tempIntProject;
        tempSearchProject = ui->lineEditSearchProject->text();
        tempIntProject = ui->lineEditSearchProject->text().toInt();
        qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
        qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
        //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
        qry.exec();
        while(qry.next()){
            qDebug()<< qry.value(0) << qry.value(1);
        }
        QSqlQueryModel *modal1 = new QSqlQueryModel();
        modal1->setQuery(qry);
        ui->tableViewProject->setModel(modal1);

        int numberOfRowsProject = 0;
        if(qry.last())
        {
            numberOfRowsProject =  qry.at() + 1;
            qry.first();
            qry.previous();
        }

        ui->lcdNumberProject->display(numberOfRowsProject);

}

void MainWindow::cleanLineEdits()
{
    ui->lineEditName->setText("");
    ui->lineEditProCode->setText("");
    ui->lineEditCat->setText("");
    ui->lineEditQuantity->setText("");
    ui->lineEditBrand->setText("");
    ui->lineEditPrice->setText("");
}

void MainWindow::on_btnAdd_clicked()
{
    QSqlQuery qry(Rdb.db);
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    int *qqq;
    int addedQuantity = ui->lineEditQuantity->text().toInt();
    qqq = &addedQuantity;
    int q = *qqq;
    QString addName;
    QString addProCode;
    QString addCategory;
    int addQuantity;
    QString addBrand;
    int addPrice;
    int newQuantity;
    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        int row = index.row();
        addName = index.sibling(row, 0).data().toString();
        addProCode =  index.sibling(row, 1).data().toString();
        addCategory = index.sibling(row, 2).data().toString();
        addQuantity = index.sibling(row, 3).data().toInt();
        addBrand = index.sibling(row, 4).data().toString();
        addPrice = index.sibling(row, 5).data().toInt();
        // To get the row/column numbers use index.row() / index.column()
    }
    qDebug() << addQuantity;
    newQuantity = addQuantity + q;
    qry.prepare("UPDATE project SET quantity = :tempQuantity WHERE name = :tempName");
    qry.bindValue(":tempQuantity", newQuantity);
    qry.bindValue(":tempName", addName);
    if(qry.exec()){
        qDebug() << "Item updated in database";
        cleanLineEdits(); // calling the function to clean the lineEdits
    }
    else{
       qDebug() << "Error in the database";
    }
    QString tempSearch;
    int tempInt;
    tempSearch = ui->lineEditSearch->text();
    tempInt = ui->lineEditSearch->text().toInt();
    qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearch));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(qry);
    ui->tableView->setModel(modal);

    int numberOfRows = 0;
    if(qry.last())
    {
        numberOfRows =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumber->display(numberOfRows);
    QString tempSearchProject;
    int tempIntProject;
    tempSearchProject = ui->lineEditSearchProject->text();
    tempIntProject = ui->lineEditSearchProject->text().toInt();
    qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal1 = new QSqlQueryModel();
    modal1->setQuery(qry);
    ui->tableViewProject->setModel(modal1);

    int numberOfRowsProject = 0;
    if(qry.last())
    {
        numberOfRowsProject =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject->display(numberOfRowsProject);
}


void MainWindow::on_btnSearch_clicked()
{
    QSqlQuery qry(Rdb.db);
    QString tempSearch;
    int tempInt;
    tempSearch = ui->lineEditSearch->text();
    tempInt = ui->lineEditSearch->text().toInt();
    qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearch));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }

    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(qry);
    ui->tableView->setModel(modal);

    int numberOfRows = 0;
    if(qry.last())
    {
        numberOfRows =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumber->display(numberOfRows);

}


void MainWindow::on_btnDimin_clicked()
{
    QSqlQuery qry(Rdb.db);
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    int *qqq;
    int addedQuantity = ui->lineEditQuantity->text().toInt();
    qqq = &addedQuantity;
    int q = *qqq;
    QString addName;
    QString addProCode;
    QString addCategory;
    int addQuantity;
    QString addBrand;
    int addPrice;
    int newQuantity;
    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        int row = index.row();
        addName = index.sibling(row, 0).data().toString();
        addProCode =  index.sibling(row, 1).data().toString();
        addCategory = index.sibling(row, 2).data().toString();
        addQuantity = index.sibling(row, 3).data().toInt();
        addBrand = index.sibling(row, 4).data().toString();
        addPrice = index.sibling(row, 5).data().toInt();
        // To get the row/column numbers use index.row() / index.column()
    }
    qDebug() << addQuantity;
    newQuantity = addQuantity - q;
    qry.prepare("UPDATE project SET quantity = :tempQuantity WHERE name = :tempName");
    qry.bindValue(":tempQuantity", newQuantity);
    qry.bindValue(":tempName", addName);
    if(qry.exec()){
        qDebug() << "Item updated in database";
        cleanLineEdits(); // calling the function to clean the lineEdits
    }
    else{
       qDebug() << "Error in the database";
    }
    QString tempSearch;
    int tempInt;
    tempSearch = ui->lineEditSearch->text();
    tempInt = ui->lineEditSearch->text().toInt();
    qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearch));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(qry);
    ui->tableView->setModel(modal);

    int numberOfRows = 0;
    if(qry.last())
    {
        numberOfRows =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumber->display(numberOfRows);
    bool ok;
    QString projectName = QInputDialog::getText(this, tr("Proje ismi"),
                                             tr("Proje Kodunu yazınız:"), QLineEdit::Normal,
                                             "", &ok);
    qry.prepare("SELECT name FROM projectNames WHERE name = :tempProCode");
    qry.bindValue(":tempProCode", projectName);
    if(qry.exec()){
        if(qry.next()){
            QMessageBox msgBox;
            msgBox.setText("Proje zaten eklenmiş");
            msgBox.exec();
            qry.prepare("INSERT INTO " + projectName + " (name, proCode, category, quantity, brand, price) VALUES (:name, :proCode, :category, :quantity, :brand, :price)");
            qry.bindValue(":name", addName);
            qry.bindValue(":proCode", addProCode);
            qry.bindValue(":category", addCategory);
            qry.bindValue(":quantity", addQuantity);
            qry.bindValue(":brand", addBrand);
            qry.bindValue(":price", addPrice);
            if(qry.exec()){
                qDebug() << "Item added to" + projectName;
                cleanLineEdits(); // calling the function to clean the lineEdits
            }
            else{
               qDebug() << "Error in the table:" + projectName;
            }
            cleanLineEdits();
        }
        else{
            qry.prepare("INSERT INTO projectNames (name) VALUES (:name)");
            qry.bindValue(":name", projectName);
            if(qry.exec()){
                qDebug() << "Item used in project";
                cleanLineEdits(); // calling the function to clean the lineEdits
            }
            else{
                qDebug() << "Error in the database";
            }
            qry.prepare("CREATE TABLE IF NOT EXISTS " + projectName  + "( name STRING, proCode STRING, category STRING, quantity INTEGER, brand STRING, price INTEGER)");
            if(qry.exec()){
                qDebug() << projectName + "table created.";
                cleanLineEdits(); // calling the function to clean the lineEdits
            }
            else{
                qDebug() << "Error in the database";
            }
            qry.prepare("INSERT INTO " + projectName + " (name, proCode, category, quantity, brand, price) VALUES (:name, :proCode, :category, :quantity, :brand, :price)");
            qry.bindValue(":name", addName);
            qry.bindValue(":proCode", addProCode);
            qry.bindValue(":category", addCategory);
            qry.bindValue(":quantity", newQuantity);
            qry.bindValue(":brand", addBrand);
            qry.bindValue(":price", addPrice);
            if(qry.exec()){
                qDebug() << "Item added to " + projectName;
                cleanLineEdits(); // calling the function to clean the lineEdits
            }
            else{
               qDebug() << "Error in the table:" + projectName;
            }
        }
    }

    QString tempSearchProject;
    int tempIntProject;
    tempSearchProject = ui->lineEditSearchProject->text();
    tempIntProject = ui->lineEditSearchProject->text().toInt();
    qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal1 = new QSqlQueryModel();
    modal1->setQuery(qry);
    ui->tableViewProject->setModel(modal1);

    int numberOfRowsProject = 0;
    if(qry.last())
    {
        numberOfRowsProject =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject->display(numberOfRowsProject);

}


void MainWindow::on_btnChange_clicked()
{

    QSqlQuery qry(Rdb.db);
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    int *qqq;
    int addedQuantity = ui->lineEditQuantity->text().toInt();
    qqq = &addedQuantity;
    int q = *qqq;
    QString addName;
    QString addProCode;
    QString addCategory;
    int addQuantity;
    QString addBrand;
    int addPrice;
    int newQuantity;
    QString newName = ui->lineEditName->text();
    QString newProCode = ui->lineEditProCode->text();
    QString newCategory = ui->lineEditCat->text();
    int changeQuantity = ui->lineEditQuantity->text().toInt();
    QString newBrand = ui->lineEditBrand->text();
    int newPrice = ui->lineEditPrice->text().toInt();

    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        int row = index.row();
        addName = index.sibling(row, 0).data().toString();
        addProCode =  index.sibling(row, 1).data().toString();
        addCategory = index.sibling(row, 2).data().toString();
        addQuantity = index.sibling(row, 3).data().toInt();
        addBrand = index.sibling(row, 4).data().toString();
        addPrice = index.sibling(row, 5).data().toInt();
        // To get the row/column numbers use index.row() / index.column()
    }
    if(ui->lineEditName->text().isEmpty() == false){
        qry.prepare("UPDATE project SET name = :newName WHERE name = :oldName");
        qry.bindValue(":newName", newName);
        qry.bindValue(":oldName", addName);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    if(ui->lineEditProCode->text().isEmpty() == false){
        qry.prepare("UPDATE project SET proCode = :newProCode WHERE proCode = :oldProCode");
        qry.bindValue(":newProCode", newProCode);
        qry.bindValue(":oldProCode", addProCode);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    if(ui->lineEditCat->text().isEmpty() == false){
        qry.prepare("UPDATE project SET category = :newCat WHERE category = :oldCat");
        qry.bindValue(":newCat", newCategory);
        qry.bindValue(":oldCat", addCategory);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    if(ui->lineEditQuantity->text().isEmpty() == false){
        qry.prepare("UPDATE project SET quantity = :newQ WHERE name = :oldQ");
        qry.bindValue(":newQ", changeQuantity);
        qry.bindValue(":oldQ", addName);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    if(ui->lineEditBrand->text().isEmpty() == false){
        qry.prepare("UPDATE project SET brand = :newBrand WHERE brand = :oldBrand");
        qry.bindValue(":newBrand", newBrand);
        qry.bindValue(":oldBrand", addBrand);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    if(ui->lineEditPrice->text().isEmpty() == false){
        qry.prepare("UPDATE project SET price = :newPrice WHERE name = :oldName");
        qry.bindValue(":newPrice", newPrice);
        qry.bindValue(":oldName", addName);
        if(qry.exec()){
            qDebug() << "Item updated in database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
    }
    QString tempSearch;
    int tempInt;
    tempSearch = ui->lineEditSearch->text();
    tempInt = ui->lineEditSearch->text().toInt();
    qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearch));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(qry);
    ui->tableView->setModel(modal);

    int numberOfRows = 0;
    if(qry.last())
    {
        numberOfRows =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumber->display(numberOfRows);
    QString tempSearchProject;
    int tempIntProject;
    tempSearchProject = ui->lineEditSearchProject->text();
    tempIntProject = ui->lineEditSearchProject->text().toInt();
    qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal1 = new QSqlQueryModel();
    modal1->setQuery(qry);
    ui->tableViewProject->setModel(modal1);

    int numberOfRowsProject = 0;
    if(qry.last())
    {
        numberOfRowsProject =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject->display(numberOfRowsProject);
}

void MainWindow::on_btnDelete_clicked()
{
    QSqlQuery qry(Rdb.db);
    QString itemName;
    QString tempSearch;
    itemName = ui->lineEditName->text();
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    QString delName;
    QString delProCode;
    QString delCategory;
    int delQuantity;
    QString delBrand;
    int delPrice;
    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);
        int row = index.row();
        delName = index.sibling(row, 0).data().toString();
        delProCode =  index.sibling(row, 1).data().toString();
        delCategory = index.sibling(row, 2).data().toString();
        delQuantity = index.sibling(row, 3).data().toInt();
        delBrand = index.sibling(row, 4).data().toString();
        delPrice = index.sibling(row, 5).data().toInt();
        // To get the row/column numbers use index.row() / index.column()
    }
    QMessageBox msgBox;
    msgBox.setText("Item deleting: " + delName + "?");
    msgBox.setInformativeText("Do you want to delete");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
    {
        qry.prepare("DELETE FROM project WHERE name = :deleteName AND proCode = :deleteProCode AND category = :deleteCategory AND quantity = :deleteQuantity AND brand = :deleteBrand AND price = :deletePrice");
        qry.bindValue(":deleteName", delName);
        qry.bindValue(":deleteProCode", delProCode);
        qry.bindValue(":deleteCategory", delCategory);
        qry.bindValue(":deleteQuantity", delQuantity);
        qry.bindValue(":deleteBrand", delBrand);
        qry.bindValue(":deletePrice", delPrice);
        if(qry.exec()){
            qDebug() << "Item deleted from database";
            cleanLineEdits(); // calling the function to clean the lineEdits
        }
        else{
           qDebug() << "Error in the database";
        }
          break;
    }
      case QMessageBox::Cancel:

        cleanLineEdits();
        break;

      default:
          // should never be reached
          break;

    }
    qry.prepare("SELECT * FROM project WHERE name LIKE :search OR proCode LIKE :search OR category LIKE :search OR quantity LIKE :search OR brand LIKE :search OR price LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearch));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }

    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(qry);
    ui->tableView->setModel(modal);

    int numberOfRows = 0;
    if(qry.last())
    {
        numberOfRows =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumber->display(numberOfRows);
    QString tempSearchProject;
    int tempIntProject;
    tempSearchProject = ui->lineEditSearchProject->text();
    tempIntProject = ui->lineEditSearchProject->text().toInt();
    qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal1 = new QSqlQueryModel();
    modal1->setQuery(qry);
    ui->tableViewProject->setModel(modal1);

    int numberOfRowsProject = 0;
    if(qry.last())
    {
        numberOfRowsProject =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject->display(numberOfRowsProject);

}


void MainWindow::on_btnClear_clicked()
{
    ui->lineEditName->setText("");
    ui->lineEditProCode->setText("");
    ui->lineEditCat->setText("");
    ui->lineEditQuantity->setText("");
    ui->lineEditBrand->setText("");
    ui->lineEditPrice->setText("");
}

void MainWindow::on_btnSearchProject_clicked()
{
    QSqlQuery qry(Rdb.db);
    QString tempSearchProject;
    int tempIntProject;
    tempSearchProject = ui->lineEditSearchProject->text();
    tempIntProject = ui->lineEditSearchProject->text().toInt();
    qry.prepare("SELECT * FROM projectNames WHERE name LIKE :search");
    qry.bindValue(":search", QString("%%1%").arg(tempSearchProject));
    //qry.bindValue(":tempInt", QString("%%1%").arg(tempInt));
    qry.exec();
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal1 = new QSqlQueryModel();
    modal1->setQuery(qry);
    ui->tableViewProject->setModel(modal1);

    int numberOfRowsProject = 0;
    if(qry.last())
    {
        numberOfRowsProject =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject->display(numberOfRowsProject);
}

void MainWindow::on_btnSearchProject_2_clicked()
{
    QSqlQuery qry(Rdb.db);
    QModelIndexList indexes2 = ui->tableViewProject->selectionModel()->selection().indexes();
    QString projectName;
    for (int i = 0; i < indexes2.count(); ++i)
    {
        QModelIndex index = indexes2.at(i);
        int row = index.row();
        projectName = index.sibling(row, 0).data().toString();
    }
    qry.prepare("SELECT * FROM " + projectName);
    if(qry.exec()){
        qDebug() << "Done.";
    }
    else{
        qDebug() << projectName + "table error.";
    }
    while(qry.next()){
        qDebug()<< qry.value(0) << qry.value(1);
    }
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(qry);
    ui->tableView3->setModel(modal2);

    int numberOfRowsProjectPro = 0;
    if(qry.last())
    {
        numberOfRowsProjectPro =  qry.at() + 1;
        qry.first();
        qry.previous();
    }

    ui->lcdNumberProject_2->display(numberOfRowsProjectPro);

}
