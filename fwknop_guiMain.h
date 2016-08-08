/* fwknop_guiMain.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header file for fwknop_guiMain.cpp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#ifndef FWKNOP_GUIMAIN_H
#define FWKNOP_GUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "fwknop_guiApp.h"
#include "wizard.h"
#include "general_config.h"
#include "rc_export.h"
#include "rc_import.h"
#include "qr_export.h"
#include "timer.h"
#include "gpgme_wrapper.h"
#include <wx/filefn.h>
#include <wx/stdpaths.h>
class fwknop_guiFrame: public wxFrame
{
    public:
        fwknop_guiFrame(wxFrame *frame, const wxString& title);

    private:
        wxBoxSizer *hbox;
        wxBoxSizer *vConfigBox;
        wxBoxSizer *vConfigBoxSuper;
        wxBoxSizer *hIPToAllowBox;
        wxBoxSizer *hServPortBox;
        wxChoice *AllowIPChoice;
        wxChoice *MessTypeChoice;
        wxBoxSizer *hAccessPortsBox;
        wxBoxSizer *hFwTimeBox;
        wxBoxSizer *hKeepAliveBox;
        wxBoxSizer *hInternalIPBox;
        wxBoxSizer *hInternalPortBox;
        wxBoxSizer *hServCmdBox;
        wxFileConfig *configFile;
        wxTextCtrl *NickTxt;
        Config *ourConfig;
        wxArrayString *ourConfigList;
        wxListBox *listbox;
        wxTextCtrl *ServAddrTxt;
        wxCheckBox *LegacyChk;
        wxCheckBox *RandomChk;
        wxTextCtrl *ServPortTxt;
        wxCheckBox *GPGChk;
        wxTextCtrl *KeyTxt;
        wxCheckBox *KeyB64Chk;
        wxChoice *ProtoChoice;
        wxTextCtrl *HmacKeyTxt;
        wxCheckBox *HmacKeyB64Chk;
        wxTextCtrl *IPToAllowTxt;
        wxTextCtrl *AccessPortsTxt;
        wxTextCtrl *FwTimeTxt;
        wxCheckBox *TimerChk;
        wxCheckBox *KeepOpenChk;
        wxTextCtrl *InternalIPTxt;
        wxTextCtrl *InternalPortTxt;
        wxTextCtrl *ServCmdTxt;
        wxChoice *DigestTypeChoice;
        wxChoice *HmacTypeChoice;
        wxCommandEvent *initMessTypeEvent;
        wxCommandEvent *initAllowIPEvent;
        wxCommandEvent *initCheckboxEvent;
        gpgme_wrapper *ourGPG;
        wxArrayString *GPGKeys;
        wxArrayString *GPGSigKeys;
        wxBoxSizer *hKeyBox;
        wxBoxSizer *hKeyB64Box;
        wxBoxSizer *hGPGChoiceBox;
        wxChoice *GPGEncryptKey;
        wxChoice *GPGSignatureKey;
        wxScrolledWindow *vConfigScroll;





        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idMenuHelpScreen,
            idMenuSettings,
            idMenuNew,
            idMenuDelete,
            idMenuLocation,
            idMenuWizard,
            idMenuImport,
            idMenuExport,
            idMenuQR,
            idMenugpgEngine,
            idMenugpgFolder,
            idMenugpgDefaults,
            idMenuGPGTools,
            ID_AllowIP,
            ID_MessType,
            ID_SaveButton,
            ID_List,
            ID_KnockButton,
            ID_Random,
            ID_DigestType,
            ID_HmacType,
            ID_USE_GPG,
            ID_html
        };

        void populate();
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnNew(wxCommandEvent& event);
        void OnDelete(wxCommandEvent& event);
        void OnLocation(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnHelpScreen(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);
        void OnWizard(wxCommandEvent& event);
        void OnImport(wxCommandEvent& event);
        void OnExport(wxCommandEvent& event);
        void OnQR(wxCommandEvent& event);
        void gpgEngine(wxCommandEvent& event);
        void gpgFolder(wxCommandEvent& event);
        void gpgDefaults(wxCommandEvent& event);
        void OnChoice(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnLoad(wxCommandEvent& event);
        void OnLink(wxHtmlLinkEvent& event);
        void OnKnock(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};



#endif // FWKNOP_GUIMAIN_H
