typedef int (*callback)(unsigned char, unsigned char, unsigned short, unsigned int);

int comms_open();
int comms_connect();
int comms_close();
void comms_send(char, char, unsigned short, unsigned int);
void comms_read();
void comms_subscribe_to(unsigned char, callback);
void comms_subscribe_from(unsigned char, callback);
void test();