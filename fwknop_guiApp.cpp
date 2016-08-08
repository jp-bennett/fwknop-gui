/* fwknop_guiApp.cpp
 * Copyright (C) 2016  Jonathan Bennett
 * Fwknop-gui application class
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

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "fwknop_guiApp.h"
#include "fwknop_guiMain.h"

IMPLEMENT_APP(fwknop_guiApp);

bool fwknop_guiApp::OnInit()
{

    fwknop_guiFrame* frame = new fwknop_guiFrame(0L, _("Fwknop-gui"));
    frame->Show();

    return true;
}
