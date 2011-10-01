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

#include "stdafx.h"
#include "IEAcceptLanguagesAction.h"

// See: HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\International

wchar_t* IEAcceptedLanguagesAction::GetName()
{
	return GetStringFromResourceIDName (IDS_IEACCEPTEDLANGUAGESACTION_NAME, szName);	
}

wchar_t* IEAcceptedLanguagesAction::GetDescription()
{
	return NULL;
}

void IEAcceptedLanguagesAction::Execute()
{
}

void IEAcceptedLanguagesAction::Result()
{

}
