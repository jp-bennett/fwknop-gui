/* wizard.cpp
 * Copyright (C) 2016  Jonathan Bennett
 * Access.conf wizard class
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "wizard.h"

BEGIN_EVENT_TABLE(wizardDialog, wxDialog)
    EVT_BUTTON(ID_OKButton, wizardDialog::OnOK)
    EVT_BUTTON(ID_CopyButton, wizardDialog::OnCopy)
    EVT_CHECKBOX(ID_CMD_CHK, wizardDialog::OnUpdate)
    EVT_CHECKBOX(ID_SRC_CHK, wizardDialog::OnUpdate)
    EVT_TEXT(ID_KEY_TXT, wizardDialog::OnUpdate)
END_EVENT_TABLE()

wizardDialog::wizardDialog(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(900, 360))
{

    tmp_config = new Config;
    fko_key_gen(generatedKey, FKO_DEFAULT_KEY_LEN, generatedHMAC, FKO_DEFAULT_HMAC_KEY_LEN, FKO_DEFAULT_HMAC_MODE);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    new wxStaticText(panel, -1, wxT("Enter text to use as key, or leave blank to use the generated key."),
      wxPoint(15, 5));

    key_txt = new wxTextCtrl(panel, ID_KEY_TXT,
      wxT(""), wxPoint(15, 30));

    ChkAllowCmd = new wxCheckBox(panel, ID_CMD_CHK,
      wxT("Allow Command Message"), wxPoint(15, 70));

    ChkReqSrc = new wxCheckBox(panel, ID_SRC_CHK,
      wxT("Require Source Address"), wxPoint(15, 100));
    ChkReqSrc->SetValue(true);

    tc = new wxTextCtrl(panel, -1, wxT(""),
      wxPoint(15, 135), wxSize(875, 225), wxTE_MULTILINE | wxTE_READONLY);

    wxButton *okButton = new wxButton(this, ID_OKButton, wxT("Ok"),
      wxDefaultPosition, wxSize(70, 30));

    wxButton *copyButton = new wxButton(this, ID_CopyButton, wxT("Copy to Clipboard"),
      wxDefaultPosition, wxSize(170, 30));

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

    if (ChkAllowCmd->GetValue()) {
        access_conf.Append(_("ENABLE_CMD_EXEC "));
        access_conf.Append(_("Y\n"));
    }
    if (ChkReqSrc->GetValue()) {
        access_conf.Append(_("REQUIRE_SOURCE_ADDRESS "));
        access_conf.Append(_("Y\n"));
    }
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
    if (key_txt->GetLineText(0).IsEmpty()){
    tmp_config->KEY = wxString::FromAscii(generatedKey);
    tmp_config->KEY_BASE64 = true;
    } else {
    tmp_config->KEY = key_txt->GetLineText(0);
    tmp_config->KEY_BASE64 = false;
    }
    tmp_config->HMAC = wxString::FromAscii(generatedHMAC);
    EndModal( wxID_OK);
}

