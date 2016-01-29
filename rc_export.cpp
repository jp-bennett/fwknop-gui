#include "rc_export.h"

rc_export::rc_export(const wxString & title, const Config *selectedConfig)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(900, 330))
{
    wxString configBuf = wxT("");
    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    //wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
//    hbox->Add(okButton, 1);
//    hbox->Add(copyButton, 1);

    tc = new wxTextCtrl(panel, -1, wxT(""),
      wxPoint(15, 80), wxSize(875, 200), wxTE_MULTILINE | wxTE_READONLY);
    tc->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    configBuf = wxT("[");
    configBuf.Append(selectedConfig->NICK_NAME);
    configBuf.Append(wxT("]"));
    configBuf.Append(wxT("\nSPA_SERVER          "));
    configBuf.Append(selectedConfig->SERVER_IP);
    if (selectedConfig->LEGACY)
        configBuf.Append(wxT("\nENCRYPTION_MODE     legacy"));

    if (selectedConfig->SERVER_PORT.CmpNoCase(_("random")) == 0) {
        configBuf.Append(wxT("\nRAND_PORT           Y"));
    } else {
        configBuf.Append(wxT("\nSPA_SERVER_PORT     "));
        configBuf.Append(selectedConfig->SERVER_PORT);
    }
    configBuf.Append(wxT("\nSPA_SERVER_PROTO    "));
    configBuf.Append(selectedConfig->PROTOCOL);
    configBuf.Append(wxT("\nDIGEST_TYPE         "));
    configBuf.Append(selectedConfig->DIGEST_TYPE);
    configBuf.Append(wxT("\nHMAC_DIGEST_TYPE    "));
    configBuf.Append(selectedConfig->HMAC_TYPE);

    if (!selectedConfig->HMAC.IsEmpty()) {
        configBuf.Append(wxT("\nUSE_HMAC            Y"));
        if (selectedConfig->HMAC_BASE64)
            configBuf.Append(wxT("\nHMAC_KEY_BASE64     "));
        else
            configBuf.Append(wxT("\nHMAC_KEY            "));
        configBuf.Append(selectedConfig->HMAC);
    }

    if (!selectedConfig->KEY.IsEmpty()) {
        if (selectedConfig->KEY_BASE64)
            configBuf.Append(wxT("\nKEY_BASE64          "));
        else
            configBuf.Append(wxT("\nKEY                 "));
        configBuf.Append(selectedConfig->KEY);
    }

    if (selectedConfig->MESS_TYPE.CmpNoCase(_("Server Command")) == 0) { // fwknoprc seems not to have support for server commands

    } else if (selectedConfig->MESS_TYPE.CmpNoCase(_("Open Port")) == 0) {
        configBuf.Append(wxT("\nACCESS              "));
        configBuf.Append(selectedConfig->PORTS);
        if(!selectedConfig->SERVER_TIMEOUT.IsEmpty()) {
            configBuf.Append(wxT("\nCLIENT_TIMEOUT      "));
            configBuf.Append(selectedConfig->SERVER_TIMEOUT);
        }
    } else if (selectedConfig->MESS_TYPE.CmpNoCase(_("Nat Access")) == 0) {
        configBuf.Append(wxT("\nACCESS              "));
        configBuf.Append(selectedConfig->PORTS);
        configBuf.Append(wxT("\nNAT_ACCESS          "));
        configBuf.Append(selectedConfig->NAT_IP);
        configBuf.Append(wxT(","));
        configBuf.Append(selectedConfig->NAT_PORT);

        if(!selectedConfig->SERVER_TIMEOUT.IsEmpty()) {
            configBuf.Append(wxT("\nCLIENT_TIMEOUT      "));
            configBuf.Append(selectedConfig->SERVER_TIMEOUT);
        }
    }  else if (selectedConfig->MESS_TYPE.CmpNoCase(_("Local Nat Access")) == 0) {
        configBuf.Append(wxT("\nACCESS              "));
        configBuf.Append(selectedConfig->PORTS);
        configBuf.Append(wxT("\nNAT_LOCAL           Y"));
        configBuf.Append(wxT("\nNAT_PORT            "));
        configBuf.Append(selectedConfig->NAT_PORT);

        if(!selectedConfig->SERVER_TIMEOUT.IsEmpty()) {
            configBuf.Append(wxT("\nCLIENT_TIMEOUT      "));
            configBuf.Append(selectedConfig->SERVER_TIMEOUT);
        }
    }



    tc->ChangeValue(configBuf);
    vbox->Add(panel, 1);
    //vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);



    Centre();

    ShowModal();

    Destroy();
}
