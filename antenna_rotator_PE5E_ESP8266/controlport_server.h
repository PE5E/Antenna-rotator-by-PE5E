#ifndef CONTROLPORT_SERVER_H
#define CONTROLPORT_SERVER_H

// controlport server
#include "rotctl_implementation.h" // implementation of the rotctl protocol

#define SERVER_HOST_NAME "controlport_server"

static std::vector<AsyncClient*> clients; // a list to hold all clients

Rotctl_impl rotctl; // translates rotctldata to actions

 /* clients events */
static void handleError(void* arg, AsyncClient* client, int8_t error) {
  Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
//  Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
//  Serial.write((char*)data, len);

  String request = "";
  for (int index = 0; index != len; index++) {
    request += ((char*)data)[index];
  }
  

  String reply = rotctl.input_data(request);

  // reply to client
  if (client->space() > reply.length() && client->canSend()) {
    client->add(reply.c_str(), reply.length());
    client->send();
  }
}

static void handleDisconnect(void* arg, AsyncClient* client) {
  Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
  Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}


/* server events */
static void handleNewClient(void* arg, AsyncClient* client) {
  Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

  // add to list
  clients.push_back(client);
  
  // register events
  client->onData(&handleData, NULL);
  client->onError(&handleError, NULL);
  client->onDisconnect(&handleDisconnect, NULL);
  client->onTimeout(&handleTimeOut, NULL);
}

#endif // CONTROLPORT_SERVER_H
