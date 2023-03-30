#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <MQTTClient.h>
#include "mqtt.h"
#define ADDRESS "192.168.43.3"
#define CLIENTID "servidor1"
#define TOPIC "test"
#define QOS 1
#define TIMEOUT 10000 L
void messageArrived(void *context, char *topicName, int topicLen,
	MQTTClient_message *message)
{
	printf("Mensaje recibido en el topico: %s\n", topicName);
	printf("Contenido del mensaje: %.*s\n", message->payloadlen, (char*) message->payload);
	if (message->payloadlen == sizeof(mqtt_connect_frame_t))
	{
		mqtt_connect_frame_t *connect_frame = (mqtt_connect_frame_t*)
		message->payload;
		if (connect_frame->type == MQTT_CONNECT)
		{
			printf("Frame MQTT CONNECT recibido del cliente\n");
			printf("Protocolo: %.*s\n", connect_frame->protocol_name_length,
				connect_frame->protocol_name);
			printf("Versión: 0x%02x\n", connect_frame->protocol_version);
			printf("Keep alive: %d\n", ntohs(connect_frame->keep_alive));
		}
	}

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
}

int main(int argc, char *argv[])
{
	MQTTClient client;
	MQTTClient_connectOptions conn_opts =
		MQTTClient_connectOptions_initializer;
	int rc;
	MQTTClient_create(&client, ADDRESS, CLIENTID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Error al conectar al broker MQTT, codigo de retorno: %d\n",
			rc);
		exit(-1);
	}

	MQTTClient_subscribe(client, TOPIC, QOS);
	MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
	printf("Servidor MQTT conectado y suscrito al topico %s\n", TOPIC);
	while (1)
	{
		MQTTClient_receive(client, 100);
	}

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}
