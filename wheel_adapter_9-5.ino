//Wheel keys adapter for SAAB 9-5 98-05

#include <mcp_can.h>
#include <SPI.h>

#define DPOD 4
#define CAN 2

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(9);                        // пин, к которому подключена кан-плата

byte VOLUP = 43;                        // блок для описания
byte VOLDN = 61;                        // значений
byte NXTTR = 29;                        // которые принимае
byte PRVTR = 41;                        // потенциометр
byte SRCBT = 3;                         // и выставляет
byte MUTBT = 9;                         // нужное сопротивление
byte NOKEY = 0;                         // 0 - кнопка не нажата

void setup()
{
    delay(100);
    CAN0.begin(CAN_47K619BPS, CAN);     // ставим скорость кан-шины 9-5 - 47 619 BPS
    pinMode(CAN, INPUT);                // пин с кан-платой - на чтение 
    pinMode(DPOD, OUTPUT);              // пин с потенциометром на отправку
    digitalWrite(DPOD, HIGH);           // при включении адаптера отключаем потенциометр
}

void loop()
{
    if(!digitalRead(CAN))
    {
        CAN0.readMsgBuf(&len, rxBuf);   // читаем кан-шину 
        rxId = CAN0.getCanId();         // ID сообщений
        if (rxId == 0x290)              // в этом ID у нас "гуляют" кнопки руля 
        {
            switch (rxBuf[2])           // на этой позиции у нас сидят сами клавиши
            {
                case 0x04:              // клавиша NXT на руле
                    keypress(MUTBT);    // присваиваем ей клавишу MUTE
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
                    
                case 0x10:              // клавиша Next Track на руле
                    keypress(NXTTR);    // присваиваем ей клавишу Next Track
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
                    
                case 0x08:              // клавиша Previous Track на руле
                    keypress(PRVTR);    // присваиваем ей клавишу Previous Track
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
                    
                case 0x20:              // клавиша SRC на руле
                    keypress(SRCBT);    // присваиваем ей клавишу SRC
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
                    
                case 0x40:              // клавиша Volume Up на руле
                    keypress(VOLUP);    // присваиваем ей клавишу Volume Up
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
                    
                case 0x80:              // клавиша Volume Down на руле
                    keypress(VOLDN);    // присваиваем ей клавишу Volume Down
                    delay(50);
                    keypress(NOKEY);    // "отпускаем" клавишу
                    break;
            }
        }
    }
}

void keypress(byte key)                 // отправка необходимого сопротивления в магнитолу
{
    digitalWrite(CAN, HIGH);            // отключаем прием из кан-шины
    digitalWrite(DPOD, LOW);            // включаем цифровой потенциометр
    SPI.transfer(0b00010001);           // отправляем первый байт в регистр конфигурации
    SPI.transfer(key);                  // устанавливаем сопротивление кнопки ("нажимаем" кнопку)
    digitalWrite(DPOD, HIGH);           // выключаем цифровой потенциометр
    digitalWrite(CAN, LOW);             // включаем прием из кан-шины
}
