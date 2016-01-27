#include "qr_export.h"

BEGIN_EVENT_TABLE(qr_export, wxDialog)

// catch paint events
EVT_PAINT(qr_export::paintEvent)

END_EVENT_TABLE()

qr_export::qr_export(const wxString & title, const Config *selectedConfig)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(150, 150))
{
    int i;
    char tmpString[10024];
    QRcode *ourQR = NULL;
    wxImage *ourQRImage;
    wxString QRBuf = wxT("");
    if (selectedConfig->KEY_BASE64)
    QRBuf = (wxT("KEY_BASE64:"));
    else
    QRBuf = (wxT("KEY:"));
    QRBuf.Append(selectedConfig->KEY);
    if (!selectedConfig->HMAC.IsEmpty()) {
        if (selectedConfig->HMAC_BASE64)
        QRBuf.Append(wxT(" HMAC_KEY_BASE64:"));
        else
        QRBuf.Append(wxT(" HMAC_KEY:"));
        QRBuf.Append(selectedConfig->HMAC);
    }
    strncpy(tmpString, (const char*)QRBuf.mb_str(wxConvUTF8), 10023);
    ourQR = QRcode_encodeString((char *)tmpString, 0, QR_ECLEVEL_Q, QR_MODE_8, 1);

        unsigned char *rawImage = new unsigned char[ourQR->width * ourQR->width * 3];
    unsigned char *p = ourQR->data;
    for (i=0; i < (ourQR->width * ourQR->width); i++) {
        if (p[i] & 0x00000001) {
            rawImage[i * 3] = 0x00;
            rawImage[i * 3 + 1] = 0x00;
            rawImage[i * 3 + 2] = 0x00;
        } else {
            rawImage[i * 3] = 0xff;
            rawImage[i * 3 + 1] = 0xff;
            rawImage[i * 3 + 2] = 0xff;

        }

    }
    ourQRImage = new wxImage(ourQR->width, ourQR->width, rawImage);
    ourQRImage->Rescale(ourQR->width * 4, ourQR->width * 4);
    bmp = wxBitmap(*ourQRImage);
    if (ourQR->width * 4 > 100)
    this->SetSize(wxDefaultCoord, wxDefaultCoord, ourQR->width * 4 + 50, ourQR->width * 4 + 50);
    Centre();

    ShowModal();

    Destroy();
    QRcode_free(ourQR);
}

void qr_export::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

void qr_export::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void qr_export::render(wxDC&  dc)
{
    dc.DrawBitmap( bmp, 15, 15, false );
}
