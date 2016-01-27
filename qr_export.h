#include <wx/wx.h>
#include "configs.h"
#include <qrencode.h>

class qr_export : public wxDialog
{
public:
    qr_export(const wxString& title, const Config *selectedConfig);



private:
    wxBitmap bmp;


    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void render(wxDC& dc);
    DECLARE_EVENT_TABLE()
};
