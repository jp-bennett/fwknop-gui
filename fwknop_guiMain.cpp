/***************************************************************
 * Name:      fwknop_guiMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Jonathan Bennett (JBennett@incomsystems.biz)
 * Created:   2015-08-05
 * Copyright: Jonathan Bennett (incomsystems.biz)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "fwknop_guiMain.h"
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30);

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(fwknop_guiFrame, wxFrame)
    EVT_CLOSE(fwknop_guiFrame::OnClose)
    EVT_MENU(idMenuQuit, fwknop_guiFrame::OnQuit)
    EVT_MENU(idMenuNew, fwknop_guiFrame::OnNew)
    EVT_MENU(idMenuLocation, fwknop_guiFrame::OnLocation)
    EVT_MENU(idMenuDelete, fwknop_guiFrame::OnDelete)
    EVT_MENU(idMenuAbout, fwknop_guiFrame::OnAbout)
    EVT_MENU(idMenuHelpScreen, fwknop_guiFrame::OnHelpScreen)
    EVT_MENU(idMenuSettings, fwknop_guiFrame::OnSettings)
    EVT_MENU(idMenuWizard, fwknop_guiFrame::OnWizard)
    EVT_MENU(idMenuImport, fwknop_guiFrame::OnImport)
    EVT_MENU(idMenuExport, fwknop_guiFrame::OnExport)
    EVT_MENU(idMenuQR, fwknop_guiFrame::OnQR)
    EVT_CHECKBOX(ID_Random, fwknop_guiFrame::OnChoice)
    EVT_CHOICE(ID_AllowIP, fwknop_guiFrame::OnChoice)
    EVT_CHOICE(ID_MessType, fwknop_guiFrame::OnChoice)
    EVT_BUTTON(ID_SaveButton, fwknop_guiFrame::OnSave)
    EVT_BUTTON(ID_KnockButton, fwknop_guiFrame::OnKnock)
    EVT_LISTBOX(ID_List, fwknop_guiFrame::OnLoad)
    EVT_HTML_LINK_CLICKED(ID_html, fwknop_guiFrame::OnLink)
END_EVENT_TABLE()

fwknop_guiFrame::fwknop_guiFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, wxID_ANY, title, wxPoint(-1, -1), wxSize(800, 600))
{
//#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuSettings, _("Settings"));
    fileMenu->Append(idMenuNew, _("&New Config"));
    fileMenu->Append(idMenuDelete, _("&Delete Config"));
    fileMenu->Append(idMenuLocation, _("&Choose Config File"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* toolsMenu = new wxMenu(_T(""));
    toolsMenu->Append(idMenuWizard, _("&Access.conf wizard"));
    toolsMenu->Append(idMenuImport, _("&Import from fwknoprc file"));
    toolsMenu->Append(idMenuExport, _("&Export as fwknoprc file"));
    toolsMenu->Append(idMenuQR, _("&Export as QR code"));
    mbar->Append(toolsMenu, _("&Tools"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    helpMenu->Append(idMenuHelpScreen, _("&Help Screen"), _("Show help screen"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
//#endif // wxUSE_MENUS


// So this will be where we set up the gui. All the interesting things will happen in event handlers


curl_global_init(CURL_GLOBAL_DEFAULT);
wxColour *BackGround = new wxColour(233,233,233);
this->SetBackgroundColour(*BackGround);

hbox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *vListBox = new wxBoxSizer(wxVERTICAL);
vConfigBox = new wxBoxSizer(wxVERTICAL);
wxScrolledWindow *vConfigScroll = new wxScrolledWindow(this);


//The following are the sizers for each line of config:
wxBoxSizer *hNickBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hServAddrBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hLegacyBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hRandomBox = new wxBoxSizer(wxHORIZONTAL);
hServPortBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hProtoBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hKeyBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hKeyB64Box = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hHmacKeyBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hHmacB64Box = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hAllowIPBox = new wxBoxSizer(wxHORIZONTAL);
hIPToAllowBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hMessTypeBox = new wxBoxSizer(wxHORIZONTAL);
hAccessPortsBox = new wxBoxSizer(wxHORIZONTAL);
hFwTimeBox = new wxBoxSizer(wxHORIZONTAL);
hKeepAliveBox = new wxBoxSizer(wxHORIZONTAL);
hInternalIPBox = new wxBoxSizer(wxHORIZONTAL);
hInternalPortBox = new wxBoxSizer(wxHORIZONTAL);
hServCmdBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hDigestTypeBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hHmacTypeBox = new wxBoxSizer(wxHORIZONTAL);

initMessTypeEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_MessType);
initAllowIPEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_AllowIP);
initCheckboxEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_Random);
configFile = new wxFileConfig (wxT("fwknop-gui"));

ourConfigList = new wxArrayString;
ourConfig = new Config;


wxButton *save = new wxButton(vConfigScroll, ID_SaveButton, wxT("Save Config"));


wxStaticText *NickLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Nickname: "));
NickTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hNickBox->Add(NickLbl,0,wxALIGN_BOTTOM);
hNickBox->Add(NickTxt,1, wxEXPAND);


wxStaticText *ServAddrLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Server Address: "));
ServAddrTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hServAddrBox->Add(ServAddrLbl,0,wxALIGN_BOTTOM);
hServAddrBox->Add(ServAddrTxt,1, wxEXPAND);


LegacyChk = new wxCheckBox(vConfigScroll, wxID_ANY,wxT("Use Legacy Mode"));
hLegacyBox->Add(LegacyChk);

RandomChk = new wxCheckBox(vConfigScroll, ID_Random,wxT("Use Random Port"));
hRandomBox->Add(RandomChk);

wxStaticText *ServPortLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Server Port: "));
ServPortTxt = new wxTextCtrl(vConfigScroll, wxID_ANY,wxT("62201"));

hServPortBox->Add(ServPortLbl,0,wxALIGN_BOTTOM);
hServPortBox->Add(ServPortTxt,1, wxEXPAND);


wxStaticText *ProtoLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Protocol: "));

wxArrayString Protos;
Protos.Add(wxT("UDP"));
Protos.Add(wxT("TCP"));
Protos.Add(wxT("HTTP"));
ProtoChoice = new wxChoice(vConfigScroll,wxID_ANY, wxDefaultPosition, wxDefaultSize,
   Protos);

hProtoBox->Add(ProtoLbl,0,wxALIGN_BOTTOM);
hProtoBox->Add(ProtoChoice);
ProtoChoice->SetSelection(0);

wxStaticText *KeyLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Rijndael Key: "));
KeyTxt = new wxTextCtrl(vConfigScroll, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
//KeyTxt->SetWindowStyleFlag(wxTE_PASSWORD);

hKeyBox->Add(KeyLbl,0,wxALIGN_BOTTOM);
hKeyBox->Add(KeyTxt,1, wxEXPAND);


KeyB64Chk = new wxCheckBox(vConfigScroll, wxID_ANY,wxT("Key Is Base 64"));

hKeyB64Box->Add(KeyB64Chk);

wxStaticText *DigestTypeLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("SPA Digest Type: "));
wxArrayString DigestType;
DigestType.Add(wxT("MD5"));
DigestType.Add(wxT("SHA1"));
DigestType.Add(wxT("SHA256"));
DigestType.Add(wxT("SHA384"));
DigestType.Add(wxT("SHA512"));
DigestTypeChoice = new wxChoice(vConfigScroll, ID_DigestType, wxDefaultPosition, wxDefaultSize,
   DigestType);

hDigestTypeBox->Add(DigestTypeLbl,0,wxALIGN_BOTTOM);
hDigestTypeBox->Add(DigestTypeChoice);
DigestTypeChoice->SetSelection(2);



wxStaticText *HmacKeyLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("HMAC Key: "));
HmacKeyTxt = new wxTextCtrl(vConfigScroll, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
//HmacKeyTxt->SetWindowStyleFlag(wxTE_PASSWORD);


hHmacKeyBox->Add(HmacKeyLbl,0,wxALIGN_BOTTOM);
hHmacKeyBox->Add(HmacKeyTxt,1, wxEXPAND);


HmacKeyB64Chk = new wxCheckBox(vConfigScroll, wxID_ANY,wxT("HMAC Is Base 64"));

hHmacB64Box->Add(HmacKeyB64Chk);




wxStaticText *HmacTypeLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("SPA HMAC Type: "));
wxArrayString HmacType;
HmacType.Add(wxT("MD5"));
HmacType.Add(wxT("SHA1"));
HmacType.Add(wxT("SHA256"));
HmacType.Add(wxT("SHA384"));
HmacType.Add(wxT("SHA512"));
HmacTypeChoice = new wxChoice(vConfigScroll, ID_HmacType, wxDefaultPosition, wxDefaultSize,
   HmacType);

hHmacTypeBox->Add(HmacTypeLbl,0,wxALIGN_BOTTOM);
hHmacTypeBox->Add(HmacTypeChoice);
HmacTypeChoice->SetSelection(2);

wxStaticText *AllowIPLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Allow IP: "));

wxArrayString AllowIP;
AllowIP.Add(wxT("Resolve IP"));
AllowIP.Add(wxT("Source IP"));
AllowIP.Add(wxT("Allow IP"));
AllowIP.Add(wxT("Prompt IP"));

AllowIPChoice = new wxChoice(vConfigScroll, ID_AllowIP, wxDefaultPosition, wxDefaultSize,
   AllowIP);


hAllowIPBox->Add(AllowIPLbl,0,wxALIGN_BOTTOM);
hAllowIPBox->Add(AllowIPChoice);
AllowIPChoice->SetSelection(0);

wxStaticText *IPToAllowLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("IP To Allow: "));
IPToAllowTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hIPToAllowBox->Add(IPToAllowLbl,0,wxALIGN_BOTTOM);
hIPToAllowBox->Add(IPToAllowTxt,1, wxEXPAND);


wxStaticText *MessTypeLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Message Type: "));

wxArrayString MessType;
MessType.Add(wxT("Open Port"));
MessType.Add(wxT("Nat Access"));
MessType.Add(wxT("Local Nat Access"));
MessType.Add(wxT("Server Command"));
MessTypeChoice = new wxChoice(vConfigScroll,ID_MessType, wxDefaultPosition, wxDefaultSize,
   MessType);

hMessTypeBox->Add(MessTypeLbl,0,wxALIGN_BOTTOM);
hMessTypeBox->Add(MessTypeChoice);
MessTypeChoice->SetSelection(0);


wxStaticText *AccessPortsLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Access Ports: "));
AccessPortsTxt = new wxTextCtrl(vConfigScroll, wxID_ANY, wxT("tcp/22"));

hAccessPortsBox->Add(AccessPortsLbl,0,wxALIGN_BOTTOM);
hAccessPortsBox->Add(AccessPortsTxt,1, wxEXPAND);


wxStaticText *FwTimeLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Firewall Timeout: "));
FwTimeTxt = new wxTextCtrl(vConfigScroll, wxID_ANY, wxT("60"));

hFwTimeBox->Add(FwTimeLbl,0,wxALIGN_BOTTOM);
hFwTimeBox->Add(FwTimeTxt,1, wxEXPAND);

KeepOpenChk = new wxCheckBox(vConfigScroll, wxID_ANY,wxT("Keep port open by automatically resending SPA packets"));

hKeepAliveBox->Add(KeepOpenChk);


wxStaticText *InternalIPLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Internal IP: "));
InternalIPTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hInternalIPBox->Add(InternalIPLbl,0,wxALIGN_BOTTOM);
hInternalIPBox->Add(InternalIPTxt,1, wxEXPAND);


wxStaticText *InternalPortLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Internal Port: "));
InternalPortTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hInternalPortBox->Add(InternalPortLbl,0,wxALIGN_BOTTOM);
hInternalPortBox->Add(InternalPortTxt,1, wxEXPAND);


wxStaticText *ServCmdLbl = new wxStaticText(vConfigScroll,wxID_ANY, wxT("Server Command: "));
ServCmdTxt = new wxTextCtrl(vConfigScroll, wxID_ANY);

hServCmdBox->Add(ServCmdLbl,0,wxALIGN_BOTTOM);
hServCmdBox->Add(ServCmdTxt,1, wxEXPAND);



listbox = new wxListBox(this, ID_List, wxPoint(-1, -1), wxSize(200, -1));
ourConfig->getAllConfigs(ourConfigList, configFile);
if (!ourConfigList->IsEmpty())
    listbox->InsertItems(*ourConfigList,0);

wxButton *ok = new wxButton(this, ID_KnockButton, wxT("Send Knock"));



vListBox->Add(listbox,1,wxBOTTOM, 50);
vListBox->Add(ok);


hbox->Add(vListBox, 0, wxEXPAND);

vConfigBox->Add(hNickBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServAddrBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hLegacyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hRandomBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServPortBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hProtoBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hKeyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hKeyB64Box,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hDigestTypeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hHmacKeyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hHmacB64Box,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hHmacTypeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hAllowIPBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hIPToAllowBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hMessTypeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hAccessPortsBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hFwTimeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hKeepAliveBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hInternalIPBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hInternalPortBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServCmdBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(save,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);

OnChoice(*initMessTypeEvent);
OnChoice(*initAllowIPEvent);

vConfigScroll->SetSizer(vConfigBox);
vConfigScroll->FitInside(); // ask the sizer about the needed size
vConfigScroll->SetScrollRate(5, 5);

hbox->Add(vConfigScroll, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxEXPAND, 5);
this->SetSizer(hbox);

}

void fwknop_guiFrame::OnSave(wxCommandEvent &event)
{
//All sorts of input validation here, port the android code

//also, on android, need to detect changes to the config, because send knock with edits is broken. Similar idea, perhaps.

//also need to be able to rename and have a dedicated copy button. Perhaps name is immutable when editing.
    ourConfig->NICK_NAME = NickTxt->GetLineText(0);
    ourConfig->SERVER_IP = ServAddrTxt->GetLineText(0);
    ourConfig->LEGACY = LegacyChk->GetValue();
    if (RandomChk->GetValue()) {
        ourConfig->SERVER_PORT = wxT("random");
    } else {
        ourConfig->SERVER_PORT = ServPortTxt->GetLineText(0);
    }
    ourConfig->PROTOCOL = ProtoChoice->GetString(ProtoChoice->GetSelection());   //Change this for i18n
    ourConfig->KEY = KeyTxt->GetLineText(0);
    ourConfig->KEY_BASE64 = KeyB64Chk->GetValue();
    ourConfig->HMAC = HmacKeyTxt->GetLineText(0);
    ourConfig->HMAC_BASE64 = HmacKeyB64Chk->GetValue();
    ourConfig->MESS_TYPE = MessTypeChoice->GetString(MessTypeChoice->GetSelection()); //Change this for i18n
    if (AllowIPChoice->GetSelection() == 0) {
        ourConfig->ACCESS_IP = wxT("Resolve IP");
    } else if (AllowIPChoice->GetSelection() == 1) {
        ourConfig->ACCESS_IP = wxT("Source IP");
    } else if (AllowIPChoice->GetSelection() == 3) {
        ourConfig->ACCESS_IP = wxT("Prompt IP");
    } else {
        ourConfig->ACCESS_IP = IPToAllowTxt->GetLineText(0);
    }
    ourConfig->PORTS = AccessPortsTxt->GetLineText(0);
    ourConfig->SERVER_TIMEOUT = FwTimeTxt->GetLineText(0);
    ourConfig->KEEP_OPEN = KeepOpenChk->IsChecked();
    ourConfig->NAT_IP = InternalIPTxt->GetLineText(0);
    ourConfig->NAT_PORT = InternalPortTxt->GetLineText(0);
    ourConfig->SERVER_CMD = ServCmdTxt->GetLineText(0);
    ourConfig->DIGEST_TYPE = DigestTypeChoice->GetString(DigestTypeChoice->GetSelection()); //Change this for i18n
    ourConfig->HMAC_TYPE = HmacTypeChoice->GetString(HmacTypeChoice->GetSelection()); //Change this for i18n

    wxString result = ourConfig->validateConfig();
    if (result.CmpNoCase(wxT("valid")) == 0)
    {
        ourConfig->saveConfig(configFile);
        ourConfig->getAllConfigs(ourConfigList, configFile);
        listbox->Clear();
        listbox->InsertItems(*ourConfigList,0);
        wxMessageBox(_("Save Successful."));
    } else {
        wxMessageBox(result);

    }

}

void fwknop_guiFrame::OnChoice(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case ID_Random:
        if (RandomChk->GetValue()) {
            vConfigBox->Hide(hServPortBox);
        } else {
            vConfigBox->Show(hServPortBox);
        }

    break;

    case ID_AllowIP:
        if (AllowIPChoice->GetSelection() == 2)
        {
            vConfigBox->Show(hIPToAllowBox);
        } else {
            vConfigBox->Hide(hIPToAllowBox);
        }
    break;

    case ID_MessType:

        switch (MessTypeChoice->GetSelection())
        {
        case 0:
            vConfigBox->Show(hAccessPortsBox);
            vConfigBox->Show(hFwTimeBox);
            vConfigBox->Show(hKeepAliveBox);
            vConfigBox->Hide(hInternalIPBox);
            vConfigBox->Hide(hInternalPortBox);
            vConfigBox->Hide(hServCmdBox);
        break;
        case 1:
            vConfigBox->Show(hAccessPortsBox);
            vConfigBox->Show(hFwTimeBox);
            vConfigBox->Show(hKeepAliveBox);
            vConfigBox->Show(hInternalIPBox);
            vConfigBox->Show(hInternalPortBox);
            vConfigBox->Hide(hServCmdBox);
        break;
        case 2:
            vConfigBox->Show(hAccessPortsBox);
            vConfigBox->Show(hFwTimeBox);
            vConfigBox->Show(hKeepAliveBox);
            vConfigBox->Hide(hInternalIPBox);
            vConfigBox->Show(hInternalPortBox);
            vConfigBox->Hide(hServCmdBox);
        break;
        case 3:
            vConfigBox->Show(hServCmdBox);
            vConfigBox->Hide(hAccessPortsBox);
            vConfigBox->Hide(hFwTimeBox);
            vConfigBox->Hide(hKeepAliveBox);
            vConfigBox->Hide(hInternalIPBox);
            vConfigBox->Hide(hInternalPortBox);
        break;
    }
    break;
    }
hbox->Layout();
}

void fwknop_guiFrame::OnLoad(wxCommandEvent &event)
{
    ourConfig->loadConfig(listbox->GetString(listbox->GetSelection()), configFile);
    this->populate();
}

void fwknop_guiFrame::OnKnock(wxCommandEvent &event)
{
    wxIPV4address serverAddr;
    wxString serverHost;

    if (listbox->GetSelection() == wxNOT_FOUND) // want to return before even using the memory if nothing is selected
    {
        wxMessageBox(_("No config selected!"));
        return;
    }

//    CURL *curl;
//    CURLcode curl_Res; //Use these when we switch to curl for sending http knocks
    wxString SPA_Result;

    ourConfig->loadConfig(listbox->GetString(listbox->GetSelection()), configFile);
    if (ourConfig->KEY.CmpNoCase(wxEmptyString) == 0)
        ourConfig->KEY = wxGetPasswordFromUser(_("Please enter your Rijndael key"));

    if (ourConfig->KEY.CmpNoCase(wxEmptyString) == 0)
        return;

    if (ourConfig->ACCESS_IP.CmpNoCase(_("Prompt IP")) == 0)
        ourConfig->ACCESS_IP = wxGetTextFromUser(_("Please enter your Access IP"));

    if (ourConfig->ACCESS_IP.CmpNoCase(wxEmptyString) == 0)
        return;

    if (!serverAddr.Hostname(ourConfig->SERVER_IP)) {
        wxMessageBox(_("Could not resolve server!"));
        return;
    }
    serverHost = ourConfig->SERVER_IP; //HTTP and eventually HTTPS should be sent to the hostname rather than the IP
    ourConfig->SERVER_IP = serverAddr.IPAddress();

    configFile->SetPath(wxT("/"));
    SPA_Result = ourConfig->gen_SPA(configFile->Read(wxT("ip_resolver_url"), _("https://api.ipify.org")));
    if (SPA_Result.CmpNoCase(wxT("Success")) != 0 ) {
        wxMessageBox(SPA_Result);
        return;
    }

    SPA_Result = ourConfig->send_SPA(&serverAddr);
    if (SPA_Result.CmpNoCase(wxT("Knock sent successfully.")) != 0 ) {
        wxMessageBox(SPA_Result);
        return;
    } else if ((configFile->Read(wxT("show_timer"), _("true")).CmpNoCase(_("true")) == 0) || (ourConfig->KEEP_OPEN)){
        timerDialog *ourTimer = new timerDialog(ourConfig->NICK_NAME, ourConfig, &serverAddr);
        ourTimer->Show();
        return;
    } else {
        wxMessageBox(SPA_Result);
        return;
    }
}


fwknop_guiFrame::~fwknop_guiFrame()
{

}

void fwknop_guiFrame::OnNew(wxCommandEvent &event)
{
    listbox->SetSelection(wxNOT_FOUND);
    ourConfig->defaultConfig();
    this->populate();

}

void fwknop_guiFrame::OnSettings(wxCommandEvent &event)
{
    gConfigDialog *gConfig = new gConfigDialog(configFile);
    gConfig->Show();
}

void fwknop_guiFrame::OnWizard(wxCommandEvent &event)
{
    wizardDialog *wizard = new wizardDialog(_("Access.conf wizard"));
    wizard->Show(true);
    if (wizard->GetReturnCode() == wxID_OK) {
        listbox->SetSelection(wxNOT_FOUND);
        ourConfig->defaultConfig();
        ourConfig->KEY = wizard->tmp_config->KEY;
        ourConfig->KEY_BASE64 = wizard->tmp_config->KEY_BASE64;
        ourConfig->HMAC = wizard->tmp_config->HMAC;
        ourConfig->HMAC_BASE64 = true;
        this->populate();
        wxMessageBox(_("Generated keys imported into a blank config"));
    }

}

void fwknop_guiFrame::OnImport(wxCommandEvent &event)
{
    bool is_changed = false;
    rc_import *import_dialog = new rc_import(_("RC Import"), ourConfig, &is_changed);
    import_dialog->Show(true);
    if (is_changed) { //could check for a dup, save if no dupe, message if there is
        listbox->SetSelection(wxNOT_FOUND);
        this->populate();
    }

}

void fwknop_guiFrame::OnExport(wxCommandEvent &event)
{
    if (ourConfig != NULL) {
        rc_export *export_dialog = new rc_export(_("RC Export"), ourConfig);
        export_dialog->Show(true);
    }
}

void fwknop_guiFrame::OnQR(wxCommandEvent &event)
{
    if (ourConfig != NULL) {
        qr_export *qr_export_dialog = new qr_export(_(""), ourConfig);
        qr_export_dialog->Show(true);
    }
}

void fwknop_guiFrame::OnLocation(wxCommandEvent &event)
{
    wxFileDialog getRC(this, _("Saved Config File to Use"), _(""), _(""), wxFileSelectorDefaultWildcardStr, wxFD_SAVE);
    if (getRC.ShowModal() == wxID_CANCEL) {
        return;
    }
    free(configFile);
    configFile = new wxFileConfig (wxT(""), wxT(""), getRC.GetPath());
    ourConfig->getAllConfigs(ourConfigList, configFile);
    listbox->Clear();
    if (!ourConfigList->IsEmpty())
        listbox->InsertItems(*ourConfigList,0);
    listbox->SetSelection(wxNOT_FOUND);
    ourConfig->defaultConfig();
    this->populate();


}

void fwknop_guiFrame::OnDelete(wxCommandEvent &event)
{
    if (listbox->GetSelection() == wxNOT_FOUND)
    {
        wxMessageBox(_("No config selected!"));
        return;
    }
    if (wxMessageBox(_("Are you sure you want to delete '") + listbox->GetString(listbox->GetSelection()) + _("'"),
        _("Delete saved knock"), wxYES_NO) == wxYES)
        {
            configFile->SetPath(wxT("/"));
            configFile->DeleteGroup(listbox->GetString(listbox->GetSelection()));
            configFile->Flush();
            listbox->Delete(listbox->GetSelection());
            listbox->SetSelection(wxNOT_FOUND);
            ourConfig->defaultConfig();
            this->populate();

        }
}

void fwknop_guiFrame::OnClose(wxCloseEvent &event)
{
    curl_global_cleanup();
    Destroy();
}

void fwknop_guiFrame::OnQuit(wxCommandEvent &event)
{
    curl_global_cleanup();
    Destroy();
}

void fwknop_guiFrame::OnAbout(wxCommandEvent &event)
{
    //wxString msg = _("Fwknop-gui was created by Jonathan Bennett \nBased on fwknop and libfko written by Michael Rash");
    //wxMessageBox(msg, _("Fwknop-gui"));
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName(_("Fwknop-gui"));
    aboutInfo.SetVersion(_("Version 1.0"));
    aboutInfo.SetDescription(_("Fwknop-gui is a cross platform graphical fwknop client."));
    aboutInfo.SetWebSite(_("https://github.com/oneru/fwknop-gui"));
    aboutInfo.AddDeveloper(_("Jonathan Bennett"));
    wxAboutBox(aboutInfo);
}

void fwknop_guiFrame::OnHelpScreen(wxCommandEvent &event)
{
    wxFrame *frame = new wxFrame(this, wxID_ANY, _("Fwknop-gui help"));
    wxHtmlWindow *html = new wxHtmlWindow(frame, ID_html);
    if (wxFileExists(_("help.html"))) {
        html->LoadPage(_("help.html"));
        frame->Show(true);
    } else if(wxFileExists(wxStandardPaths::Get().GetDataDir() + _("/help.html"))) {
        html->LoadPage(wxStandardPaths::Get().GetDataDir() + _("/help.html"));
        frame->Show(true);
    } else {
        wxMessageBox(_("Could not open help file at: ") + wxStandardPaths::Get().GetDataDir());
    }


}

void fwknop_guiFrame::OnLink(wxHtmlLinkEvent &event)
{
    wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());

}

void fwknop_guiFrame::populate()
{

    NickTxt->ChangeValue(ourConfig->NICK_NAME);
    ServAddrTxt->ChangeValue(ourConfig->SERVER_IP);
    LegacyChk->SetValue(ourConfig->LEGACY);
    if (ourConfig->SERVER_PORT.CmpNoCase(wxT("Random")) == 0) {
    RandomChk->SetValue(true);
    } else {
        RandomChk->SetValue(false);
        ServPortTxt->SetValue(ourConfig->SERVER_PORT);
    }
    if (ourConfig->PROTOCOL.CmpNoCase(wxT("UDP")) == 0)
        ProtoChoice->SetSelection(0);
    else if (ourConfig->PROTOCOL.CmpNoCase(wxT("TCP")) == 0)
        ProtoChoice->SetSelection(1);
    else if (ourConfig->PROTOCOL.CmpNoCase(wxT("HTTP")) == 0)
        ProtoChoice->SetSelection(2);

    KeyTxt->SetValue(ourConfig->KEY);
    KeyB64Chk->SetValue(ourConfig->KEY_BASE64);
    HmacKeyTxt->SetValue(ourConfig->HMAC);
    HmacKeyB64Chk->SetValue(ourConfig->HMAC_BASE64);

    if (ourConfig->ACCESS_IP.CmpNoCase(wxT("Resolve IP")) == 0)
        AllowIPChoice->SetSelection(0);
    else if (ourConfig->ACCESS_IP.CmpNoCase(wxT("Source IP")) == 0)
        AllowIPChoice->SetSelection(1);
    else if (ourConfig->ACCESS_IP.CmpNoCase(wxT("Prompt IP")) == 0)
        AllowIPChoice->SetSelection(3);
    else {
        AllowIPChoice->SetSelection(2);
        IPToAllowTxt->SetValue(ourConfig->ACCESS_IP);
    }

    if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Open Port")) == 0)
        MessTypeChoice->SetSelection(0);
    else if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Nat Access")) == 0)
        MessTypeChoice->SetSelection(1);
    else if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Local Nat Access")) == 0)
        MessTypeChoice->SetSelection(2);
    else if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Server Command")) == 0)
        MessTypeChoice->SetSelection(3);

    AccessPortsTxt->SetValue(ourConfig->PORTS);
    FwTimeTxt->SetValue(ourConfig->SERVER_TIMEOUT);
    KeepOpenChk->SetValue(ourConfig->KEEP_OPEN);
    InternalIPTxt->SetValue(ourConfig->NAT_IP);
    InternalPortTxt->SetValue(ourConfig->NAT_PORT);
    ServCmdTxt->SetValue(ourConfig->SERVER_CMD);

    if (ourConfig->DIGEST_TYPE.CmpNoCase(wxT("MD5")) == 0)
        DigestTypeChoice->SetSelection(0);
    else if (ourConfig->DIGEST_TYPE.CmpNoCase(wxT("SHA1")) == 0)
        DigestTypeChoice->SetSelection(1);
    else if (ourConfig->DIGEST_TYPE.CmpNoCase(wxT("SHA256")) == 0)
        DigestTypeChoice->SetSelection(2);
    else if (ourConfig->DIGEST_TYPE.CmpNoCase(wxT("SHA384")) == 0)
        DigestTypeChoice->SetSelection(3);
    else if (ourConfig->DIGEST_TYPE.CmpNoCase(wxT("SHA512")) == 0)
        DigestTypeChoice->SetSelection(4);

    if (ourConfig->HMAC_TYPE.CmpNoCase(wxT("MD5")) == 0)
        HmacTypeChoice->SetSelection(0);
    else if (ourConfig->HMAC_TYPE.CmpNoCase(wxT("SHA1")) == 0)
        HmacTypeChoice->SetSelection(1);
    else if (ourConfig->HMAC_TYPE.CmpNoCase(wxT("SHA256")) == 0)
        HmacTypeChoice->SetSelection(2);
    else if (ourConfig->HMAC_TYPE.CmpNoCase(wxT("SHA384")) == 0)
        HmacTypeChoice->SetSelection(3);
    else if (ourConfig->HMAC_TYPE.CmpNoCase(wxT("SHA512")) == 0)
        HmacTypeChoice->SetSelection(4);

    OnChoice(*initMessTypeEvent);
    OnChoice(*initAllowIPEvent);
    OnChoice(*initCheckboxEvent);

}


/*static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if(os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
} */

/**
 * timeout is in seconds
 **/
 /*
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if(curl)
	{
		if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
		#ifdef __WIN32__
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt"))
		#endif
		&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))

		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}
*/
