#define p1 8
#define p2 9
int svet = 0;
int reg = 0;
int a = 0;
int shag = 0;
unsigned long t = 0;
unsigned long tl = 0;

void setup() {
  pinMode(p1, OUTPUT);  //Управление мотором pin 1
  digitalWrite(p1, HIGH);
  pinMode(p2, OUTPUT);  //Управление мотором pin 2
  digitalWrite(p2, HIGH);
}

void loop() {
  reg = analogRead(0);  //Получение положения ручного регулятора
  svet = analogRead(7);  //Получение данных датчика света

  if (shag == -1) {  //Аварийное состояние, остановка
    digitalWrite(p2, HIGH);  //Выключение двигателя
    digitalWrite(p1, HIGH);
  }

  if (shag == 0 && svet > reg) { //Если шаг 0 и яркость выше положения регулятора и дверь была закрыта 8 часов
    digitalWrite(p2, HIGH);
    digitalWrite(p1, LOW);  //Открывать дверь
    shag = 1;  //Переход к следующему шагу
    tl = millis();  //Запись текущего времени
    delay(1000);  //Ждать 1 сек
  }
  if (shag == 1) {  //Проверка открытия двери
    digitalWrite(p2, HIGH);
    digitalWrite(p1, LOW);  //Открывать дверь
    if (digitalRead(6) == 1) {  //Если дверь открылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Выключить мотор
      shag = 4;  //Переход к шагу 4
      a = 0;  //Обнуление счетчика повторений операции
      delay(2000);
    }
    if (millis() > tl + 8000) { //Если за 8 секунд дверь не открылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Выключить мотор
      a = a + 1;  //Увеличить счетчик повторения операции на 1
      delay(4000);
      digitalWrite(p2, LOW);  //Зарывать дверь
      shag = 2;  //Переход к шагу 2 (проверка закрытия двери после того как она не открылась)
      tl = millis();
    }
  }
  if (shag == 2 && digitalRead(6) == 1) {  //Если дверь успешно закрылась
    digitalWrite(p2, HIGH);  
    digitalWrite(p1, HIGH);  //Выключить мотор
    delay(2000);
    shag = 1;  //Повторно попытаться открыть дверь
    tl = millis();
  }
  if (shag == 2 && millis() > tl + 10000) {  //Если дверь не закрылась
    digitalWrite(p2, HIGH);
    digitalWrite(p1, HIGH);  //Выключить мотор
    a = a + 10;  //Ввод в аварийное состояние, зацикливание программы
  }


  if (shag == 4 && svet < reg) {  //Когда яркость меньше положения регулятора - 50 (защита от ложных срабатываний)
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);  //Закрывать дверь
    shag = 5;  //Переход к шагу 5
    tl = millis();
    delay(1000);
  }
  if (shag == 5) {  //Проверка закрытия двери
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);  //Закрывать дверь
    if (digitalRead(6) == 1) {  //Если успешно дверь закрылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Выключить мотор
      shag = 0;  //Переход к шагу 0
      a = 0;
      t = millis();
      delay(2000);
    }
    if (millis() > tl + 8000) {  //Если за 8 секунд дверь не закрылась
      digitalWrite(p2, HIGH);
      digitalWrite(p1, HIGH);  //Остановить мотор
      a = a + 1;  //Увеличить счетчик повторения операции на 1
      delay(4000);
      digitalWrite(p1, LOW);  //Попытатся открыть дверь
      shag = 6;  //Переход к шагу 6
      tl = millis();
    }
  }
  if (shag == 6 && digitalRead(6) == 1) {  //Если дверь открылась
    digitalWrite(p2, HIGH);
    digitalWrite(p1, HIGH);  //Остановить мотор
    delay(2000);
    shag = 5;  //Переход к шагу 5 (попытатся еще раз закрыть дверь)
    tl = millis();
  }
  if (shag == 6 && millis() > tl + 10000) {  //Если дверь не открылась
    digitalWrite(p2, HIGH);
    digitalWrite(p1, HIGH);  //Остановить мотор
    a = a + 10;  //Переход в аварийное состояние
  }
  if (a > 4) {  //Если дверь не открылась/закрылась 5 раз
    shag = -1;  //Зацикливание программы на шаге -1
    a = 0;
  }
}
