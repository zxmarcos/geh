// =============================================================================
// Copyright (C) 2013    Marcos Medeiros
//
// This file is part of "SGEH - Sistema Gerador de Erro Humano"
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// =============================================================================
#include <QtWidgets>
#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);

	m_cnpqLogo = new QPixmap(tr(":/images/logo_cnpq.png"));
	m_femaLogo = new QPixmap(tr(":/images/logo_fema.png"));

	setWindowTitle(tr("Sobre"));
#define NL	"<br/>"
	QLabel *text = new QLabel(tr(
		"<h1>Sistema Gerador de Erro Humano</h1>" NL
		"<br/> Esse sistema foi desenvolvido com o propósito de ajudar os" NL
		"desenvolvedores de interface gráfica a identificarem erros que podem ocorrer" NL
		"no momento em que haja inteção-homem-máquina, isso é, as possibilidades" NL
		"de falha humana que podem ocorrer." NL
		"<br/>Desenvolvido por Marcos Medeiros, orientado de Dr Luiz Carlos Begosso," NL
		"na FEMA - Instituição Educacional do Município de Assis, com apoio do CNPq, " NL
        "através do programa de bolsas PIBIT 2013/2014/2015." NL NL
        "Esse programa é software livre, licenciado sobre GPLv3, uma cópia da licença" NL
        "pode ser obtida em:" NL
        "<a href=\"http://www.gnu.org/licenses/gpl.html\">http://www.gnu.org/licenses/gpl.html</a>" NL NL
        "Os ícones fazem parte do pacote Silk Icon de Mark James e são distribuidos sobre" NL
        "a licença Creative Commons." NL NL
		"Links úteis:" NL
		"<a href=\"http://fema.edu.br/\">FEMA - Fundação Educacional do Município de Assis</a>" NL
        "<a href=\"http://www.cnpq.br/\">CNPq - Conselho Nacional de Pesquisa e Desenvolvimento</a>" NL NL
        "<a href=\"http://www.famfamfam.com/lab/icons/silk/\">Silk icon set 1.3 (Mark James)</a>" NL NL
		));

#undef NL

	QPushButton *button = new QPushButton(tr("&Ok"));
	
	connect(button, SIGNAL(clicked()), this, SLOT(close()));

	QLabel *fema = new QLabel;
	QLabel *cnpq = new QLabel;
	cnpq->setPixmap(*m_cnpqLogo);
	fema->setPixmap(*m_femaLogo);

	QVBoxLayout *left = new QVBoxLayout;
	left->addWidget(fema);
	left->addWidget(cnpq);
	left->addStretch();

	QVBoxLayout *right = new QVBoxLayout;
	right->addWidget(text);
	right->addStretch();


	QHBoxLayout *center = new QHBoxLayout;
	center->addLayout(left);
	center->addLayout(right);
	center->addStretch();

	QHBoxLayout *panel = new QHBoxLayout;
	panel->addStretch();
	panel->addWidget(button);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(center);
	layout->addLayout(panel);

	setLayout(layout);
    adjustSize();
    setFixedSize(size());
}

AboutDialog::~AboutDialog()
{
	delete m_cnpqLogo;
	delete m_femaLogo;
}
