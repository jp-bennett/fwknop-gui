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

        typedef struct fwknop_options
        {
            char           *spa_server_str;
            unsigned int    spa_dst_port;
            char           *spa_data;

            //nat access options // am I using these?
            char nat_access_str[1024];
            int  nat_local;
            int  nat_port;
            int  nat_rand_port;
            char server_command[1024];
        } fwknop_options_t;


        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            ID_AllowIP,
            ID_MessType,
            ID_SaveButton,
            ID_List,
            ID_KnockButton
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnChoice(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnLoad(wxCommandEvent& event);
        void OnKnock(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // FWKNOP_GUIMAIN_H
