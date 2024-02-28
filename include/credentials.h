#ifndef SHELL_CREDENTIALS
#define SHELL_CREDENTIALS
#include "IO.h"
/*
    TODO
        Create a secure way to register a new user and then store
        the user credentials in a file in the home dir.

        Should use some kind of encryption.

        Should be able to have different users and user creds.

        Should have a logout function that does not quit,
        but brings the user to a login screen.
*/

size_t str_hash(String str);



#endif /* SHELL_CREDENTIALS */