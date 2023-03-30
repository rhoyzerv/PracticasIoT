#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADDRESS "tcp://localhost:1883"
#define CLIENTID "cliente1"
#define TOPIC "test"
#define QOS 1
#define TIMEOUT 10000 L
int main(int argc, char *argv[])
{
	MQTTClient client;
	MQTTClient_connectOptions conn_opts =
		MQTTClient_connectOptions_initializer;
	MQTTClient_message msg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	int rc;
	char payload[100];
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

	mqtt_connect_frame_t connect_frame;
	connect_frame.type = MQTT_CONNECT;
	connect_frame.flags = 0;
	connect_frame.length = 12;
	connect_frame.protocol_name_length = 6;
	memcpy(connect_frame.protocol_name, "MQTT", 6);
	connect_frame.protocol_version = 0x04;
	connect_frame.connect_flags = 0x02;
	connect_frame.keep_alive = htons(30);
	MQTTClient_publish(client, TOPIC, sizeof(mqtt_connect_frame_t), &connect_frame, QOS, 0);
	printf("Frame MQTT CONNECT enviado al broker MQTT\n");
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return rc;
}
