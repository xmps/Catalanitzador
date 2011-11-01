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

#include "Action.h"
#include "Runner.h"

class WindowsLPIAction : public Action
{
public:
		WindowsLPIAction ();
		~WindowsLPIAction ();

		virtual wchar_t* GetName();
		virtual wchar_t* GetDescription();
		virtual int GetID() { return WindowsLPI;};
		virtual bool Download(ProgressStatus progress, void *data);
		virtual bool IsNeed();
		virtual bool IsRebootNeed();
		virtual void Execute(ProgressStatus progress, void *data);
		virtual ActionStatus GetStatus();

private:
		static BOOL CALLBACK _enumUILanguagesProc(LPTSTR lpUILanguageString, LONG_PTR lParam);
		static VOID CALLBACK _timerProc(HWND hwnd, UINT uMsg,  UINT_PTR idEvent, DWORD dwTime);
		bool _directoryExists(LPCTSTR szPath);
		wchar_t* _getPackageName();
		void _updateIsInstalled();
		void _setDefaultLanguage();
		bool _wasLIPInstalled();

		bool m_installed;
		wchar_t filename[MAX_PATH];		
		Runner runner;
		UINT hTimerID;
};

