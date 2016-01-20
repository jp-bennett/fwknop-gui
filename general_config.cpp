#include "general_config.h"

BEGIN_EVENT_TABLE(gConfigDialog, wxDialog)
    EVT_BUTTON(ID_DefButton, gConfigDialog::OnDef)
    EVT_BUTTON(ID_OKButton, gConfigDialog::OnOK)
    EVT_BUTTON(ID_CancelButton, gConfigDialog::OnCancel)
END_EVENT_TABLE()


gConfigDialog::gConfigDialog(wxFileConfig  *configFile)
    : wxDialog(NULL, -1, _("Config"), wxDefaultPosition, wxSize(-1, -1))
{
    privateConfigFile = configFile;
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    privateConfigFile->SetPath(wxT("/"));

    wxStaticText *key_lbl = new wxStaticText(panel, -1, wxT("Enter URL to use to resolve the local IP."),
      wxPoint(15, 5));

    url_txt = new wxTextCtrl(panel, -1,
      configFile->Read(wxT("ip_resolver_url"), _("https://api.ipify.org")) , wxPoint(15, 30), wxSize(350, 30));

    wxButton *defaultButton = new wxButton(panel, ID_DefButton, wxT("Set to Default"), wxPoint(15, 70));

    wxButton *okButton = new wxButton(this, ID_OKButton, wxT("Ok"),
      wxDefaultPosition, wxSize(70, 30));

    wxButton *cancelButton = new wxButton(this, ID_CancelButton, wxT("Cancel"),
      wxDefaultPosition, wxSize(70, 30));

    hbox->Add(okButton, 1);
    hbox->Add(cancelButton, 1);


    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();
    ShowModal();

    Destroy();
}

void gConfigDialog::OnDef(wxCommandEvent &event)
{
    url_txt->ChangeValue(wxT("https://api.ipify.org"));
}

void gConfigDialog::OnOK(wxCommandEvent &event)
{
    wxString tmp_url = url_txt->GetValue();
    if (tmp_url.IsEmpty()) {
        wxMessageBox(_("URL cannot be empty!"));
        return;
    } else {
        privateConfigFile->Write(wxT("ip_resolver_url"), tmp_url);
        privateConfigFile->Flush();
        Destroy();
    }
}

void gConfigDialog::OnCancel(wxCommandEvent &event)
{
    Destroy();
}
