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
//#include "configs.h"

class fwknop_guiFrame: public wxFrame
{
    public:
        fwknop_guiFrame(wxFrame *frame, const wxString& title);
        ~fwknop_guiFrame();
    private:
        wxBoxSizer *hbox;
        wxBoxSizer *vConfigBox;
        wxBoxSizer *hIPToAllowBox;
        wxBoxSizer *hServPortBox;
        wxChoice *AllowIPChoice;
        wxChoice *MessTypeChoice;
        wxBoxSizer *hAccessPortsBox;
        wxBoxSizer *hFwTimeBox;
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
        wxTextCtrl *KeyTxt;
        wxCheckBox *KeyB64Chk;
        wxChoice *ProtoChoice;
        wxTextCtrl *HmacKeyTxt;
        wxCheckBox *HmacKeyB64Chk;
        wxTextCtrl *IPToAllowTxt;
        wxTextCtrl *AccessPortsTxt;
        wxTextCtrl *FwTimeTxt;
        wxTextCtrl *InternalIPTxt;
        wxTextCtrl *InternalPortTxt;
        wxTextCtrl *ServCmdTxt;
        wxCommandEvent *initMessTypeEvent;
        wxCommandEvent *initAllowIPEvent;
        wxCommandEvent *initCheckboxEvent;




        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idMenuHelpScreen,
            idMenuSettings,
            idMenuNew,
            idMenuDelete,
            ID_AllowIP,
            ID_MessType,
            ID_SaveButton,
            ID_List,
            ID_KnockButton,
            ID_Random,
            ID_html
        };
        void populate();
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnNew(wxCommandEvent& event);
        void OnDelete(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnHelpScreen(wxCommandEvent& event);
        void OnSettings(wxCommandEvent& event);
        void OnChoice(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnLoad(wxCommandEvent& event);
        void OnLink(wxHtmlLinkEvent& event);
        void OnKnock(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};



#endif // FWKNOP_GUIMAIN_H
