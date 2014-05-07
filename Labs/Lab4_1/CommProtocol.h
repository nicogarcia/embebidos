#ifndef COMMPROTOCOL_H_
#define COMMPROTOCOL_H_

/************************************************************************
Message Format:
	S [ M M 1 1 2 2 3 3 4 4 ] E

	- Start and End token
	- 2 bytes for each temperature:
		1 => Actual
		2 => Maxima
		3 => Minima
		4 => Promedio
	- 2 bytes for mode (FIXME: 1 is enough but with 2 the msg is multiple of int, useless?)

************************************************************************/

// Message Struct to be used by user
typedef struct {
    int mode;
    double temp_actual;
    double temp_maxima;
    double temp_minima;
    double temp_promedio;
} TempMessage;

class CommProtocol_ {
public:
    TempMessage ReadMessage();

    TempMessage ParseMessage();

    void SendMessage(TempMessage message);

    // Modes values (from 5 available)
    static const int MODO_TEMP_ACTUAL = 30;
    static const int MODO_TEMP_MAXIMA = 40;
    static const int MODO_TEMP_MINIMA = 50;
    static const int MODO_TEMP_PROMEDIO = 60;

private:
    // Message length without tokens
    static const int MSG_LENGTH = 10;

    // Start and end tokens
    static const char START_TOKEN = 2;
    static const char END_TOKEN = 4;

    // Number padding to map, for ex. 0 => 100, 1 => 101
    // This is to avoid getting false numbers, like 0
    static const char NUMBER_PADDING = 100;

    // Temporary arrays to read and write message
    byte reading_msg[MSG_LENGTH];
    byte writing_msg[MSG_LENGTH];
};
extern CommProtocol_ CommProtocol;

#endif /* COMMPROTOCOL_H_ */