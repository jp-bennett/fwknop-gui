#include "gpgme_wrapper.h"

bool gpgme_wrapper::doInit(wxFileConfig * configFile) {
    gpgme_engine_info_t tmp_Info;
    configFile->SetPath(wxT("/"));

    //Starts the actual init
    gpgme_check_version(nullptr);
    gpgerr = gpgme_new(&gpgcon);
    if (gpgerr != GPG_ERR_NO_ERROR) {
        wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
        return 0;
    }

    tmp_Info = gpgme_ctx_get_engine_info(gpgcon);
    if (gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP) != GPG_ERR_NO_ERROR && (configFile->Read(wxT("show_gpg"), _("true")).CmpNoCase(_("true")) == 0))
    {
        if (wxGetOsVersion() & wxOS_WINDOWS) {
            wxRichMessageDialog dlg(NULL, _("GPG engine missing, launch browser to download?"), _("GPG engine missing"), wxYES_NO);
            dlg.ShowCheckBox("Don't show this dialog again");
            if (dlg.ShowModal() == wxID_YES) {
                wxMessageBox(_("The download will now begin, start fwknop-gui again after the installation is complete"));
                wxLaunchDefaultBrowser(_("https://files.gpg4win.org/gpg4win-latest.exe"));
            }
            if ( dlg.IsCheckBoxChecked() ) {
                configFile->Write(wxT("show_gpg"), _("false"));
                configFile->Flush();
            }
        } else if (wxGetOsVersion() & wxOS_MAC_OS) {
            wxRichMessageDialog dlg(NULL, _("GPG engine missing, launch browser to download?"), _("GPG engine missing"), wxYES_NO);
            dlg.ShowCheckBox("Don't show this dialog again");
            if (dlg.ShowModal() == wxID_YES) {
                wxMessageBox(_("The download will now begin, start fwknop-gui again after the installation is complete"));
                //wxLaunchDefaultBrowser(_("https://files.gpg4win.org/gpg4win-latest.exe"));
            }
            if ( dlg.IsCheckBoxChecked() ) {
                configFile->Write(wxT("show_gpg"), _("false"));
                configFile->Flush();
            }
        } else {
            wxRichMessageDialog dlg(NULL, _("GPG engine missing, please download"), _("GPG engine missing"));
            dlg.ShowCheckBox("Don't show this dialog again");
            dlg.ShowModal();
            if ( dlg.IsCheckBoxChecked() ) {
                configFile->Write(wxT("show_gpg"), _("false"));
                configFile->Flush();
            }
        }
    }
    //We grab the default settings before loading in the saved stuff
    gpgEngineDefault = _(tmp_Info->file_name);
    gpgFolderDefault = _(tmp_Info ->home_dir);
    gpgEngine = configFile->Read(wxT("gpg_engine"), _(tmp_Info->file_name));
    gpgHomeFolder = configFile->Read(wxT("gpg_home_folder"), _(tmp_Info->home_dir));
    if (gpgHomeFolder.IsEmpty())
        gpgme_ctx_set_engine_info(gpgcon, GPGME_PROTOCOL_OpenPGP, (const char*)gpgEngine.mb_str(wxConvUTF8), nullptr);
    else
        gpgme_ctx_set_engine_info(gpgcon, GPGME_PROTOCOL_OpenPGP, (const char*)gpgEngine.mb_str(wxConvUTF8), (const char*)gpgHomeFolder.mb_str(wxConvUTF8));
    enabled = true;
    return 1;
}

