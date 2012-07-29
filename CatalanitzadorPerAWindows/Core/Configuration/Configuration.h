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

#include "ConfigurationRemote.h"
#include "TriBool.h"
#include "OSVersion.h"
#include <string>

using namespace std;

class Configuration
{
	public:
			ConfigurationRemote& GetRemote() {return m_remote;}
			void SetRemote(ConfigurationRemote remote) {m_remote = remote;}

			// Application Configuraiton
			
			void SetAeroEnabled(bool bUseAero) 
			{ 
				m_useAero = bUseAero; 
			}

			bool GetAeroEnabled()
			{
				if (m_useAero.IsUndefined())
				{
					bool bIsAero;
					OSVersion osversion;

					bIsAero = osversion.GetVersion() != WindowsXP;
					return bIsAero;
				}
				return m_useAero == true;
			}

	private:

			ConfigurationRemote m_remote;
			TriBool m_useAero;
};
