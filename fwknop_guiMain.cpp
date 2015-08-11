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
    EVT_MENU(idMenuAbout, fwknop_guiFrame::OnAbout)
    EVT_CHOICE(ID_AllowIP, fwknop_guiFrame::OnChoice)
    EVT_CHOICE(ID_MessType, fwknop_guiFrame::OnChoice)
    EVT_BUTTON(ID_SaveButton, fwknop_guiFrame::OnSave)
    EVT_BUTTON(ID_KnockButton, fwknop_guiFrame::OnKnock)
    EVT_LISTBOX(ID_List, fwknop_guiFrame::OnLoad)
END_EVENT_TABLE()

fwknop_guiFrame::fwknop_guiFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, wxID_ANY, title, wxPoint(-1, -1), wxSize(800, 600))
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS


// So this will be where we set up the gui. All the interesting things will happen in event handlers


//wxPanel *configPanel = new wxPanel(this, wxID_ANY);


hbox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *vListBox = new wxBoxSizer(wxVERTICAL);
vConfigBox = new wxBoxSizer(wxVERTICAL);

//The following are the sizers for each line of config:
wxBoxSizer *hNickBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hServAddrBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hLegacyBox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *hServPortBox = new wxBoxSizer(wxHORIZONTAL);
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
hInternalIPBox = new wxBoxSizer(wxHORIZONTAL);
hInternalPortBox = new wxBoxSizer(wxHORIZONTAL);
hServCmdBox = new wxBoxSizer(wxHORIZONTAL);
initMessTypeEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_MessType);
initAllowIPEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_AllowIP);
configFile = new wxFileConfig (wxT("fwknop-gui"));
ourConfigList = new wxArrayString;
ourConfig = new Config;


wxButton *save = new wxButton(this, ID_SaveButton, wxT("Save Config"));


wxStaticText *NickLbl = new wxStaticText(this,wxID_ANY, wxT("Nickname: "));
NickTxt = new wxTextCtrl(this, wxID_ANY);

hNickBox->Add(NickLbl,0,wxALIGN_BOTTOM);
hNickBox->Add(NickTxt,1, wxEXPAND);


wxStaticText *ServAddrLbl = new wxStaticText(this,wxID_ANY, wxT("Server Address: "));
ServAddrTxt = new wxTextCtrl(this, wxID_ANY);

hServAddrBox->Add(ServAddrLbl,0,wxALIGN_BOTTOM);
hServAddrBox->Add(ServAddrTxt,1, wxEXPAND);


LegacyChk = new wxCheckBox(this, wxID_ANY,wxT("Use Legacy Mode"));

hLegacyBox->Add(LegacyChk);


wxStaticText *ServPortLbl = new wxStaticText(this,wxID_ANY, wxT("Server Port: "));
ServPortTxt = new wxTextCtrl(this, wxID_ANY,wxT("62201"));

hServPortBox->Add(ServPortLbl,0,wxALIGN_BOTTOM);
hServPortBox->Add(ServPortTxt,1, wxEXPAND);


wxStaticText *ProtoLbl = new wxStaticText(this,wxID_ANY, wxT("Protocol: "));

wxArrayString Protos;
Protos.Add(wxT("UDP"));
Protos.Add(wxT("TCP"));
Protos.Add(wxT("HTTP"));
ProtoChoice = new wxChoice(this,wxID_ANY, wxDefaultPosition, wxDefaultSize,
   Protos);

hProtoBox->Add(ProtoLbl,0,wxALIGN_BOTTOM);
hProtoBox->Add(ProtoChoice);
ProtoChoice->SetSelection(0);

wxStaticText *KeyLbl = new wxStaticText(this,wxID_ANY, wxT("Rijndael Key: "));
KeyTxt = new wxTextCtrl(this, wxID_ANY);

hKeyBox->Add(KeyLbl,0,wxALIGN_BOTTOM);
hKeyBox->Add(KeyTxt,1, wxEXPAND);


KeyB64Chk = new wxCheckBox(this, wxID_ANY,wxT("Key Is Base 64"));

hKeyB64Box->Add(KeyB64Chk);


wxStaticText *HmacKeyLbl = new wxStaticText(this,wxID_ANY, wxT("HMAC Key: "));
HmacKeyTxt = new wxTextCtrl(this, wxID_ANY);

hHmacKeyBox->Add(HmacKeyLbl,0,wxALIGN_BOTTOM);
hHmacKeyBox->Add(HmacKeyTxt,1, wxEXPAND);


