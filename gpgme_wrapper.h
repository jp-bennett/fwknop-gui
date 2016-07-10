#ifndef GPGME_WRAPPER
#define GPGME_WRAPPER
#include <gpgme.h>  //This seems strange, not sure how to handle this.  Ship with my code?
#include <wx/dynlib.h>
#include <wx/wx.h>
#include <wx/base64.h>
#include <wx/config.h>
#include <wx/fileconf.h>

class gpgme_wrapper {
    public:
        bool enabled;
        wxString currError;  // or make a function or two?

        bool doInit(wxFileConfig * configFile);
        void getAllKeys(wxArrayString * keys);
        bool encryptAndSign(wxString encryptKey, wxString sigKey, char * plaintext, char * cipher);
        bool selectHomeDir(wxFileConfig * configFile);
        bool selectEngine(wxFileConfig * configFile);
        bool setDefaults(wxFileConfig * configFile);
        wxString gpgEngine;
        wxString gpgHomeFolder;
        // get key list function
        // encrypt data function
        //sign data function
        //eventually additional functions like key import and management


    private:
    /*
        //Pointers that will contain the functions we're interested in
        gpgme_error_t (* gpgme_new_ptr) (gpgme_ctx_t *ctx); //Defines the pointer for gpgme_new
        const char * (* gpgme_strerror_ptr) (gpgme_error_t err);//Defines the pointer for gpgme_strerror
        const char * (* gpgme_check_version_ptr) (const char *required_version);//Defines the pointer for gpgme_check_version
        gpgme_error_t (* gpgme_op_keylist_start_ptr) (gpgme_ctx_t ctx, const char *pattern, int secret_only);
        gpgme_error_t (* gpgme_op_keylist_next_ptr) (gpgme_ctx_t ctx, gpgme_key_t *r_key);
        gpgme_error_t (* gpgme_op_keylist_end_ptr) (gpgme_ctx_t ctx);
        gpgme_error_t (* gpgme_get_key_ptr) (gpgme_ctx_t ctx, const char *fpr, gpgme_key_t *r_key, int secret);
        const char * (* gpgme_key_get_string_attr_ptr) (gpgme_key_t key, gpgme_attr_t what, const void *reserved, int idx);//Defines the pointer for gpgme_key_get_string_attr
        gpgme_error_t (* gpgme_data_new_from_mem_ptr) (gpgme_data_t *dh, const char *buffer, size_t size, int copy);
        gpgme_error_t (* gpgme_data_new_ptr) (gpgme_data_t *dh);
        char * (* gpgme_data_release_and_get_mem_ptr) (gpgme_data_t dh, size_t *length);
        void (* gpgme_signers_clear_ptr) (gpgme_ctx_t ctx);
        gpgme_error_t (* gpgme_signers_add_ptr) (gpgme_ctx_t ctx, const gpgme_key_t key);
        gpgme_error_t (* gpgme_op_encrypt_ptr) (gpgme_ctx_t ctx, gpgme_key_t recp[], gpgme_encrypt_flags_t flags, gpgme_data_t plain, gpgme_data_t cipher);
        gpgme_error_t (* gpgme_op_encrypt_sign_ptr) (gpgme_ctx_t ctx, gpgme_key_t recp[], gpgme_encrypt_flags_t flags, gpgme_data_t plain, gpgme_data_t cipher);
        gpgme_error_t (* gpgme_set_protocol_ptr) (gpgme_ctx_t ctx, gpgme_protocol_t proto);
        void (* gpgme_set_armor_ptr) (gpgme_ctx_t ctx, int yes);

        wxDynamicLibrary dl;
        */
        gpgme_ctx_t gpgcon;
        gpgme_error_t gpgerr;
        wxString gpgEngineDefault;

};
#endif
