/*
 * Copyright (C) 2013 Jordi Mas i Hernàndez <jmas@softcatala.org>
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


#include "SpellCheckerAction.h"
#include "OSVersion.h"

using namespace std;

SpellCheckerAction::SpellCheckerAction() : Action()
{
    m_authorizationRef = NULL;
}

SpellCheckerAction::~SpellCheckerAction()
{
    if (m_authorizationRef)
    {
        AuthorizationFree(m_authorizationRef, kAuthorizationFlagDestroyRights);
        m_authorizationRef = NULL;
    }
}

bool SpellCheckerAction::requestPermissions()
{
    OSStatus status;
 
    status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &m_authorizationRef);
    
    if (status != errAuthorizationSuccess)
    {
        NSLog(@"SpellCheckerAction::requestPermissions. AuthorizationCreate failed: %d", status);
        return false;
    }
    
    AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights rights = {1, &right};
    AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed |
    kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;
    
    status = AuthorizationCopyRights(m_authorizationRef, &rights, NULL, flags, NULL);
    if (status != errAuthorizationSuccess)
    {
        NSLog(@"SpellCheckerAction::requestPermissions. AuthorizationCopyRights failed: %d", status);
        return false;
    }
    
    return true;
}

NSString* SpellCheckerAction::_getBundlePath(CFStringRef file, CFStringRef extension)
{
    CFURLRef url;
    CFBundleRef mainBundle;
    
    mainBundle = CFBundleGetMainBundle();
    url = CFBundleCopyResourceURL(mainBundle, file, extension, NULL);
    CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
    return (NSString *)path;
}

bool SpellCheckerAction::_copyfile(NSString* src, NSString* trg)
{
    NSString * trkitMoveUtilityPath = @"/bin/cp";
    OSStatus status;
    
    char * args[3] = {
        [0] = (char *)[[src stringByStandardizingPath] UTF8String],
        [1] = (char *)[[trg stringByStandardizingPath] UTF8String],
        [2] = NULL
    };
    
    status = AuthorizationExecuteWithPrivileges(m_authorizationRef,
                                                [[trkitMoveUtilityPath stringByStandardizingPath] UTF8String],
                                                0, args, NULL);
    return status == errAuthorizationSuccess;
}


const char* SpellCheckerAction::GetVersion()
{
    if (m_version.empty())
    {
        char szVersion[1024];
        OSVersion version;
        
        sprintf(szVersion, "%u.%u.%u", version.GetMajorVersion(), version.GetMinorVersion(), version.GetBugFix());
        m_version = szVersion;
    }
    
    return m_version.c_str();
}


void SpellCheckerAction::Execute()
{
    NSString* srcFile;
    bool isOk;
    
    requestPermissions();
    
    srcFile = _getBundlePath(CFSTR("Català"), CFSTR("aff"));
    _copyfile(srcFile, @"/Library/Spelling/Català.aff");

    srcFile = _getBundlePath(CFSTR("Català"), CFSTR("dic"));
    _copyfile(srcFile, @"/Library/Spelling/Català.dic");
    
    isOk = _isDictionaryInstalled();
    SetStatus(isOk ? Successful : FinishedWithError);
    
    NSLog(@"SpellCheckerAction::Execute. Result %u", isOk);
}

bool SpellCheckerAction::_isDictionaryInstalled()
{
    bool installed;
    NSString* DICTIONARY_FILE_MANUAL(@"/Library/Spelling/catala.aff");
    NSString* DICTIONARY_FILE(@"/Library/Spelling/Català.aff");
    
    installed = [[NSFileManager defaultManager] fileExistsAtPath:DICTIONARY_FILE];
    
    if (installed == false)
    {
        installed = [[NSFileManager defaultManager] fileExistsAtPath:DICTIONARY_FILE_MANUAL];
    }
    
    NSLog(@"SpellCheckerAction::_isDictionaryInstalled %u", installed);
    return installed;
}

bool SpellCheckerAction::IsNeed()
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
    
    NSLog(@"SpellCheckerAction::IsNeed. %u", bNeed);
	return bNeed;
}

void SpellCheckerAction::CheckPrerequirements(Action * action)
{
     OSVersion version;
    
	if (version.GetMajorVersion() > 10 ||
        (version.GetMajorVersion() == 10 && version.GetMinorVersion() >= 7))
    {
        if (_isDictionaryInstalled())
        {
            SetStatus(AlreadyApplied);
        }
    }
    else
    {
        m_cannotBeApplied = "No es pot realitzar aquesta acció en aquesta versió del sistema.";
        SetStatus(CannotBeApplied);
    }
}
