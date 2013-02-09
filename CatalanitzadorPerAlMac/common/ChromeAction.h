/* 
 * Copyright (C) 2012 Xavier Ivars-Ribes <xavi.ivars@gmail.com>
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
#include <vector>
#include <algorithm>

using namespace std;


class ChromeAction : public Action
{
public:
		ChromeAction();

		virtual bool IsNeed();
		virtual void Execute();
	
		void ParseLanguage(string regvalue);
		void CreateJSONString(string &regvalue);
		void AddCatalanToArrayAndRemoveOldIfExists();
		vector <string> * GetLanguages() {return &m_languages;}

protected:
		
		bool _isInstalled();
		bool _readLanguageCode(string& langcode);

private:		
		
		void _getFirstLanguage(string& regvalue);
		void _readInstallLocation(string& path);
		bool _writeAcceptLanguageCode(string langcode);
		bool _writeUILocale(string langcode);
		bool _isUILocaleOk();

		bool _findIntl(string,int&);
		bool _findSemicolon(string,int&);
		bool _findStartBlock(string,int&);
		bool _findAcceptedKey(string,int&);
		bool _findAcceptedValue(string,int&);
		bool _findLanguageString(string,int &,string &);
		bool _findAppLocaleKey(string line, int & pos);

		//string m_version;
		vector <string> m_languages;
		//TriBool m_isInstalled;
		ActionStatus uiStatus;
};

