#include <wx/wx.h>
#include "configs.h"

class rc_export : public wxDialog
{
public:
  rc_export(const wxString& title, const Config *selectedConfig);

private:
  wxTextCtrl *tc;
};
