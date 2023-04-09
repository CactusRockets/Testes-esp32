# Testes

## Primeiro teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
#define spreadingFactor 12
#define signalBandwidth 7.8E3
#define CRC 1
#define codingRateDenominator 8
#define txPower 2
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Em poucos metros (~50m) os pacotes não estavam sendo mais recebidos.

## Segundo teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
#define spreadingFactor 12
#define signalBandwidth 125E3
#define CRC 1
#define codingRateDenominator 8
#define txPower 2
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Essa foi a melhor configuração até o momento.

Em cerca de 150m, a maioria dos pacotes estavam sendo recebidos (mesmo com um dos módulos em movimento). A velocidade com que os pacotes estavam sendo transmitidos estava um pouco lenta no módulo transmissor.