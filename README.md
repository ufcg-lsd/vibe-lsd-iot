# Vibe LSD

Controlador de mandalas de papel e palitos de picolé via wi-fi.

## Desenvolvimento

Este controlador foi desenvolvido com uma placa NodeMCU ESP8266 (que possui wi-fi) e um motor de passo 28BYJ-48 com controlador (driver ULN2003).
Utilizando a Arduino IDE, é necessária a adição do [gerenciador da placa ESP8266](http://esp8266.github.io/Arduino/versions/2.2.0/doc/installing.html) e da biblioteca para controle do motor [CustomStepper](https://playground.arduino.cc/Main/CustomStepper)

## Rotas
```
GET http://<device_ip>/ # Retorna o status atual da mandala
GET http://<device_ip>/go?step=x # Posiciona a mandala no ponto x
```

