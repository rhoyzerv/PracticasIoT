#define PREAMB 170
#define SENSOR_FAIL 254
#define SIZE_FAIL 0
#define CRC_FAIL 255

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
}connect_frame;

typedef struct{
    uint8_t header;
    uint8_t length;
    uint8_t connack_flags;
    uint8_t return_code;
}connack_frame;

typedef struct{
    uint8_t header;
    uint8_t length;
}pingreq_frame;

typedef struct{
    uint8_t header;
    uint8_t length;
}pingresp_frame;
