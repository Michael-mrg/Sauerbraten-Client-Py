#include "sauer.h"

PyObject *sauer;

int connect_client() {
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 28785;

    client = enet_host_create(NULL, 2, 0, 0);
    peer = enet_host_connect(client, &address, 2);
    ENetEvent event;
    if(enet_host_service(client, &event, 5000) <= 0 || event.type != ENET_EVENT_TYPE_CONNECT) {
        enet_peer_reset(peer);
        return -1;
    }
    return 0;
}

int disconnect_client() {
    char data[] = "\x07\0";
    PyObject *cn = PyObject_GetAttrString(sauer, "client_num");
    if(!cn)
        return 0;
    data[1] = PyInt_AsLong(cn);
    Py_DECREF(cn);
    ENetPacket *packet = enet_packet_create(data, 3, 0);
    enet_peer_send(peer, 1, packet);
    enet_peer_disconnect(peer, 1);
    enet_host_flush(client);
    enet_peer_reset(peer);
    enet_host_destroy(client);
    return 0;
}

static PyObject *disconnect_client_py(PyObject *self, PyObject *args) {
    disconnect_client();
    return Py_None;
}

static PyObject *send_packet(PyObject *self, PyObject *args) {
    PyObject *str = PyTuple_GetItem(args, 0);
    ENetPacket *packet = enet_packet_create(PyString_AsString(str), PyString_Size(str), 0);
    enet_peer_send(peer, 1, packet);
    return Py_None;
}

static PyMethodDef methods[] = {
    {"send_packet", send_packet, METH_VARARGS, "Send a packet to the server."},
    {"disconnect", disconnect_client_py, METH_VARARGS, "Disconnects the client."},
    {NULL, NULL, 0, NULL}
};

void initSauerModule() {
    sauer = Py_InitModule("sauer", methods);
    PyObject *cn = PyInt_FromLong(-1);
    PyObject_SetAttrString(sauer, "client_num", cn);
    Py_DECREF(cn);
}

