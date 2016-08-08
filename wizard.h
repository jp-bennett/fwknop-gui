/* wizard.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header for wizard.cpp
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

#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <fko.h>
#include "configs.h"
#define MAX_KEY_LEN                 128
#define MAX_B64_KEY_LEN             180
class wizardDialog : public wxDialog
{
public:
  wizardDialog(const wxString& title);

  Config *tmp_config;

private:

    wxTextCtrl *key_txt;
    wxCheckBox *ChkAllowCmd;
    wxCheckBox *ChkReqSrc;
    wxString keyString;
    char generatedKey[MAX_B64_KEY_LEN+1];
    char generatedHMAC[MAX_B64_KEY_LEN+1];

    wxString access_conf;
    wxString hmacString;
    wxTextCtrl *tc;
    enum
    {
    ID_OKButton = 1100,
    ID_CopyButton,
    ID_KEY_TXT,
    ID_CMD_CHK,
    ID_SRC_CHK
    };

  void OnUpdate(wxCommandEvent& event);
  void OnOK(wxCommandEvent& event);
  void OnCopy(wxCommandEvent& event);
  DECLARE_EVENT_TABLE()
};
