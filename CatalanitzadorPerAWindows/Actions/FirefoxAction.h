/* 
 * Copyright (C) 2012 Jordi Mas i Hern�ndez <jmas@softcatala.org>
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
#include "OSVersion.h"
#include "IRegistry.h"
#include "IOSVersion.h"

#include <vector>
#include <algorithm>

using namespace std;

class _APICALL FirefoxAction : public Action
{
public:
		FirefoxAction (IRegistry* registry);

		virtual wchar_t* GetName();
		virtual wchar_t* GetDescription();
		virtual int GetID() { return Firefox;}
		virtual bool IsDownloadNeed() {return false;}
		virtual bool IsNeed();
		virtual void Execute();
		virtual DWORD GetProcessIDForRunningApp();

		void ParseLanguage(wstring regvalue);
		void AddCatalanToArrayAndRemoveOldIfExists();
		void CreatePrefsString(wstring& string);
		vector <wstring> * GetLanguages() {return &m_languages;}

private:
		bool _readLanguageCode(wstring& langcode);
		void _getFirstLanguage(wstring& regvalue);
		void _writeLanguageCode(wstring &langcode);
		void _getPrefLine(wstring langcode, wstring& line);
		
		void _getProfilesIniLocation(wstring &location);
		void _getPreferencesFile(wstring &location);
		void _getProfileLocationFromProfilesIni(wstring file, wstring &profileLocation);

		IRegistry* m_registry;
		vector <wstring> m_languages;
};
