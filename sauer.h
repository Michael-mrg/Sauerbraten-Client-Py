#include <enet/enet.h>
#include <Python.h>

ENetHost *client;
ENetPeer *peer;

int connect_client();
int disconnect_client();
void initSauerModule();

#define Py_ERR(x) \
    if(!x) { \
        if(PyErr_Occurred()) \
            PyErr_Print(); \
        return -1; \
    }
    
