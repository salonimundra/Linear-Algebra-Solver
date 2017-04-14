#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVector>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool validate(QStringList pieces)
{
    bool x= true;
    int i;
    for(i=0;i<pieces.size();i++)
    {
        for(int j=0;j<pieces[i].size();j++)
        {
            if(pieces[i][j]==' '|| pieces[i][j]=='.'|| pieces[i][j]=='-'|| (pieces[i][j]<='9'&& pieces[i][j]>='0'))
            {

            }
            else
            {
                return(false);
            }
        }
    }
    return(true);
}

QVector<double> gauss(QVector< QVector<double> > A) {
    int n = A.size();

    for (int i=0; i<n; i++) {
        // Search for maximum in this column
        double maxEl = abs(A[i][i]);
        int maxRow = i;
        for (int k=i+1; k<n; k++) {
            if (abs(A[k][i]) > maxEl) {
                maxEl = abs(A[k][i]);
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for (int k=i; k<n+1;k++) {
            double tmp = A[maxRow][k];
            A[maxRow][k] = A[i][k];
            A[i][k] = tmp;
        }

        // Make all rows below this one 0 in current column
        for (int k=i+1; k<n; k++) {
            if(A[i][i]!=0)
            {
                double c = -A[k][i]/A[i][i];
                for (int j=i; j<n+1; j++) {
                    if (i==j) {
                        A[k][j] = 0;
                    } else {
                        A[k][j] += c * A[i][j];
                    }
                }
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    QVector<double> x(n);
    QVector<double> empty(0);
    QVector<double> empty1(1);
    for (int i=n-1; i>=0; i--) {
        if(A[i][i]==0)
        {
            if(A[i][n]==0)
            {

                return empty;
            }
            else
            {

                empty1[0]=-1;
                return empty1;
            }
        }
        else
        {
            x[i] = A[i][n]/A[i][i];
            for (int k=i-1;k>=0; k--) {
                A[k][n] -= A[k][i] * x[i];
            }
        }
    }


    return x;
}


void MainWindow::on_solveButton_clicked()
{
    QString strA= ui->Atext->toPlainText();
    QString strB= ui->Btext->toPlainText();
    QString strN=(ui->ntext->text());
    int n=strN.toInt();

    QStringList pieces =strA.split('\n');
    QStringList piecesA;
    QStringList piecesB;

    for(int i = 0; i < pieces.size(); i++) {
        if(pieces[i] != "") {
            piecesA.append(pieces[i]);
        }
    }

    pieces =strB.split('\n');

    for(int i = 0; i < pieces.size(); i++) {
        if(pieces[i] != "") {
            piecesB.append(pieces[i]);
        }
    }

    bool x;
    int countrows=0,countcolumns=0;

    if(strA.size() == 0 || strB.size() == 0 || strN.size() == 0)
    {
        ui->error->setText("Enter all fields");
        x = false;
    }

    else if(validate(piecesA)&&validate(piecesB))
    {
        ui->error->setText("");
        x=true;
    }
    else
    {
        ui->error->setText("Error: Enter only numeric matrix");
        ui->ntext->setText("");
        ui->Atext->setText("");
        ui->Btext->setText("");
        ui->Xtext->setText("");
        x=false;
    }
    if(x)
    {
        QVector<double> line(n+1,0);
        QVector< QVector<double> > A(n,line);
        double arr[n*n];
        int m=0, flag=0;
        for (int i=0; i<piecesA.size(); i++) {
            QStringList valuesA=piecesA[i].split(' ');

            if(piecesA[i].size()!=0)
            {
                countrows++;
            }
            countcolumns=0;

            for(int j=0;j<valuesA.size();j++)
            {

                if(valuesA[j].size()!=0)
                {
                    double r= valuesA[j].toDouble();

                    arr[m++]=r;

                    countcolumns++;
                }


            }
            if(countcolumns!=n)
            {
                flag=1;
            }
        }



        double a[n];
        int s=0;
        for (int i=0; i<piecesB.size(); i++) {
            QStringList valuesB=piecesB[i].split(' ');

            for(int j=0;j<valuesB.size();j++)
            {

                if(valuesB[j].size()!=0)
                {
                    double r= valuesB[j].toDouble();

                    a[s]=r;
                    s++;


                }


            }
        }

        if(flag==0 && countrows==n && s==n)
        {

            m=0;

            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    A[i][j]=arr[m++];
                }
            }
            m=0;
            for(int i=0;i<n;i++)
            {
                A[i][n]=a[m++];
            }


            QVector<double> x(n);
            x=gauss(A);
            if(x.size()==0)
            {
                ui->error->setText("Infinite Solutions Exist");
                ui->Xtext->setText("");
            }
            if(x.size()==1 && x[0]==-1)
            {
                ui->error->setText("No Solution Exists");
                ui->Xtext->setText("");
            }
            if(x.size()!=0 && x.size()!=1)
            {
                ui->Xtext->setText("");
                for (int i=0; i<n; i++) {
                    QString valueAsString = QString::number(x[i]);
                    ui->Xtext->append(valueAsString);
                }

            }
        }
        else
        {
            ui->error->setText("Enter Valid Input");
            ui->ntext->setText("");
            ui->Atext->setText("");
            ui->Btext->setText("");
            ui->Xtext->setText("");
        }
    }

}