HmacKeyB64Chk = new wxCheckBox(this, wxID_ANY,wxT("HMAC Is Base 64"));

hHmacB64Box->Add(HmacKeyB64Chk);


wxStaticText *AllowIPLbl = new wxStaticText(this,wxID_ANY, wxT("Allow IP: "));

wxArrayString AllowIP;
AllowIP.Add(wxT("Resolve IP"));
AllowIP.Add(wxT("Source IP"));
AllowIP.Add(wxT("Allow IP"));

AllowIPChoice = new wxChoice(this, ID_AllowIP, wxDefaultPosition, wxDefaultSize,
   AllowIP);


hAllowIPBox->Add(AllowIPLbl,0,wxALIGN_BOTTOM);
hAllowIPBox->Add(AllowIPChoice);
AllowIPChoice->SetSelection(0);

wxStaticText *IPToAllowLbl = new wxStaticText(this,wxID_ANY, wxT("IP To Allow: "));
IPToAllowTxt = new wxTextCtrl(this, wxID_ANY);

hIPToAllowBox->Add(IPToAllowLbl,0,wxALIGN_BOTTOM);
hIPToAllowBox->Add(IPToAllowTxt,1, wxEXPAND);


wxStaticText *MessTypeLbl = new wxStaticText(this,wxID_ANY, wxT("Message Type: "));

wxArrayString MessType;
MessType.Add(wxT("Open Port"));
MessType.Add(wxT("Nat Access"));
MessType.Add(wxT("Server Command"));
MessTypeChoice = new wxChoice(this,ID_MessType, wxDefaultPosition, wxDefaultSize,
   MessType);

hMessTypeBox->Add(MessTypeLbl,0,wxALIGN_BOTTOM);
hMessTypeBox->Add(MessTypeChoice);
MessTypeChoice->SetSelection(0);




wxStaticText *AccessPortsLbl = new wxStaticText(this,wxID_ANY, wxT("Access Ports: "));
AccessPortsTxt = new wxTextCtrl(this, wxID_ANY, wxT("tcp/22"));

hAccessPortsBox->Add(AccessPortsLbl,0,wxALIGN_BOTTOM);
hAccessPortsBox->Add(AccessPortsTxt,1, wxEXPAND);


wxStaticText *FwTimeLbl = new wxStaticText(this,wxID_ANY, wxT("Firewall Timeout: "));
FwTimeTxt = new wxTextCtrl(this, wxID_ANY, wxT("60"));

hFwTimeBox->Add(FwTimeLbl,0,wxALIGN_BOTTOM);
hFwTimeBox->Add(FwTimeTxt,1, wxEXPAND);


wxStaticText *InternalIPLbl = new wxStaticText(this,wxID_ANY, wxT("Internal IP: "));
InternalIPTxt = new wxTextCtrl(this, wxID_ANY);

hInternalIPBox->Add(InternalIPLbl,0,wxALIGN_BOTTOM);
hInternalIPBox->Add(InternalIPTxt,1, wxEXPAND);


wxStaticText *InternalPortLbl = new wxStaticText(this,wxID_ANY, wxT("Internal Port: "));
InternalPortTxt = new wxTextCtrl(this, wxID_ANY);

hInternalPortBox->Add(InternalPortLbl,0,wxALIGN_BOTTOM);
hInternalPortBox->Add(InternalPortTxt,1, wxEXPAND);


wxStaticText *ServCmdLbl = new wxStaticText(this,wxID_ANY, wxT("Server Command: "));
ServCmdTxt = new wxTextCtrl(this, wxID_ANY);

hServCmdBox->Add(ServCmdLbl,0,wxALIGN_BOTTOM);
hServCmdBox->Add(ServCmdTxt,1, wxEXPAND);







listbox = new wxListBox(this, ID_List, wxPoint(-1, -1), wxSize(200, -1));
ourConfig->getAllConfigs(ourConfigList, configFile);
listbox->InsertItems(*ourConfigList,0);

wxButton *ok = new wxButton(this, ID_KnockButton, wxT("Send Knock"));



vListBox->Add(listbox,1,wxBOTTOM, 50);
vListBox->Add(ok);
hbox->Add(vListBox, 0, wxEXPAND);



