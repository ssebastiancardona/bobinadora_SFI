struct servoMoveMessage
{
    int servoNum;
};

struct servoMoveMessage message;
int valorEntrada;
void recieveStructure(byte *structurePointer, int structureLength)
{
    if (Serial.available() < sizeof(message))
        return;
    Serial.readBytes(structurePointer, structureLength);
    Serial.println(valorEntrada);
}

void setup()
{
    Serial.begin(9600);
    recieveStructure((byte *)&message, sizeof(message));
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop()
{
    valorEntrada = message.servoNum;
    if (Serial.available())
    {
        recieveStructure((byte *)&message, sizeof(message));
    }
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(valorEntrada);             // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(valorEntrada);             // wait for a second
}
