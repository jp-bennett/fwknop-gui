/* qr_export.h
 * Copyright (C) 2016  Jonathan Bennett
 * Header for qr_export.cpp
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
