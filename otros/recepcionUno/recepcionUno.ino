struct servoMoveMessage
{
    float servoNum;
};

struct servoMoveMessage message;
int valorEntrada;
bool activo;
void recieveStructure(byte *structurePointer, int structureLength)
{
    if (Serial.available() < sizeof(message))
        return;
    Serial.readBytes(structurePointer, structureLength);
    // Serial.println(valorEntrada);
}

void setup()
{
    Serial.begin(9600);
    // pinMode(LED_BUILTIN, OUTPUT);
    //  pinMode(13, OUTPUT);
    //  digitalWrite(13, LOW);
    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);
    pinMode(4, INPUT_PULLUP);
    recieveStructure((byte *)&message, sizeof(message));
}

void loop()
{
    valorEntrada = message.servoNum;
    int aceleracion = valorEntrada * 15;

    if (Serial.available())
    {
        recieveStructure((byte *)&message, sizeof(message));
    }

    while (digitalRead(4) == LOW)
    {
        while (digitalRead(4) == LOW && aceleracion > valorEntrada)
        {
            digitalWrite(11, HIGH);         // turn the LED on (HIGH is the voltage level)
            delayMicroseconds(aceleracion); // wait for a second
            digitalWrite(11, LOW);          // turn the LED off by making the voltage LOW
            delayMicroseconds(aceleracion); // wait for a second
            aceleracion--;
        }
        digitalWrite(11, HIGH);          // turn the LED on (HIGH is the voltage level)
        delayMicroseconds(valorEntrada); // wait for a second
        digitalWrite(11, LOW);           // turn the LED off by making the voltage LOW
        delayMicroseconds(valorEntrada); // wait for a second
    }
}