vConfigBox->Add(hNickBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServAddrBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hLegacyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServPortBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hProtoBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hKeyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hKeyB64Box,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hHmacKeyBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hHmacB64Box,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hAllowIPBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hIPToAllowBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hMessTypeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hAccessPortsBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hFwTimeBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hInternalIPBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hInternalPortBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(hServCmdBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
vConfigBox->Add(save,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);

OnChoice(*initMessTypeEvent);
OnChoice(*initAllowIPEvent);


hbox->Add(vConfigBox, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);
this->SetSizer(hbox);

//Centre();



}

void fwknop_guiFrame::OnSave(wxCommandEvent &event)
{
//All sorts of input validation here, port the android code
//TODO: Move validation into it's own function. Here we just shuffle the data into the class. This would greatly clean up the android code, too.

//also, on android, need to detect changes to the config, because send knock with edits is broken. Similar idea, perhaps.

//also need to be able to rename and have a dedicated copy button. Perhaps name is immutable when editing.
    ourConfig->NICK_NAME = NickTxt->GetLineText(0);
    ourConfig->SERVER_IP = ServAddrTxt->GetLineText(0);
    ourConfig->LEGACY = LegacyChk->GetValue();
    ourConfig->SERVER_PORT = ServPortTxt->GetLineText(0);
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
    } else {
        ourConfig->ACCESS_IP = IPToAllowTxt->GetLineText(0);
    }
    ourConfig->PORTS = AccessPortsTxt->GetLineText(0);
    ourConfig->SERVER_TIMEOUT = FwTimeTxt->GetLineText(0);
    ourConfig->NAT_IP = InternalIPTxt->GetLineText(0);
    ourConfig->NAT_PORT = InternalPortTxt->GetLineText(0);
    ourConfig->SERVER_CMD = ServCmdTxt->GetLineText(0);

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
            vConfigBox->Hide(hInternalIPBox);
            vConfigBox->Hide(hInternalPortBox);
            vConfigBox->Hide(hServCmdBox);
        break;
        case 1:
            vConfigBox->Show(hAccessPortsBox);
            vConfigBox->Show(hFwTimeBox);
            vConfigBox->Show(hInternalIPBox);
            vConfigBox->Show(hInternalPortBox);
            vConfigBox->Hide(hServCmdBox);
        break;
        case 2:
            vConfigBox->Show(hServCmdBox);
            vConfigBox->Hide(hAccessPortsBox);
            vConfigBox->Hide(hFwTimeBox);
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
    NickTxt->ChangeValue(ourConfig->NICK_NAME);
    ServAddrTxt->ChangeValue(ourConfig->SERVER_IP);
    LegacyChk->SetValue(ourConfig->LEGACY);
    ServPortTxt->SetValue(ourConfig->SERVER_PORT);
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
    else {
        AllowIPChoice->SetSelection(2);
        IPToAllowTxt->SetValue(ourConfig->ACCESS_IP);
    }

    if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Open Port")) == 0)
        MessTypeChoice->SetSelection(0);
    else if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Nat Access")) == 0)
        MessTypeChoice->SetSelection(1);
    else if (ourConfig->MESS_TYPE.CmpNoCase(wxT("Server Command")) == 0)
        MessTypeChoice->SetSelection(2);

    AccessPortsTxt->SetValue(ourConfig->PORTS);
    FwTimeTxt->SetValue(ourConfig->SERVER_TIMEOUT);
    InternalIPTxt->SetValue(ourConfig->NAT_IP);
    InternalPortTxt->SetValue(ourConfig->NAT_PORT);
    ServCmdTxt->SetValue(ourConfig->SERVER_CMD);
    OnChoice(*initMessTypeEvent);
    OnChoice(*initAllowIPEvent);
}

