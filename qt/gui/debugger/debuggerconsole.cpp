/******************************************************************************
 * debuggerconsole.cpp - The Widget to control POV-Ray SDL debugging
 *
 * qtpovray - A Qt IDE frontend for POV-Ray
 * Copyright(c) 2020 - Dick Balaska, and BuckoSoft.
 *
 * qtpovray is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * qtpovray is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#include <QLabel>
#include <QToolBar>
#include <QDebug>

#include "debuggerpanel.h"
#include "debuggerconsole.h"
#include "debuggerman.h"

DebuggerConsole::DebuggerConsole(QTabWidget* parent, QStackedWidget* consoleBar, MainWindow* mainWindow)
	: QSplitter(Qt::Horizontal, parent),
	  m_mainWindow(mainWindow)
{
	m_debuggerPanel = new DebuggerPanel(this, mainWindow);
	this->addWidget(m_debuggerPanel);
	m_debuggerTabs = new QTabWidget(this);
	m_debuggerOptions = new DebuggerOptions(m_debuggerTabs, mainWindow);
	m_debuggerTabs->addTab(m_debuggerOptions, "Options");
	m_includeStack = new IncludeStack(m_debuggerTabs, mainWindow);
	m_debuggerTabs->addTab(m_includeStack, "IncStack");
	m_breakpointsWidget = new BreakpointsWidget(m_debuggerTabs, mainWindow);
	m_debuggerTabs->addTab(m_breakpointsWidget, "Breakpoints");
	
	this->addWidget(m_debuggerTabs);
	consoleBar->addWidget(new QLabel());	// just a dummy to blank the bar
}

IncludeStack::IncludeStack(QTabWidget* parent, MainWindow* mainWindow)
	: QListWidget(parent),
	  m_mainWindow(mainWindow)
{
	
}

DebuggerOptions::DebuggerOptions(QTabWidget* parent, MainWindow* mainWindow)
	: QWidget(parent),
	  m_mainWindow(mainWindow)
{
	
}

BreakpointsWidget::BreakpointsWidget(QTabWidget* parent, MainWindow* mainWindow)
	: QTableWidget(parent),
	  m_mainWindow(mainWindow)
{
	this->setColumnCount(3);
	setHorizontalHeaderLabels(QStringList({"Enabled", "#", "File"}));
}

void BreakpointsWidget::addBreakpoint(Breakpoint* bp)
{
	int row = this->rowCount();
	this->setRowCount(row+1);
	QTableWidgetItem* twi;
	twi = new QTableWidgetItem();
	twi->setCheckState(bp->m_enabled ? Qt::Checked : Qt::Unchecked);
	this->setItem(row, 0, twi);
	twi = new QTableWidgetItem(QString("%1").arg(bp->m_lineNumber));
	twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	this->setItem(row, 1, twi);
	twi  = new QTableWidgetItem(bp->m_pathName);
	twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	this->setItem(row, 2, twi);
}

void BreakpointsWidget::removeBreakpoint(Breakpoint* bp)
{
	for (int row = 0; row < this->rowCount(); row++) {
		QTableWidgetItem* twi = this->item(row, 2);
		if (twi->text() == bp->m_pathName) {
			twi = this->item(row, 1);
			int line = twi->text().toInt();
			if (line == bp->m_lineNumber) {
				this->removeRow(row);
				return;
			}
		}
	}
	qWarning() << "BreakpointsWidget: failed to remove Breakpoint" << bp->m_pathName << bp->m_lineNumber;
}
