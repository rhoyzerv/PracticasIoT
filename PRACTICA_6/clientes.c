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

typedef struct
{
    uint8_t header;
    uint8_t length;
    uint8_t protocol_name_len;
    uint8_t protocol_name[6];
    uint8_t protocol_version;
    uint8_t connect_flags;
    uint16_t keep_alive;
    uint16_t client_id_len;
    uint8_t client_id[23];
} mqtt_connect_frame;

typedef struct {
    uint8_t header;
    uint8_t length;
    uint8_t connack_flags;
    uint8_t return_code;
} mqtt_connack_frame;

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

        mqtt_connect_frame msg = {PREAMB, sizeof(mqtt_connect_frame), 6, "MQTT", 4, 2, htons(60), htons(23), "ExampleClientSub"};
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = &msg;
        pubmsg.payloadlen = sizeof(mqtt_connect_frame);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_deliveryToken token;
        MQTTClient_publishMessage(client, "example/topic", &pubmsg, &token);
        printf("Waiting for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               "Hello MQTT", "example/topic", CLIENTID);

        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
   }
