#include "main.h"

static void exit_signal(int code) {
    exit(0);
}

static void quit() {
    disconnect_client();
    Py_Finalize();
    enet_deinitialize();
}

int main(int argc, const char *argv[]) {
    enet_initialize();
    Py_Initialize();
    signal(SIGINT, exit_signal);
    atexit(quit);

    connect_client();
    initSauerModule();

    PyObject *bstringModule = PyImport_ImportModule("binary_string");
    PyObject *bstringClass = PyObject_GetAttrString(bstringModule, "BinaryString");

    PyObject *mainModule = PyImport_ImportModule("main");
    Py_ERR(mainModule);
    PyObject *parseString = PyString_FromString("parse_packet");
    Py_ERR(parseString);

    while(1) {
        ENetEvent event;
        if(enet_host_service(client, &event, 0) <= 0)
            continue;
        if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            char str[event.packet->dataLength];
            strncpy(str, (const char *)event.packet->data, event.packet->dataLength);
            PyObject *bstring = PyObject_CallFunction(bstringClass, "s", str);
            Py_ERR(bstring);
            PyObject *res = PyObject_CallMethodObjArgs(mainModule, parseString, bstring, NULL);
            Py_ERR(res);
            long cres = PyInt_AsLong(res);
            if(cres != 0)
                return -1;
            Py_DECREF(res);
            Py_DECREF(bstring);
        }
        enet_packet_destroy(event.packet);
    }

    Py_DECREF(parseString);
    Py_DECREF(mainModule);

    Py_DECREF(bstringClass);
    Py_DECREF(bstringModule);

    return 0;
}

