#include <QPushButton>
#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    ui->labelVersion->setText(QString("Ver: ").append(APP_VERSION));
    ui->textAbout->setText("This program is a free software.\n\n"
                    "You can redistribute it and/or modify it under the terms of "
                    "the GNU Library General Public License as published by "
                    "the Free Software Foundation; either version 3 of the License, "
                    "or (at your option) any later version.\n\n"
                    "This package is distributed in the hope that it will be useful, but WITHOUT "
                    "ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
                    "FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public License "
                    "for more details.\n");
    ui->labelContact->setText("<p>GUI Author: Tianze Wang &lt;<a href=\"mailto:zwpwjwtz@126.com\">zwpwjwtz@126.com</a>&gt;</p>"
                    "<p>Author of F3: Michel Machado &lt;<a href=\"mailto:michel@digirati.com.br\">michel@digirati.com.br</a>&gt;</p>"
                    "<p>Project Home: <a href=\"https://github.com/zwpwjwtz/f3-qt\">Github</a>"
                    "<p align=\"center\">Feel free to report bugs and give suggestions!</p>");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
