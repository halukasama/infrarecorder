/*
 * InfraRecorder - CD/DVD burning software
 * Copyright (C) 2006-2012 Christian Kindahl
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.hh"
#include <base/string_util.hh>
#include <wmsdk.h>
#include "config_general_page.hh"

CConfigGeneralPage::CConfigGeneralPage()
{
    m_pConfig = NULL;
}

CConfigGeneralPage::~CConfigGeneralPage()
{
}

bool CConfigGeneralPage::SetConfig(CEncoderConfig *pConfig)
{
    if (pConfig == NULL)
        return false;

    m_pConfig = pConfig;
    return true;
}

bool CConfigGeneralPage::OnApply()
{
    m_pConfig->m_iProfile = (int)m_ProfileList.GetItemData(m_ProfileList.GetCurSel());

    return true;
}

bool CConfigGeneralPage::FillProfileList()
{
    HRESULT hResult = S_OK;
    IWMProfileManager *pIWMProfileManager = NULL;
    IWMProfileManager2 *pIWMProfileManager2 = NULL;
    IWMProfile *pIWMProfile = NULL;

    TCHAR szProfileName[256];

    do
    {
        // Create profile manager.
        hResult = g_LibraryHelper.irc_WMCreateProfileManager(&pIWMProfileManager);
        if (FAILED(hResult))
            break;

        hResult = pIWMProfileManager->QueryInterface(IID_IWMProfileManager2,(void **)&pIWMProfileManager2);
        if (FAILED(hResult))
            break;

        // Set system profile version to 8.0.
        hResult = pIWMProfileManager2->SetSystemProfileVersion(WMT_VER_8_0);
        if (FAILED(hResult))
            break;

        unsigned long ulNumProfiles = 0;
        hResult = pIWMProfileManager->GetSystemProfileCount(&ulNumProfiles);
        if (FAILED(hResult))
            break;

        // Iterate all system profiles.
        for (unsigned int i = 0; i < ulNumProfiles; i++)
        {
            hResult = pIWMProfileManager->LoadSystemProfile(i,&pIWMProfile);
            if (FAILED(hResult))
                break;

            unsigned long ulProfileNameLen = 255;
            hResult = pIWMProfile->GetName(szProfileName,&ulProfileNameLen);
            if (FAILED(hResult))
                break;

            if (!lstrncmp(szProfileName,_T("Windows Media Audio"),19))
            {
                szProfileName[16] = 'W';
                szProfileName[17] = 'M';
                szProfileName[18] = 'A';

                // Add the profile to the list.
                m_ProfileList.AddString(szProfileName + 16);
                m_ProfileList.SetItemData(m_ProfileList.GetCount() - 1,i);
            }
    
            if (pIWMProfile != NULL)
            {
                pIWMProfile->Release();
                pIWMProfile = NULL;
            }
        }

        if (FAILED(hResult))
            break;
    }
    while (false);

    // Release all resources.
    if (pIWMProfile != NULL)
    {
        pIWMProfile->Release();
        pIWMProfile = NULL;
    }

    if (pIWMProfileManager != NULL)
    {
        pIWMProfileManager->Release();
        pIWMProfileManager = NULL;
    }

    if (pIWMProfileManager2 != NULL)
    {
        pIWMProfileManager2->Release();
        pIWMProfileManager2 = NULL;
    }

    if (FAILED(hResult))
        return false;

    // Select the last profile that we found.
    m_ProfileList.SetCurSel(m_ProfileList.GetCount() - 1);

    return true;
}

LRESULT CConfigGeneralPage::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
    // Initialize the profile list.
    m_ProfileList = GetDlgItem(IDC_PROFILELIST);
    FillProfileList();

    return TRUE;
}
