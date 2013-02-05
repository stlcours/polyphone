/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013 Davy Triponney                                     **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
****************************************************************************/

#include "page_sf2.h"
#include "ui_page_sf2.h"
#include "mainwindow.h"
#include <time.h>

Page_Sf2::Page_Sf2(MainWindow * mainWindow, Tree * tree, QStackedWidget * qStackedWidget, \
                   Pile_sf2 * sf2, Audio *audio, QWidget * parent) :
    Page(parent), ui(new Ui::Page_Sf2)
{
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    this->tree = tree;
    this->qStackedWidget = qStackedWidget;
    this->sf2 = sf2;
    this->audio = audio;
    this->preparation = 0;
}

Page_Sf2::~Page_Sf2()
{
    delete ui;
}

void Page_Sf2::afficher()
{
    // Préparation de l'affichage
    preparation = 1;
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    id.indexElt = 0;
    id.indexElt2 = 0;
    id.indexMod = 0;
    // Eléments non utilisés
    this->compte();
    // Mode 24 bits ?
    ui->check_24bits->setChecked(this->sf2->get(id, champ_wBpsSave).wValue == 24);
    // Informations
    ui->label_filename->setText(this->sf2->getQstr(id, champ_filename));
    char T[20];
    if (this->sf2->get(id, champ_IFIL).sfVerValue.wMinor < 10)
        sprintf(T, "%d.0%d", this->sf2->get(id, champ_IFIL).sfVerValue.wMajor, this->sf2->get(id, champ_IFIL).sfVerValue.wMinor);
    else
        sprintf(T, "%d.%d", this->sf2->get(id, champ_IFIL).sfVerValue.wMajor, this->sf2->get(id, champ_IFIL).sfVerValue.wMinor);
    ui->label_sfVersion->setText(T);
    ui->label_soundEngine->setText(this->sf2->getQstr(id, champ_ISNG));
    if (this->sf2->get(id, champ_IVER).sfVerValue.wMinor < 10)
        sprintf(T, "%d.0%d", this->sf2->get(id, champ_IVER).sfVerValue.wMajor, this->sf2->get(id, champ_IVER).sfVerValue.wMinor);
    else
        sprintf(T, "%d.%d", this->sf2->get(id, champ_IVER).sfVerValue.wMajor, this->sf2->get(id, champ_IVER).sfVerValue.wMinor);
    if (this->sf2->getQstr(id, champ_IROM).isEmpty())
        ui->label_romVersion->setText(QString("- (version ").append(T).append(")"));
    else
        ui->label_romVersion->setText(this->sf2->getQstr(id, champ_IROM).append(" (version ").append(T).append(")"));
    ui->label_software->setText(this->sf2->getQstr(id, champ_ISFT));
    ui->lineEdit_name->setText(this->sf2->getQstr(id, champ_name));
    ui->lineEdit_copyright->setText(this->sf2->getQstr(id, champ_ICOP));
    ui->lineEdit_author->setText(this->sf2->getQstr(id, champ_IENG));
    ui->lineEdit_date->setText(this->sf2->getQstr(id, champ_ICRD));
    ui->lineEdit_product->setText(this->sf2->getQstr(id, champ_IPRD));
    ui->textEdit_Com->setPlainText(this->sf2->getQstr(id, champ_ICMT));
    // Basculement de l'affichage
    this->qStackedWidget->setCurrentWidget(this);
    preparation = 0;
}

void Page_Sf2::set24bits(int checked)
{
    if (preparation) return;
    sf2->prepareNewActions();
    // Configuration d'un SF2 : mode 16 ou 24 bits
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    Valeur valTmp;
    if (checked == 2)
        valTmp.wValue = 24;
    else
        valTmp.wValue = 16;
    this->sf2->set(id, champ_wBpsSave, valTmp);
    this->mainWindow->updateDo();
}

