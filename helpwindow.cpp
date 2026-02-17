#include <QtWidgets/QPushButton>
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
    ui->textHelp->setText("<h2>Operation mode</h2>"
                    "<h3>Normal Operation:</h3>"
                    "<p>Fill the freespace on the mounted card using f3write "
                    "and read back the contents with f3read to check for "
                    "corruption and determine the actual size of the card.</p>"
                    "<h3>Advanced Operation (requires root):</h3>"
                    "<ul>"
                    "<li>Quick Test - Uses f3probe to quickly check a card by "
                    "reading/writing directly to blocks. When this option is "
                    "unchecked, the card will be mounted and f3write/f3read "
                    "will be used to verify card.</li>"
                    "<li>Use less memory - Slower test with block restoration "
                    "but requires less memory. "
                    "<p><b>Note</b> in some cases (i.e. power is lost, "
                    "application is closed, drive is counterfeit) the blocks "
                    "might not fully/properly restored even if the destructive "
                    "option is not used.</p>"
                    "</li>"
                    "<li>Use cached data - Read *.h2w files that are already "
                    "on the card. Does not write new h2w files.</li>"
                    "<li>Destructive Test - Speed up the test and minimize "
                    "memory usage by skipping restoration of written blocks."
                    "</li>"
                    "</ul>");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
