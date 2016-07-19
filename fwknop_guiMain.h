/***************************************************************
 * Name:      fwknop_guiMain.h
 * Purpose:   Defines Application Frame
 * Author:    Jonathan Bennett (JBennett@incomsystems.biz)
 * Created:   2015-08-05
 * Copyright: Jonathan Bennett (incomsystems.biz)
 * License:
 **************************************************************/

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
