#include "rc_import.h"

BEGIN_EVENT_TABLE(rc_import, wxDialog)
    EVT_BUTTON(ID_OKButton, rc_import::OnOK)
    EVT_LISTBOX(ID_ListBox, rc_import::OnList)
    EVT_BUTTON(ID_CancelButton, rc_import::OnCancel)
END_EVENT_TABLE()

rc_import::rc_import(const wxString & title, Config *ourGivenConfig, bool *is_changed)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(900, 300))
{
    ourConfig = ourGivenConfig;
    InternalIsChanged = is_changed;
    configs = new wxArrayString;
    wxString singleLine;

    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxFileDialog getRC(this, _("fwknoprc file to import"), stdp.GetDocumentsDir(), wxT(".fwknoprc"));
    if (getRC.ShowModal() == wxID_CANCEL){
    Destroy();
    return;
    }

    filename = getRC.GetPath();
    if (!rcFile.Open(filename)) {
    wxMessageBox(_("Could not open file"));
    Destroy();
    return;
    }

    rcFile.GoToLine(0);

    while(!rcFile.Eof())
    {
        singleLine = rcFile.GetNextLine();
        singleLine.Trim(0);
        singleLine.Trim(1);
        if (singleLine.StartsWith(wxT("#")) || singleLine.IsEmpty())
            continue;
        if (singleLine.StartsWith(wxT("[")) && singleLine.EndsWith(wxT("]"))){
            singleLine.RemoveLast();
            singleLine.Remove(0, 1);
            if (singleLine.CmpNoCase(wxT("default")) != 0)
            configs->Add(singleLine);
        }

    }
    if (configs->GetCount() == 0){
    wxMessageBox(_("No stanzas in file"));
    Destroy();
    return;
    }
//if configs are blank, then bail

//open file, find the stanza names, let the user select which stanza/s to import (first iteration

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(this, ID_OKButton, wxT("Ok"),
      wxDefaultPosition, wxSize(70, 30));
    wxButton *cancelButton = new wxButton(this, ID_CancelButton, wxT("Cancel"),
      wxDefaultPosition, wxSize(70, 30));

    hbox->Add(okButton, 1);
    hbox->Add(cancelButton, 1);

    listbox = new wxListBox(this, ID_ListBox, wxPoint(10, 10), wxSize(200, 200));
    listbox->InsertItems(*configs, 0);

    tc = new wxTextCtrl(panel, -1, wxT(""),
      wxPoint(220, 10), wxSize(650, 200), wxTE_MULTILINE | wxTE_READONLY);
    tc->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();

    ShowModal();

    Destroy();
}

void rc_import::OnCancel(wxCommandEvent &event)
{
    Destroy();
}

