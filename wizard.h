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
