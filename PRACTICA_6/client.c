#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <MQTTClient.h>
#include "mqtt.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected\n");

        connect_frame conn_frame = {0};
        conn_frame.header = PREAMB;
        conn_frame.length = sizeof(conn_frame);
        conn_frame.protocol_name_len = 4;
        memcpy(conn_frame.protocol_name, "MQTT", 4);
        conn_frame.protocol_version = 4;
        conn_frame.connect_flags = 0;
        conn_frame.keep_alive = 60;
        conn_frame.client_id_len = 7;
        memcpy(conn_frame.client_id, "client1", 7);

        MQTTClient_message msg = MQTTClient_message_initializer;
        MQTTClient_deliveryToken token;

        msg.payload = (void*)&conn_frame;
        msg.payloadlen = sizeof(conn_frame);
        msg.qos = QOS;
        msg.retained = 0;

        MQTTClient_publishMessage(client, "example/topic", &msg, &token);
        printf("Waiting for publication of connect_frame\n"
               "on topic %s for client with ClientID: %s\n",
               "example/topic", CLIENTID);

        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}

