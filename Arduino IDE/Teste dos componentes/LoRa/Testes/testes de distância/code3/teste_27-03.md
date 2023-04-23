# Testes

## Primeiro teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
0E3#define spreadingFactor 12
#define signalBandwidth 125E3
#define CRC 1
#define codingRateDenominator 8
#define txPower 8

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 200m.

Perda de pacotes mínima.

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
#define txPower 14

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 200m.

Perda de pacotes mínima.

## Terceiro teste

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
#define txPower 14

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 300m.

No pacote 193 a pessoa começou a correr voltando.

Grande perda de pacotes.

## Quarto teste

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
#define codingRateDenominator 4
#define txPower 14

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 300m.

## Quinto teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
#define spreadingFactor 12
#define signalBandwidth 250E3
#define CRC 1
#define codingRateDenominator 4
#define txPower 14

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 300m.

133 foi o número do pacote enviado nos 300m de distância.

Foi um pouco melhor do que a configuração anterior.

## Sexto teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
#define spreadingFactor 12
#define signalBandwidth 250E3
#define CRC 1
#define codingRateDenominator 4
#define txPower 14

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 300m.

Tivemos o mínimo de perda de pacotes nessa configuração.

## Sétimo teste

Parâmetros usados.

```cpp
#define BAND 433E6

#define SS 18
#define RESET 14
#define DIO0 26

#define frequency_SPI 10E6
#define spreadingFactor 12
#define signalBandwidth 250E3
#define CRC 1
#define codingRateDenominator 4
#define txPower 17

Pino da antena: PA_OUTPUT_PA_BOOST_PIN
```

O teste foi realizado em uma área com interferências da rede elétrica.
Foi utilizado a placa Heltec LoRa Wifi 32 (V2) com a antena.

### Conclusões

Distância percorrida: 300m.

Tivemos grande perda de pacotes na volta dos 300m (em cerca de 200m a 100m).