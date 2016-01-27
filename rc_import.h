#include <wx/wx.h>
#include <wx/stdpaths.h>
#include "configs.h"
#include <wx/tokenzr.h>

class rc_import : public wxDialog
{
public:
  rc_import(const wxString& title, Config *ourGivenConfig, bool *is_changed);

private:
    wxListBox      *listbox;
    wxArrayString  *configs;
    wxArrayString  rc_Pair;
    wxString        filename;
    wxTextFile      rcFile;
    Config         *ourConfig;
    wxTextCtrl     *tc;
    wxString        preview;
    bool           *InternalIsChanged;

enum
    {
    ID_OKButton = 1200,
    ID_ListBox,
    ID_CancelButton
    };

    void OnOK(wxCommandEvent& event);
    void OnList(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};
