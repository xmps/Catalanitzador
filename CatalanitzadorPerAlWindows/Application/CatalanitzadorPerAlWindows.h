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

#include "stdafx.h"
#include "Serializer.h"
#include "OSVersion.h"
#include "Actions.h"
#include "DownloadManager.h"

#include <string>
using namespace std;

class CatalanitzadorPerAWindows
{
	
public:
		CatalanitzadorPerAWindows(HINSTANCE hInstance);
		~CatalanitzadorPerAWindows();

		void Run(wstring commandLine);

private:

		void _initLog();
		bool _supportedOS();
		void _createWizard();
		bool _isAlreadyRunning();
		bool _hasAdminPermissionsDialog();
		
		HINSTANCE m_hInstance;
		HANDLE m_hEvent;
		Serializer m_serializer;
		OSVersion m_osVersion;
		DownloadManager m_downloadManager;
		Actions m_actions;
};
