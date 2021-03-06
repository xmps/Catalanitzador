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

#include "ConfigurationFileActionDownloads.h"
#include "ActionID.h"
#include <string>

using namespace std;

class ConfigurationRemote
{
	public:
			ConfigurationRemote();
			
			wstring& GetCompatibility() {return m_compatibility;}
			void SetCompatibility(wstring compatibility) {m_compatibility = compatibility;}

			_LARGE_INTEGER GetDateTime() {return m_datetime;}
			void SetDateTime(_LARGE_INTEGER datetime) {m_datetime = datetime;}

			vector <ConfigurationFileActionDownloads>& GetFileActionsDownloads() {return m_fileActionsDownloads;}

			int AddFileActionDownloads(ConfigurationFileActionDownloads fileDownloads);			
			ConfigurationFileActionDownload& GetDownloadForActionID(ActionID actionID, ApplicationVersion version);
			ConfigurationFileActionDownload& GetDownloadForActionID(ActionID actionID, wstring version);

			bool IsOlderThan(ConfigurationRemote other);
			
	private:
			
			wstring m_compatibility;
			_LARGE_INTEGER m_datetime;
			vector <ConfigurationFileActionDownloads> m_fileActionsDownloads;	
};
