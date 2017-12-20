#define p1 8
#define p2 9
int svet = 0;
int reg = 0;
int shag = 0;
void setup() {
  pinMode(p1, OUTPUT);  //Управление мотором pin 1
  digitalWrite(p1, HIGH);
  pinMode(p2, OUTPUT);  //Управление мотором pin 2
  digitalWrite(p2, HIGH);
}
void loop() {
  reg = analogRead(0);  //Получение положения ручного регулятора
  svet = analogRead(7);  //Получение данных датчика света

  if (shag == 0 && svet > reg) { //Если шаг 0 и яркость выше положения регулятора и дверь была закрыта 8 часов
    digitalWrite(p2, HIGH);
    digitalWrite(p1, LOW);  //Открывать дверь
    shag = 1;  //Переход к следующему шагу
    delay(1000);  //Ждать 1 сек
  }
  if (shag == 1) {  //Проверка открытия двери
    digitalWrite(p2, HIGH);
    digitalWrite(p1, LOW);  //Открывать дверь
    if (digitalRead(6) == 1) {  //Если дверь открылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Выключить мотор
      shag = 2;  //Переход к шагу 2
      delay(2000);
    }
  }
  if (shag == 2 && svet < reg) {  //Когда яркость меньше положения регулятора
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);  //Закрывать дверь
    shag = 3;  //Переход к шагу 3
    delay(1000);
  }
  if (shag == 3) {  //Проверка закрытия двери
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);  //Закрывать дверь
    if (digitalRead(6) == 1) {  //Если успешно дверь закрылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Выключить мотор
      shag = 0;  //Переход к шагу 0
      delay(2000);
    }
  }
}
