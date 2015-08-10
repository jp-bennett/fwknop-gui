/***************************************************************
 * Name:      fwknop_guiApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Jonathan Bennett (JBennett@incomsystems.biz)
 * Created:   2015-08-05
 * Copyright: Jonathan Bennett (incomsystems.biz)
 * License:
 **************************************************************/

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
