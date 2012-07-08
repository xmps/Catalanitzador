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

#include "stdafx.h"
#include <Shlobj.h>

#include "WindowsLiveAction.h"
#include "Winver.h"
#include "FileVersionInfo.h"

WindowsLiveAction::WindowsLiveAction(IRegistry* registry, IRunner* runner)
{
	m_registry = registry;
	m_runner = runner;
}

wchar_t* WindowsLiveAction::GetName()
{	
	return _getStringFromResourceIDName(IDS_WINDOWSLIVE_NAME, szName);	
}

wchar_t* WindowsLiveAction::GetDescription()
{
	return _getStringFromResourceIDName(IDS_WINDOWSLIVE_DESCRIPTION, szName);
}

bool WindowsLiveAction::IsNeed()
{
	bool bNeed;

	switch(GetStatus())
	{		
		case NotInstalled:
		case AlreadyApplied:
		case CannotBeApplied:
			bNeed = false;
			break;
		default:
			bNeed = true;
			break;
	}

	g_log.Log(L"WindowsLiveAction::IsNeed returns %u (status %u)", (wchar_t *) bNeed, (wchar_t*) GetStatus());
	return bNeed;
}

bool WindowsLiveAction::Download(ProgressStatus progress, void *data)
{
	// The installer downloads the language packs. We indicate that the actions
	// downloads but is delagated to the installer (as this the internet connection
	// detection is checked for this action)
	return true;
}

const wchar_t* WindowsLiveAction::GetVersion()
{
	if (m_version.length() == 0)
	{
		_readVersionInstalled();
	}
	return m_version.c_str();
}

void WindowsLiveAction::_readVersionInstalled()
{
	if (m_version.size() > 0)
		return;

	wstring location;

	_getInstallerLocation(location);
	FileVersionInfo fileVersion(location);
	fileVersion.ReadVersion(m_version);
}

void WindowsLiveAction::_getInstallerLocation(wstring& location)
{
	wchar_t szPath[MAX_PATH];

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES|CSIDL_FLAG_CREATE,  NULL, 0, szPath);
	location = szPath;
	location += L"\\Windows Live\\Installer\\wlarp.exe";
}

void WindowsLiveAction::Execute()
{
	wchar_t szParams[MAX_PATH] = L"";
	wchar_t szApp[MAX_PATH] = L"";
	wstring location;

	_getInstallerLocation(location);
	
	wcscpy_s(szApp, location.c_str());
	wcscat_s(szApp, L" -install -language:ca /quiet");
	SetStatus(InProgress);
	g_log.Log(L"WindowsLiveAction::Execute '%s' with params '%s'", szApp, szParams);
	m_runner->Execute(NULL, szApp);	
}

ActionStatus WindowsLiveAction::GetStatus()
{
	if (status == InProgress)
	{
		if (m_runner->IsRunning())
			return InProgress;

		if (_isLangSelected())
		{
			SetStatus(Successful);
		}
		else
		{
			SetStatus(FinishedWithError);
		}
		
		g_log.Log(L"WindowsLiveAction::GetStatus is '%s'", status == Successful ? L"Successful" : L"FinishedWithError");
	}
	return status;
}

int WindowsLiveAction::_getMajorVersion()
{
	size_t pos;

	pos = m_version.find_first_of(L".", 0);

	if (pos == string::npos)
	{
		return -1;
	}

	return _wtoi(m_version.substr(0, pos).c_str());
}

#define LANG_REGKEY L"Software\\Microsoft\\Windows Live\\Common\\"
#define LANGUAGE_CODE L"CA"

bool WindowsLiveAction::_isLangSelected()
{
	bool bSelected = false;

	if (m_registry->OpenKey(HKEY_CURRENT_USER, LANG_REGKEY, false))
	{
		wchar_t szLanguage[MAX_PATH];

		if (m_registry->GetString(L"UserLanguage", szLanguage, sizeof(szLanguage)))
		{
			bSelected = _wcsnicmp(szLanguage, LANGUAGE_CODE, wcslen(LANGUAGE_CODE)) == 0;
		}
		m_registry->Close();
	}

	return bSelected;
}


void WindowsLiveAction::CheckPrerequirements(Action * action)
{
	_readVersionInstalled();

	if (m_version.size() > 0)
	{
		if (_isLangSelected() == true)
		{
			SetStatus(AlreadyApplied);
			return;
		}

		int major = _getMajorVersion();
		if (major != 15)
		{
			_getStringFromResourceIDName(IDS_NOTSUPPORTEDVERSION, szCannotBeApplied);
			g_log.Log(L"WindowsLiveAction::CheckPrerequirements. Version not supported");
			SetStatus(CannotBeApplied);
			return;
		}
	}
	else
	{
		_setStatusNotInstalled();
		return;
	}
}