void gpgme_wrapper::getAllKeys(wxArrayString * keys) {
    gpgme_key_t tmpKey;
    keys->Empty();
    gpgerr = gpgme_op_keylist_start(gpgcon, 0, 0);
    if (gpgerr != GPG_ERR_NO_ERROR){
    wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
    return;
    }
    while (gpgme_op_keylist_next(gpgcon, &tmpKey) != GPG_ERR_EOF) {
        if (tmpKey == 0)
            break;
        keys->Insert(_(gpgme_key_get_string_attr(tmpKey, GPGME_ATTR_KEYID, 0, 0)).Right(8), 0);
    }
    gpgme_op_keylist_end(gpgcon);

}
//This function is not used, but is temporarily left for reference
/*
bool gpgme_wrapper::encryptAndSign(wxString encryptKey, wxString sigKey, char * plaintext, char * cipher) {
    gpgme_data_t plain_data;
    gpgme_data_t cipher_data;
    gpgme_key_t         key[2]      = { NULL, NULL };
    gpgme_key_t sig_key;
    size_t * buf_len;
    char *buf;
    char key_buf[64] = {0};
    buf_len = new size_t;
    char *ndx;

    gpgerr = gpgme_data_new_from_mem(&plain_data, plaintext, strlen(plaintext), 1);
    if (gpgerr != GPG_ERR_NO_ERROR) {
        wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
        return 0;
    }
    gpgerr = gpgme_data_new(&cipher_data);
    if (gpgerr != GPG_ERR_NO_ERROR) {
        wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
        return 0;
    }
    strcpy(key_buf, (const char*)encryptKey.mb_str(wxConvUTF8));
    gpgerr = gpgme_get_key(gpgcon, key_buf, &key[0], 0);
    if (gpgerr != GPG_ERR_NO_ERROR) {
        wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
        return 0;
    }
    gpgme_set_protocol(gpgcon, GPGME_PROTOCOL_OpenPGP);
    gpgme_set_armor(gpgcon, 0);

    gpgme_signers_clear(gpgcon);
    if (sigKey.CmpNoCase(wxT("None")) == 0) {
        gpgerr = gpgme_op_encrypt(gpgcon, key, GPGME_ENCRYPT_ALWAYS_TRUST, plain_data, cipher_data);
        if (gpgerr != GPG_ERR_NO_ERROR) {
            wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
            return 0;
        }



    } else {
        strcpy(key_buf, (const char*)sigKey.mb_str(wxConvUTF8));
        gpgerr = gpgme_get_key(gpgcon, key_buf, &sig_key, 0);
        if (gpgerr != GPG_ERR_NO_ERROR) {
            wxMessageBox(_("GPG returned the error: ") + _(gpgme_strerror(gpgerr)));
            return 0;
        }
        gpgme_signers_add(gpgcon, sig_key);
        gpgme_op_encrypt_sign(gpgcon, key, GPGME_ENCRYPT_ALWAYS_TRUST, plain_data, cipher_data);
    }
    buf = gpgme_data_release_and_get_mem(cipher_data, buf_len);
    if (buf == nullptr)
        return 0;
    wxBase64Encode(cipher, 4096, buf, *buf_len);
    if((ndx = strchr(cipher, '=')) != NULL)
        *ndx = '\0';
//base64 encode before returning
return 1;
}
*/
bool gpgme_wrapper::selectHomeDir(wxFileConfig * configFile) {
    configFile->SetPath(wxT("/"));
    wxDirDialog dlg(NULL, _("Choose GPG directory"), gpgHomeFolder, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    if(dlg.ShowModal() == wxID_OK){
        gpgHomeFolder = dlg.GetPath();
        configFile->Write(_("gpg_home_folder"), gpgHomeFolder);
        configFile->Flush();
        gpgme_ctx_set_engine_info(gpgcon, GPGME_PROTOCOL_OpenPGP, (const char*)gpgEngine.mb_str(wxConvUTF8), (const char*)gpgHomeFolder.mb_str(wxConvUTF8));
        return true;
    } else {
        return false;
    }

}

bool gpgme_wrapper::selectEngine(wxFileConfig * configFile) {
    configFile->SetPath(wxT("/"));
    wxFileDialog dlg(NULL, _("Choose gpg or gpg2 executable"), wxEmptyString, gpgEngine);
    if (dlg.ShowModal() == wxID_OK){
        gpgEngine = dlg.GetPath();
        configFile->Write(_("gpg_engine"), gpgEngine);
        configFile->Flush();
        gpgme_ctx_set_engine_info(gpgcon, GPGME_PROTOCOL_OpenPGP, (const char*)gpgEngine.mb_str(wxConvUTF8), (const char*)gpgHomeFolder.mb_str(wxConvUTF8));
        return true;
    } else {
        return false;
    }
}

bool gpgme_wrapper::setDefaults(wxFileConfig * configFile) {
    configFile->SetPath(wxT("/"));
    wxMessageDialog dlg(NULL, _("Are you sure you want to reset GPG?"), _("Confirm"), wxYES_NO);
    if (dlg.ShowModal() == wxID_YES){
        gpgEngine = gpgEngineDefault;
        gpgHomeFolder = gpgFolderDefault;
        configFile->Write(_("gpg_engine"), gpgEngine);
        configFile->Write(_("gpg_home_folder"), wxEmptyString);
        configFile->Flush();
        gpgme_ctx_set_engine_info(gpgcon, GPGME_PROTOCOL_OpenPGP, (const char*)gpgEngine.mb_str(wxConvUTF8), nullptr);
        return true;
    } else {
        return false;
    }
}
