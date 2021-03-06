/******************************************************************************
 * debugger.h - The qtpovray SDL debugger
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
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "rawtokenizer.h"
#include "backend/control/parsertask.h"
#include "parser.h"
#include "povdbgobject.h"

namespace pov_parser
{

/**
 * @brief The qtpovray SDL debugger.
 * Monitor the Parser and control its execution.
 * 
 * This class' lifetime is per-scene.
 */
class Debugger
{
public:
	Debugger();
	void init();
	
	void send(const char* text);
	void debuggerPaused();
	void checkForBreakpoint(const RawToken& rawToken);
	void setParser(Parser* parser) { mParser = parser; mDbgObjectFactory.setParser(parser); }
	void setParserTask(ParserTask* task) { mParserTask = task; }
	
	void messageFromGui(const char* msg);

private:
	void	commandWatchSymbol(const char* name);
	
	void	sendWatches();
	void	sendWatch(const char* name);

	PovDbgObjectFactory	mDbgObjectFactory;
	Parser*	mParser;
	ParserTask* mParserTask;
	int		mParseLine = -1;
	bool	mStepping = false;
};


}	// namespace pov_parser

#endif // DEBUGGER_H