void fwknop_guiFrame::OnKnock(wxCommandEvent &event)
{
    fko_ctx_t ctx;
    fwknop_options_t opts;
    int key_len, hmac_key_len;
    int res, hmac_str_len = 0;
    ourConfig->loadConfig(listbox->GetString(listbox->GetSelection()), configFile);
    short message_type = FKO_CLIENT_TIMEOUT_NAT_ACCESS_MSG;
    char key_str[129] = {0}, hmac_str[129] = {0};
    char spa_msg[256] = {0};
    char nat_access_str[25] = {0};

    memset(&opts, 0, sizeof(fwknop_options_t));

    if (ourConfig->KEY.CmpNoCase(wxEmptyString) == 0)
    ourConfig->KEY = wxGetTextFromUser(_("Please enter your Rijndael key"));

    if (ourConfig->KEY.CmpNoCase(wxEmptyString) == 0)
        return;
    if (ourConfig->SERVER_PORT.CmpNoCase(wxT("random")) == 0)
    {
        srand((int)wxGetLocalTime());
        ourConfig->SERVER_PORT = wxEmptyString;
        ourConfig->SERVER_PORT << (rand()%55535 + 10000); // do this better
    }
    if (ourConfig->ACCESS_IP.CmpNoCase(wxT("Source IP")) == 0)
        ourConfig->ACCESS_IP = wxT("0.0.0.0");
    else if (ourConfig->ACCESS_IP.CmpNoCase(wxT("Resolve IP")) == 0)
    {
        wxHTTP get;
        get.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
        get.SetTimeout(10); // 10 seconds of timeout instead of 10 minutes ...
        get.Connect(wxT("whatismyip.akamai.com"));
        wxInputStream *httpStream = get.GetInputStream(wxT("/"));
        if (get.GetError() == wxPROTO_NOERR)
        {
            wxString res;
            wxStringOutputStream out_stream(&res);
            httpStream->Read(out_stream);
            ourConfig->ACCESS_IP = res;
            wxDELETE(httpStream);
            get.Close();
        }
        else
        {
            wxMessageBox(_("Unable to resolve our IP!"));
            wxDELETE(httpStream);
            get.Close();
            return;
        }
    }

    if (ourConfig->KEY_BASE64)
    {
        key_len = fko_base64_decode(ourConfig->KEY.mb_str(), (unsigned char *)key_str);
    } else {
        strncpy(key_str, (const char*)ourConfig->KEY.mb_str(wxConvUTF8), 128);
        key_len = (int)strlen(key_str);
    }

    if (ourConfig->HMAC_BASE64)
    {
        hmac_str_len = fko_base64_decode(ourConfig->HMAC.mb_str(), (unsigned char *)hmac_str);
    } else {
        strncpy(hmac_str, (const char*)ourConfig->HMAC.mb_str(wxConvUTF8), 128);
        hmac_str_len = (int)strlen(hmac_str);
    }

    res = fko_new(&ctx);

    if (ourConfig->SERVER_CMD.CmpNoCase(wxEmptyString) != 0)
    {
    message_type = FKO_COMMAND_MSG;
    fko_set_spa_message_type(ctx, message_type);
    res = fko_set_spa_message(ctx, ourConfig->SERVER_CMD.mb_str());

    } else {


    res = fko_set_spa_client_timeout(ctx, wxAtoi(ourConfig->SERVER_TIMEOUT));

    snprintf(spa_msg, 256, "%s,%s", (const char*)ourConfig->ACCESS_IP.mb_str(wxConvUTF8), (const char*)ourConfig->PORTS.mb_str(wxConvUTF8));
    res = fko_set_spa_message(ctx, spa_msg);

    }

    //Todo: Nat Access and Server Cmd
    if (ourConfig->LEGACY)
        fko_set_spa_encryption_mode(ctx, FKO_ENC_MODE_CBC_LEGACY_IV);

    if (ourConfig->HMAC.CmpNoCase(wxEmptyString) != 0)
        fko_set_spa_hmac_type(ctx, FKO_DEFAULT_HMAC_MODE);

    if (ourConfig->NAT_IP.CmpNoCase(wxEmptyString) != 0)
    {
        sprintf(nat_access_str, "%s,%s", (const char*)ourConfig->NAT_IP.mb_str(wxConvUTF8), (const char*)ourConfig->NAT_PORT.mb_str(wxConvUTF8));
        res = fko_set_spa_nat_access(ctx, nat_access_str);

    }

    fko_spa_data_final(ctx, key_str, key_len, hmac_str, hmac_str_len);
     res = fko_get_spa_data(ctx, &opts.spa_data);
     ourConfig->SPA_STRING = wxString::FromUTF8(opts.spa_data);


     //if udp, and then implement tcp and http
    if (ourConfig->PROTOCOL.CmpNoCase(wxT("UDP")) == 0)
    {
        wxIPV4address serverAddr;
        wxIPV4address ourAddr;
        ourAddr.AnyAddress();
        ourAddr.Service(0);
        serverAddr.Hostname(ourConfig->SERVER_IP);
        serverAddr.Service(ourConfig->SERVER_PORT);
        wxDatagramSocket *m_socket;
        m_socket = new wxDatagramSocket(ourAddr, wxSOCKET_NOWAIT);
        m_socket->SendTo(serverAddr, ourConfig->SPA_STRING.mb_str(), ourConfig->SPA_STRING.Len());
        m_socket->WaitForWrite();
        m_socket->Destroy();
    } else
        wxMessageBox(_("Not implemented yet"));

}


fwknop_guiFrame::~fwknop_guiFrame()
{

}

void fwknop_guiFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void fwknop_guiFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void fwknop_guiFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
