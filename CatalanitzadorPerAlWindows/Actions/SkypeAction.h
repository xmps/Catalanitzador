/* 
 * Copyright (C) 2013 Jordi Mas i Hern�ndez <jmas@softcatala.org>
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
#include "IRegistry.h"
#include "IFileVersionInfo.h"
#include "SkypeProfile.h"

class SkypeAction : public Action
{
public:	

		SkypeAction(IRegistry* registry, IFileVersionInfo* fileVersionInfo);

		virtual wchar_t* GetName();
		virtual wchar_t* GetDescription();
		virtual ActionID GetID() const { return SkypeActionID;};
		virtual ActionGroup GetGroup() const {return ActionGroupInternet;}
		virtual bool IsDownloadNeed() {return false;}
		virtual bool IsNeed();
		virtual void Execute();
		virtual void CheckPrerequirements(Action * action);
		virtual const wchar_t* GetVersion();

protected:
		
		bool _isDefaultInstallLanguage();
		bool _isDefaultLanguage();
		void _setDefaultLanguage();
		void _getProgramLocation(wstring& location);

private:
	
		int _getVersionInstalled();
		
		void _readVersionInstalled();
		int _getMajorVersion();
		void _setDefaultInstallLanguage();

		wstring m_version;
		int m_majorVersion;
		IFileVersionInfo* m_fileVersionInfo;
		IRegistry* m_registry;
		SkypeProfile m_profile;
};