void rc_import::OnOK(wxCommandEvent &event)
{
    wxString singleLine;
    //ourConfig = new Config;
    ourConfig->defaultConfig();

    rcFile.GoToLine(0);

    while(!rcFile.Eof())
    {
        singleLine = rcFile.GetNextLine();
        singleLine.Trim(0);
        singleLine.Trim(1);
        if (singleLine.StartsWith(wxT("#")) || singleLine.IsEmpty())
            continue;
        if (singleLine.StartsWith(wxT("[")) && singleLine.EndsWith(wxT("]"))){
            singleLine.RemoveLast();
            singleLine.Remove(0, 1);
            if (singleLine.CmpNoCase(listbox->GetString(listbox->GetSelection())) == 0)
                break;
        }

    }
    ourConfig->NICK_NAME = singleLine;
    ourConfig->MESS_TYPE = wxT("");
    //rc_Pair = new wxArrayString;
    while(!rcFile.Eof())
    {
        singleLine = rcFile.GetNextLine();
        singleLine.Trim(0);
        singleLine.Trim(1);
        if (singleLine.StartsWith(wxT("#")) || singleLine.IsEmpty())
            continue;
        if (singleLine.StartsWith(wxT("[")) && singleLine.EndsWith(wxT("]"))){
                break; //once we hit the next stanza, we're done
        }
        rc_Pair = wxStringTokenize(singleLine);//buse wxStringTokenize
        if (rc_Pair[0].CmpNoCase(wxT("SPA_SERVER")) == 0)
            ourConfig->SERVER_IP = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("ENCRYPTION_MODE")) == 0 && rc_Pair[1].CmpNoCase(wxT("legacy")) == 0)
            ourConfig->LEGACY = true;
        else if (rc_Pair[0].CmpNoCase(wxT("RAND_PORT")) == 0 && rc_Pair[1].CmpNoCase(wxT("Y")) == 0)
            ourConfig->SERVER_PORT = wxT("random");
        else if (rc_Pair[0].CmpNoCase(wxT("SPA_SERVER_PORT")) == 0)
            ourConfig->SERVER_PORT = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("SPA_SERVER_PROTO")) == 0)
            ourConfig->PROTOCOL = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("DIGEST_TYPE")) == 0)
            ourConfig->DIGEST_TYPE = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("HMAC_DIGEST_TYPE")) == 0)
            ourConfig->HMAC_TYPE = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("HMAC_KEY_BASE64")) == 0){
            ourConfig->HMAC = rc_Pair[1];
            ourConfig->HMAC_BASE64 = true;
        }
        else if (rc_Pair[0].CmpNoCase(wxT("HMAC_KEY")) == 0){
            ourConfig->HMAC = rc_Pair[1];
            ourConfig->HMAC_BASE64 = false;
        }
        else if (rc_Pair[0].CmpNoCase(wxT("KEY_BASE64")) == 0){
            ourConfig->KEY = rc_Pair[1];
            ourConfig->KEY_BASE64 = true;
        }
        else if (rc_Pair[0].CmpNoCase(wxT("KEY")) == 0){
            ourConfig->KEY = rc_Pair[1];
            ourConfig->KEY_BASE64 = false;
        }
        else if (rc_Pair[0].CmpNoCase(wxT("CLIENT_TIMEOUT")) == 0)
            ourConfig->SERVER_TIMEOUT = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("ACCESS")) == 0)
            ourConfig->PORTS = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("NAT_LOCAL")) == 0 && rc_Pair[1].CmpNoCase(wxT("Y")) == 0)
            ourConfig->MESS_TYPE = wxT("Local Nat Access");
        else if (rc_Pair[0].CmpNoCase(wxT("NAT_PORT")) == 0)
            ourConfig->NAT_PORT = rc_Pair[1];
        else if (rc_Pair[0].CmpNoCase(wxT("NAT_ACCESS")) == 0) {
            ourConfig->NAT_IP = rc_Pair[1].BeforeFirst(',');
            ourConfig->NAT_PORT = rc_Pair[1].AfterLast(',');
        }



        if (ourConfig->MESS_TYPE.IsEmpty())
            ourConfig->MESS_TYPE = wxT("Open Port");


    }

//itirate through file to find stanza start, and specified options
//fill config
//save

    wxMessageBox(wxT("Import Successful, Press Save to confirm"));
    *InternalIsChanged = true;
    Destroy();
}

void rc_import::OnList(wxCommandEvent &event)
{
    wxString singleLine;
    preview = wxT("[");
    rcFile.GoToLine(0);

    while(!rcFile.Eof()) //this loop puts our file cursor at the first line of the config we are importing
    {

        singleLine = rcFile.GetNextLine();
        singleLine.Trim(0);
        singleLine.Trim(1);
        if (singleLine.StartsWith(wxT("#")) || singleLine.IsEmpty())
            continue;
        if (singleLine.StartsWith(wxT("[")) && singleLine.EndsWith(wxT("]"))){
            singleLine.RemoveLast();
            singleLine.Remove(0, 1);
            if (singleLine.CmpNoCase(listbox->GetString(listbox->GetSelection())) == 0)
                break;
        }

    }
    preview.Append(singleLine);
    preview.Append(wxT("]\n"));

    while(!rcFile.Eof()) //This loop dumps that stanza to the preview
    {
        singleLine = rcFile.GetNextLine();
        if (singleLine.Find(wxT("[")) != wxNOT_FOUND && singleLine.Find(wxT("]")) != wxNOT_FOUND )
                break; //once we hit the next stanza, we're done
        preview.Append(singleLine);
        preview.Append(wxT("\n"));
    }



    tc->ChangeValue(preview);

}
