#include <wx/wx.h>
#include <wx/fileconf.h>

class gConfigDialog : public wxDialog
{
public:
  gConfigDialog(wxFileConfig *configFile);

private:

  wxTextCtrl *url_txt;
  wxFileConfig *privateConfigFile;

  enum
    {
    ID_OKButton = 1200,
    ID_DefButton,
    ID_CancelButton
    };

  void OnDef(wxCommandEvent& event);
  void OnOK(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  DECLARE_EVENT_TABLE()
};
