#include "wizard.h"

BEGIN_EVENT_TABLE(wizardDialog, wxDialog)
    EVT_BUTTON(ID_OKButton, wizardDialog::OnOK)
    EVT_BUTTON(ID_CopyButton, wizardDialog::OnCopy)
    EVT_CHECKBOX(ID_CMD_CHK, wizardDialog::OnUpdate)
    EVT_TEXT(ID_KEY_TXT, wizardDialog::OnUpdate)
END_EVENT_TABLE()

wizardDialog::wizardDialog(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(900, 330))
{


    fko_key_gen(generatedKey, FKO_DEFAULT_KEY_LEN, generatedHMAC, FKO_DEFAULT_HMAC_KEY_LEN, FKO_DEFAULT_HMAC_MODE);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *key_lbl = new wxStaticText(panel, -1, wxT("Enter text to use as key, or leave blank to generate."),
      wxPoint(15, 5));

    key_txt = new wxTextCtrl(panel, ID_KEY_TXT,
      wxT(""), wxPoint(15, 30));

    ChkAllowCmd = new wxCheckBox(panel, ID_CMD_CHK,
      wxT("Allow Command Message"), wxPoint(15, 70));

    tc = new wxTextCtrl(panel, -1, wxT(""),
      wxPoint(15, 105), wxSize(875, 225), wxTE_MULTILINE | wxTE_READONLY);

    wxButton *okButton = new wxButton(this, ID_OKButton, wxT("Ok"),
      wxDefaultPosition, wxSize(70, 30));

    wxButton *copyButton = new wxButton(this, ID_CopyButton, wxT("Copy"),
      wxDefaultPosition, wxSize(70, 30));

    hbox->Add(okButton, 1);
    hbox->Add(copyButton, 1);

    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();
    wxCommandEvent *initEvent = new wxCommandEvent(wxEVT_COMMAND_CHOICE_SELECTED, ID_KEY_TXT);
    OnUpdate(*initEvent);
    ShowModal();

    Destroy();
}

void wizardDialog::OnUpdate(wxCommandEvent &event)
{
    if (key_txt->GetLineText(0).IsEmpty()){
        keyString = _("KEY_BASE64 ");
        keyString.Append(wxString::FromAscii(generatedKey));
        keyString.Append(_("\n"));
    } else {
        keyString = _("KEY ");
        keyString.Append(key_txt->GetLineText(0));
        keyString.Append(_("\n"));
    }
    access_conf = _("SOURCE ANY\n");
    access_conf.Append(keyString);

    hmacString = _("HMAC_KEY_BASE64 ");
    hmacString.Append(wxString::FromAscii(generatedHMAC));
    hmacString.Append(_("\n"));
    access_conf.Append(hmacString);
    access_conf.Append(_("ENABLE_CMD_EXEC "));
    if (ChkAllowCmd->GetValue())
        access_conf.Append(_("Y"));
    else
        access_conf.Append(_("N"));

    tc->ChangeValue(access_conf);
}

void wizardDialog::OnCopy(wxCommandEvent &event)
{
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData( new wxTextDataObject(access_conf) );
        wxTheClipboard->Close();
    }
}

void wizardDialog::OnOK(wxCommandEvent &event)
{
  Destroy();
}

