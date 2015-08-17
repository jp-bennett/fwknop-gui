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
    EVT_MENU(idMenuDelete, fwknop_guiFrame::OnDelete)
    EVT_MENU(idMenuAbout, fwknop_guiFrame::OnAbout)
    EVT_CHECKBOX(ID_Random, fwknop_guiFrame::OnChoice)
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
    fileMenu->Append(idMenuNew, _("&New Config"));
    fileMenu->Append(idMenuDelete, _("&Delete Config"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS


// So this will be where we set up the gui. All the interesting things will happen in event handlers


//wxPanel *configPanel = new wxPanel(this, wxID_ANY);
curl_global_init(CURL_GLOBAL_DEFAULT);
wxColour *BackGround = new wxColour(233,233,233);
this->SetBackgroundColour(*BackGround);

hbox = new wxBoxSizer(wxHORIZONTAL);
wxBoxSizer *vListBox = new wxBoxSizer(wxVERTICAL);
vConfigBox = new wxBoxSizer(wxVERTICAL);

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
hInternalIPBox = new wxBoxSizer(wxHORIZONTAL);
hInternalPortBox = new wxBoxSizer(wxHORIZONTAL);
hServCmdBox = new wxBoxSizer(wxHORIZONTAL);
initMessTypeEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_MessType);
initAllowIPEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_AllowIP);
initCheckboxEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_Random);
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

RandomChk = new wxCheckBox(this, ID_Random,wxT("Use Random Port"));
hRandomBox->Add(RandomChk);

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
vConfigBox->Add(hRandomBox,1,wxALIGN_LEFT | wxEXPAND | wxALL,2);
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
    this->populate();
}

void fwknop_guiFrame::OnKnock(wxCommandEvent &event)
{
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
        ourConfig->KEY = wxGetTextFromUser(_("Please enter your Rijndael key"));

    SPA_Result = ourConfig->gen_SPA();
    if (SPA_Result.Cmp(wxT("Success")) != 0 ) {
    wxMessageBox(SPA_Result);
    return;
    }

    if (ourConfig->PROTOCOL.CmpNoCase(wxT("UDP")) == 0) {
        wxIPV4address serverAddr;
        wxIPV4address ourAddr;
        ourAddr.AnyAddress();
        ourAddr.Service(0);
        if (serverAddr.Hostname(ourConfig->SERVER_IP))
        {
            if (serverAddr.Service(ourConfig->SERVER_PORT))
            {
                wxDatagramSocket *m_socket;
                m_socket = new wxDatagramSocket(ourAddr, wxSOCKET_NOWAIT);
                m_socket->SendTo(serverAddr, ourConfig->SPA_STRING.mb_str(), ourConfig->SPA_STRING.Len());
                m_socket->WaitForWrite();
                if (m_socket->Error()) {
                    wxMessageBox(_("Could not send knock: Error sending."));
                } else {
                    wxMessageBox(_("Knock sent successfully."));
                }
                m_socket->Destroy();

            } else
                wxMessageBox(_("Could not send knock: could not set server port."));

        } else
            wxMessageBox(_("Could not send knock: could not set server address."));

    } else if (ourConfig->PROTOCOL.CmpNoCase(wxT("TCP")) == 0) {
        wxIPV4address tcp_serverAddr;
        wxIPV4address ourAddr;
        ourAddr.AnyAddress();
        ourAddr.Service(0);
        if (tcp_serverAddr.Hostname(ourConfig->SERVER_IP))
        {
            if (tcp_serverAddr.Service(ourConfig->SERVER_PORT))
            {
                wxSocketClient *tcp_socket = new wxSocketClient;
                tcp_socket->Connect(tcp_serverAddr);
                tcp_socket->WaitForWrite();
                tcp_socket->Write(ourConfig->SPA_STRING.mb_str(), ourConfig->SPA_STRING.Len());
                tcp_socket->WaitForWrite();
                if (tcp_socket->Error()) {
                    wxMessageBox(_("Could not send knock: Error sending."));
                } else {
                    wxMessageBox(_("Knock sent successfully."));
                }
                tcp_socket->Destroy();

            } else
            wxMessageBox(_("Could not send knock: could not set server address."));

        } else
            wxMessageBox(_("Could not send knock: could not set server address."));

    } else if (ourConfig->PROTOCOL.CmpNoCase(wxT("HTTP")) == 0) {
        wxHTTP *http_serv = new wxHTTP;
        http_serv->Connect(ourConfig->SERVER_IP, wxAtoi(ourConfig->SERVER_PORT));
        wxInputStream *tmp_stream;
        tmp_stream = http_serv->GetInputStream(ourConfig->SPA_STRING);
        delete tmp_stream;
        http_serv->Destroy();

    } else
        wxMessageBox(_("Not implemented yet"));

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
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
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
    OnChoice(*initCheckboxEvent);

}


static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if(os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

/**
 * timeout is in seconds
 **
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
