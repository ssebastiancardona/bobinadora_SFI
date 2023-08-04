struct servoMoveMessage
{
    int servoNum;
};

struct servoMoveMessage message;

void sendStructure(byte *structurePointer, int structureLength)
{
    Serial.write(structurePointer, structureLength);
}

void setup()
{
    Serial.begin(9600);

    sendStructure((byte *)&message, sizeof(message));
}

void loop()
{
    int valor = 3000;
    message.servoNum = valor;
    sendStructure((byte *)&message, sizeof(message));
    delay(1000);
}