void Page_Sf2::setName()
{
    if (preparation) return;
    ui->lineEdit_name->setText(ui->lineEdit_name->text().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->lineEdit_name->text().compare(this->sf2->getQstr(id, champ_name)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_name, ui->lineEdit_name->text());
    this->mainWindow->updateDo();
}
void Page_Sf2::setCopyright()
{
    if (preparation) return;
    ui->lineEdit_copyright->setText(ui->lineEdit_copyright->text().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->lineEdit_copyright->text().compare(this->sf2->getQstr(id, champ_ICOP)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_ICOP, ui->lineEdit_copyright->text());
    this->mainWindow->updateDo();
}
void Page_Sf2::setAuthor()
{
    if (preparation) return;
    ui->lineEdit_author->setText(ui->lineEdit_author->text().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->lineEdit_author->text().compare(this->sf2->getQstr(id, champ_IENG)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_IENG, ui->lineEdit_author->text());
    this->mainWindow->updateDo();
}
void Page_Sf2::setDate()
{
    if (preparation) return;
    ui->lineEdit_date->setText(ui->lineEdit_date->text().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->lineEdit_date->text().compare(this->sf2->getQstr(id, champ_ICRD)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_ICRD, ui->lineEdit_date->text());
    this->mainWindow->updateDo();
}
void Page_Sf2::setProduct()
{
    if (preparation) return;
    ui->lineEdit_product->setText(ui->lineEdit_product->text().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->lineEdit_product->text().compare(this->sf2->getQstr(id, champ_IPRD)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_IPRD, ui->lineEdit_product->text());
    this->mainWindow->updateDo();
}
void Page_Sf2::setCommentaire()
{
    if (preparation) return;
    ui->textEdit_Com->setPlainText(ui->textEdit_Com->toPlainText().left(255));
    EltID id = this->tree->getID(0);
    id.typeElement = elementSf2;
    if (ui->textEdit_Com->toPlainText().compare(this->sf2->getQstr(id, champ_ICMT)) == 0) return;
    sf2->prepareNewActions();
    // Reprise de l'identificateur si modification
    id = this->tree->getID(0);
    id.typeElement = elementSf2;
    this->sf2->set(id, champ_ICMT, ui->textEdit_Com->toPlainText());
    this->mainWindow->updateDo();
}

void Page_Sf2::setNow()
{
    const char * NomMois[] = {tr("janvier").toStdString().c_str(),
                              tr("février").toStdString().c_str(),
                              tr("mars").toStdString().c_str(),
                              tr("avril").toStdString().c_str(),
                              tr("mai").toStdString().c_str(),
                              tr("juin").toStdString().c_str(),
                              tr("juillet").toStdString().c_str(),
                              tr("août").toStdString().c_str(),
                              tr("septembre").toStdString().c_str(),
                              tr("octobre").toStdString().c_str(),
                              tr("novembre").toStdString().c_str(),
                              tr("décembre").toStdString().c_str()};
    char T[30];
    time_t timestamp = time(NULL);
    struct tm * t = localtime(&timestamp);
    sprintf(T, "%u %s %u, %02u:%02u:%02u", t->tm_mday, NomMois[t->tm_mon], 1900 + t->tm_year, \
            t->tm_hour, t->tm_min, t->tm_sec);
    ui->lineEdit_date->setText(QString::fromUtf8(T));
    setDate();
}

// METHODES PRIVEES

void Page_Sf2::compte()
{
    if (this->tree->getSelectedItemsNumber() == 0) return;
    if (!this->tree->isSelectedItemsSf2Unique()) return;
    EltID id = this->tree->getID(0);
    EltID id2 = this->tree->getID(0);
    // Compte des samples, instruments et presets utilisés et non utilisés
    int unusedSmpl = 0;
    int unusedInst = 0;
    int usedSmpl = 0;
    int usedInst = 0;
    int usedPrst = 0;
    int instGen = 0;
    int prstGen = 0;
    id.typeElement = elementSmpl;
    int nbSmpl = sf2->count(id);
    id.typeElement = elementInst;
    int nbInst = sf2->count(id);
    id.typeElement = elementPrst;
    int nbPrst = sf2->count(id);
    bool smplUsed, instUsed;
    int nbPrstInst, nbInstSmpl;
    // Nombre de samples non utilisés
    for (int i = 0; i < nbSmpl; i++)
    {
        // le sample ne doit pas être masqué
        id.typeElement = elementSmpl;
        id.indexElt = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            smplUsed = 0;
            // on regarde chaque instrument
            for (int j = 0; j < nbInst; j++)
            {
                // l'instrument ne doit pas être masqué
                id.typeElement = elementInst;
                id.indexElt = j;
                if (!sf2->get(id, champ_hidden).bValue)
                {
                    // on regarde les InstSmpl
                    id.typeElement = elementInstSmpl;
                    id.indexElt = j;
                    nbInstSmpl = sf2->count(id);
                    for (int l = 0; l < nbInstSmpl; l++)
                    {
                        id.indexElt2 = l;
                        if (!sf2->get(id, champ_hidden).bValue)
                            if (sf2->get(id, champ_sampleID).wValue == i) smplUsed = true;
                    }
                }
            }
            if (!smplUsed) unusedSmpl++;
        }
    }

    // Nombre d'instruments non utilisés
    for (int i = 0; i < nbInst; i++)
    {
        // l'instrument ne doit pas être masqué
        id.typeElement = elementInst;
        id.indexElt = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            instUsed = 0;
            // on regarde chaque preset
            for (int j = 0; j < nbPrst; j++)
            {
                // le preset ne doit pas être masqué
                id.typeElement = elementPrst;
                id.indexElt = j;
                if (!sf2->get(id, champ_hidden).bValue)
                {
                    // on regarde les PrstInst
                    id.typeElement = elementPrstInst;
                    id.indexElt = j;
                    nbPrstInst = sf2->count(id);
                    for (int l = 0; l < nbPrstInst; l++)
                    {
                        id.indexElt2 = l;
                        if (!sf2->get(id, champ_hidden).bValue)
                            if (sf2->get(id, champ_instrument).wValue == i) instUsed = true;
                    }
                }
            }
            if (!instUsed) unusedInst++;
        }
    }

    // Nombre de smpl, inst, prst et gen
    for (int i = 0; i < nbSmpl; i++)
    {
        // le sample ne doit pas être masqué
        id.typeElement = elementSmpl;
        id.indexElt = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            usedSmpl++;
        }
    }
    for (int i = 0; i < nbInst; i++)
    {
        // l'instrument ne doit pas être masqué
        id.typeElement = elementInst;
        id.indexElt = i;
        id2.indexElt = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            usedInst++;
            // Inst Gen
            id2.typeElement = elementInstGen;
            instGen += sf2->count(id2);
            id2.typeElement = elementInstSmpl;
            for (int j = 0; j < sf2->count(id2); j++)
            {
                id2.indexElt2 = j;
                if (!sf2->get(id2, champ_hidden).bValue)
                {
                    id2.typeElement = elementInstSmplGen;
                    instGen += sf2->count(id2);
                    id2.typeElement = elementInstSmpl;
                }
            }
        }
    }
    for (int i = 0; i < nbPrst; i++)
    {
        // le preset ne doit pas être masqué
        id.typeElement = elementPrst;
        id.indexElt = i;
        id2.indexElt = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            usedPrst++;
            // Inst Gen
            id2.typeElement = elementPrstGen;
            prstGen += sf2->count(id2);
            id2.typeElement = elementPrstInst;
            for (int j = 0; j < sf2->count(id2); j++)
            {
                id2.indexElt2 = j;
                if (!sf2->get(id2, champ_hidden).bValue)
                {
                    id2.typeElement = elementPrstInstGen;
                    prstGen += sf2->count(id2);
                    id2.typeElement = elementPrstInst;
                }
            }
        }
    }
    // Affichage
    char T[30];
    sprintf(T,"%d", unusedSmpl);
    this->ui->label_unusedSmpl->setText(QString::fromUtf8(T));
    if (unusedSmpl)
        this->ui->label_unusedSmpl->setStyleSheet("QLabel { color : red; font-weight: bold;}");
    else
        this->ui->label_unusedSmpl->setStyleSheet("QLabel { color : black; font-weight: normal;}");
    sprintf(T,"%d", unusedInst);
    this->ui->label_unusedInst->setText(QString::fromUtf8(T));
    if (unusedInst)
        this->ui->label_unusedInst->setStyleSheet("QLabel { color : red; font-weight: bold;}");
    else
        this->ui->label_unusedInst->setStyleSheet("QLabel { color : black; font-weight: normal;}");
    sprintf(T,"%d", usedSmpl);
    this->ui->label_nbSmpl->setText(QString::fromUtf8(T));
    sprintf(T,"%d", usedInst);
    this->ui->label_nbInst->setText(QString::fromUtf8(T));
    sprintf(T,"%d", usedPrst);
    this->ui->label_nbPrst->setText(QString::fromUtf8(T));
    sprintf(T,"%d", instGen);
    this->ui->label_nbInstGen->setText(QString::fromUtf8(T));
    sprintf(T,"%d", prstGen);
    this->ui->label_nbPrstGen->setText(QString::fromUtf8(T));
}
