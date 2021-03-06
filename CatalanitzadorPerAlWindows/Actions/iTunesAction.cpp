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

#include "stdafx.h"

#include "iTunesAction.h"
#include "OSVersion.h"
#include "FileVersionInfo.h"

iTunesAction::iTunesAction(IRegistry* registry, IFileVersionInfo* fileVersionInfo)
{	
	m_fileVersionInfo = fileVersionInfo;
	m_registry = registry;
}

wchar_t* iTunesAction::GetName()
{
	return _getStringFromResourceIDName(IDS_ITUNESACTION_NAME, szName);	
}

wchar_t* iTunesAction::GetDescription()
{
	return _getStringFromResourceIDName(IDS_ITUNESACTION_DESCRIPTION, szDescription);
}

bool iTunesAction::IsNeed()
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
	g_log.Log(L"iTunesAction::IsNeed returns %u (status %u)", (wchar_t *) bNeed, (wchar_t*) GetStatus());
	return bNeed;
}

void iTunesAction::Execute()
{
	SetStatus(InProgress);

	_setDefaultLanguageForUser();
	if (_isDefaultLanguageForUser() == true)
		SetStatus(Successful);
	else
		SetStatus(FinishedWithError);
}

void iTunesAction::_getProgramLocation(wstring& location)
{
	wchar_t* ITUNES_REGKEY = L"SOFTWARE\\Apple Computer, Inc.\\iTunes";
	wchar_t szAppPath[MAX_PATH];
	wchar_t szProgFolder[MAX_PATH];
	
	if (m_registry->OpenKey(HKEY_LOCAL_MACHINE, ITUNES_REGKEY, false))
	{
		if (m_registry->GetString(L"ProgramFolder", szAppPath, sizeof(szAppPath)))
		{
			SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES|CSIDL_FLAG_CREATE,  NULL, 0, szProgFolder);
			location = szProgFolder;
			location += L"\\";
			location += szAppPath;
			location += L"iTunes.exe";
		}
		m_registry->Close();
	}
}

const wchar_t* iTunesAction::GetVersion()
{
	if (m_version.length() == 0)
	{
		_readVersionInstalled();
	}
	return m_version.c_str();
}

void iTunesAction::_readVersionInstalled()
{
	if (m_version.size() > 0)
		return;

	wstring location;

	_getProgramLocation(location);

	if (location.size() > 0)
	{
		m_fileVersionInfo->SetFilename(location);
		m_version = m_fileVersionInfo->GetVersion();
	}
}

int iTunesAction::_getMajorVersion()
{
	wstring location;

	_getProgramLocation(location);
	m_fileVersionInfo->SetFilename(location);
	return m_fileVersionInfo->GetMajorVersion();
}

#define ITUNES_USERKEY L"Software\\Apple Computer, Inc.\\iTunes"
#define CATALAN_LANGCODE L"1027"

bool iTunesAction::_isDefaultLanguage()
{
	TriBool defaultUser;
	wchar_t szLang[1024];

	defaultUser = _isDefaultLanguageForUser();

	if (defaultUser.IsUndefined()) 
	{		
		if (m_registry->OpenKey(HKEY_LOCAL_MACHINE, ITUNES_USERKEY, false))
		{
			if (m_registry->GetString(L"InstalledLangID", szLang, sizeof(szLang)))
			{
				g_log.Log(L"iTunesAction::_isDefaultLanguageForUser. Machine key %s", (wchar_t *) szLang);
				defaultUser = _wcsnicmp(szLang, CATALAN_LANGCODE, sizeof(CATALAN_LANGCODE)) == 0;
			}
			m_registry->Close();
		}
	}

	g_log.Log(L"iTunesAction::_isDefaultLanguage. Returns %u", (wchar_t *) (defaultUser == true));
	return defaultUser == true;
}

TriBool iTunesAction::_isDefaultLanguageForUser()
{
	wchar_t szLang[1024];
	TriBool defaultLanguage;

	if (m_registry->OpenKey(HKEY_CURRENT_USER, ITUNES_USERKEY, false))
	{
		if (m_registry->GetString(L"LangID", szLang, sizeof(szLang)))
		{
			g_log.Log(L"iTunesAction::_isDefaultLanguageForUser. User key %s", (wchar_t *) szLang);
			defaultLanguage = _wcsnicmp(szLang, CATALAN_LANGCODE, sizeof(CATALAN_LANGCODE)) == 0;
		}
		m_registry->Close();
	}
	g_log.Log(L"iTunesAction::_isDefaultLanguageForUser: %u", (wchar_t *) (defaultLanguage == true));
	return defaultLanguage;
}

bool iTunesAction::_setDefaultLanguageForUser()
{
	bool bInstalled = false;

	if (m_registry->OpenKey(HKEY_CURRENT_USER, ITUNES_USERKEY, true))
	{
		m_registry->SetString(L"LangID", CATALAN_LANGCODE);
		m_registry->Close();
	}
	return bInstalled;
}

#define FIRST_VERSION_WITH_CATALAN_SUPPORT L"10.6.3.0"

void iTunesAction::CheckPrerequirements(Action * action)
{
	_readVersionInstalled();
	
	if (m_version.size() > 0)
	{
		ApplicationVersion version(m_version);

		if (version < ApplicationVersion(FIRST_VERSION_WITH_CATALAN_SUPPORT))
		{
			_getStringFromResourceIDName(IDS_NOTSUPPORTEDVERSION, szCannotBeApplied);
			g_log.Log(L"iTunesAction::CheckPrerequirements. Version not supported");
			SetStatus(CannotBeApplied);
			return;
		}
			
		if (_isDefaultLanguage() == true)
		{
			SetStatus(AlreadyApplied);
			return;
		}
	}
	else
	{
		_setStatusNotInstalled();
		return;
	}
}
