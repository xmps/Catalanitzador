/* 
 * Copyright (C) 2011 Jordi Mas i Hern�ndez <jmas@softcatala.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

#pragma once

#include <windows.h>

enum ActionResult
{
	NotStarted,
	InProgress,
	Successfull,
	FinishedWithError
};

class Action
{
public:
		virtual wchar_t* GetName() = 0;
		virtual wchar_t* GetDescription() = 0;
		virtual bool Download() {return false;}
		virtual bool IsNeed() = 0;
		virtual void Execute() = 0;
		virtual ActionResult Result() = 0;
		// TODO: This action requiere reboot

protected:
		wchar_t* GetStringFromResourceIDName(int nID, wchar_t* string);

		TCHAR szName[MAX_LOADSTRING];
		TCHAR szDescription[MAX_LOADSTRING];
};

