# Heltec LoRa basic library API

## Setup

### Begin

Initialize the library with the specified frequency.

```arduino
LoRa.begin(frequency);
```
 * `frequency` - frequency in Hz (`433E6`, `866E6`, `915E6`)

Returns `1` on success, `0` on failure.

### Set pins

Override the default `NSS`, `NRESET`, and `DIO0` pins used by the library. **Must** be called before `LoRa.begin()`.

```arduino
LoRa.setPins(ss, reset, dio0);
```
 * `ss` - new slave select pin to use, defaults to `18`
 * `reset` - new reset pin to use, defaults to `14`
 * `dio0` - new DIO0 pin to use, defaults to `26`

This call is optional and only needs to be used if you need to change the default pins used.

### Set SPI Frequency

Override the default SPI frequency of 10 MHz used by the library. **Must** be called before `LoRa.begin()`.

```arduino
LoRa.setSPIFrequency(frequency);
```
 * `frequency` - new SPI frequency to use, defaults to `10E6`

This call is optional and only needs to be used if you need to change the default SPI frequency used. Some logic level converters cannot support high speeds such as 10 MHz, so a lower SPI frequency can be selected with `LoRa.setSPIFrequency(frequency)`.

### End

Stop the library

```arduino
LoRa.end()
```

## Sending data

### Begin packet

Start the sequence of sending a packet.

```arduino
LoRa.beginPacket();

LoRa.beginPacket(implicitHeader);
```

 * `implicitHeader` - (optional) `true` enables implicit header mode, `false` enables explicit header mode (default)

Returns `1` on success, `0` on failure.

### Writing

Write data to the packet. Each packet can contain up to 255 bytes.

```arduino
LoRa.write(byte);

LoRa.write(buffer, length);
```
* `byte` - single byte to write to packet

or

* `buffer` - data to write to packet
* `length` - size of data to write

Returns the number of bytes written.

**Note:** Other Arduino `Print` API's can also be used to write data into the packet

### End packet

End the sequence of sending a packet.

```arduino
LoRa.endPacket()
```

Returns `1` on success, `0` on failure.

## Receiving data

### Parsing packet

Check if a packet has been received.

```arduino
int packetSize = LoRa.parsePacket();

int packetSize = LoRa.parsePacket(size);
```

 * `size` - (optional) if `> 0` implicit header mode is enabled with the expected a packet of `size` bytes, default mode is explicit header mode


Returns the packet size in bytes or `0` if no packet was received.

### Continuous receive mode

#### Register callback

Register a callback function for when a packet is received.

```arduino
LoRa.onReceive(onReceive);

void onReceive(int packetSize) {
 // ...
}
```

 * `onReceive` - function to call when a packet is received.

#### Receive mode

Puts the radio in continuous receive mode.

```arduino
LoRa.receive();

LoRa.receive(int size);
```

 * `size` - (optional) if `> 0` implicit header mode is enabled with the expected a packet of `size` bytes, default mode is explicit header mode

The `onReceive` callback will be called when a packet is received.

### Packet RSSI

```arduino
int rssi = LoRa.packetRssi();
```

Returns the RSSI of the received packet.

### Packet SNR

```arduino
float snr = LoRa.packetSnr();
```

Returns the estimated SNR of the received packet in dB.

### Available

```arduino
int availableBytes = LoRa.available()
```

Returns number of bytes available for reading.

### Peeking

Peek at the next byte in the packet.

```arduino
byte b = LoRa.peek();
```

Returns the next byte in the packet or `-1` if no bytes are available.

### Reading

Read the next byte from the packet.

```arduino
byte b = LoRa.read();
```

Returns the next byte in the packet or `-1` if no bytes are available.

**Note:** Other Arduino [`Stream` API's](https://www.arduino.cc/en/Reference/Stream) can also be used to read data from the packet

## Other radio modes

### Idle mode

Put the radio in idle (standby) mode.

```arduino
LoRa.idle();
```

### Sleep mode

Put the radio in sleep mode.

```arduino
LoRa.sleep();
```

## Radio parameters

### TX Power

Change the TX power of the radio.

LoRa technology uses a variable transmission power to control the range and reliability of the transmission. Higher transmission power provides greater range and resistance to interference but can consume more power and may cause regulatory compliance issues. In a mobile environment, it is recommended to use the lowest transmission power that provides reliable communication to conserve power and avoid regulatory issues.

[A tecnologia LoRa usa uma potência de transmissão variável para controlar o alcance e a confiabilidade da transmissão. A potência de transmissão mais alta fornece maior alcance e resistência à interferência, mas pode consumir mais energia e causar problemas de conformidade regulatória. Em um ambiente móvel, é recomendável usar a potência de transmissão mais baixa que forneça comunicação confiável para economizar energia e evitar problemas regulatórios.]

```arduino
LoRa.setTxPower(txPower);

LoRa.setTxPower(txPower, outputPin);
```
 * `txPower` - TX power in dB, defaults to `14`
 * `outputPin` - (optional) PA output pin, supported values are `PA_OUTPUT_RFO_PIN` and `PA_OUTPUT_PA_BOOST_PIN`, defaults to `PA_OUTPUT_PA_BOOST_PIN`.

Supported values are between `2` and `17` for `PA_OUTPUT_PA_BOOST_PIN`, `0` and `14` for `PA_OUTPUT_RFO_PIN`.

Most modules have the PA output pin connected to PA BOOST.

### Frequency

Change the frequency of the radio.

```arduino
LoRa.setFrequency(frequency);
```
 * `frequency` - frequency in Hz (`433E6`, `866E6`, `915E6`)

### Spreading Factor

Change the spreading factor of the radio.

LoRa technology uses a spreading factor to control the data rate and range of the transmission. Higher spreading factors provide better range and resistance to interference but result in lower data rates. Lower spreading factors provide higher data rates but with reduced range and increased susceptibility to interference. In a mobile environment, it is recommended to use higher spreading factors to improve reliability.

[A tecnologia LoRa usa um fator de espalhamento para controlar a taxa de dados e o alcance da transmissão. Fatores de espalhamento mais altos fornecem melhor alcance e resistência à interferência, mas resultam em taxas de dados mais baixas. Fatores de espalhamento mais baixos fornecem taxas de dados mais altas, mas com alcance reduzido e maior suscetibilidade a interferências. Em um ambiente móvel, é recomendável usar fatores de espalhamento mais altos para melhorar a confiabilidade.]

Spreading Factor (SF - Fator de Espalhamento): O fator de espalhamento é um parâmetro atrelado a taxa do chirp e a taxa de informação da banda de base. (Não é necessário dominarmos esses dois conceitos por hora), mas é importante saber que a variação do SF de seu intervalo (7 a 12) interfere diretamente na sensibilidade de receber sinais e na taxa de transmissão de dados.

![Untitled](Documentation/Untitled%200.png)

Nesse caso, temos que um Spreading Factor maior, significa uma maior sensibilidade na recepção de sinais, e ao mesmo tempo, uma diminuição da taxa de transmissão de dados. Para aumentar a taxa de transmissão de dados, teremos que, necessariamente, diminuir o SF, e com ele, a sensibilidade de recepção de dados. Uma maneira de contornar essa situação, mantendo uma alta transmissão de dados sem a perda dos mesmos é aumentar a potência do sinal transmitido.

```arduino
LoRa.setSpreadingFactor(spreadingFactor);
```
 * `spreadingFactor` - spreading factor, defaults to `7`

Supported values are between `6` and `12`. If a spreading factor of `6` is set, implicit header mode must be used to transmit and receive packets.

### Signal Bandwidth

Change the signal bandwidth of the radio.

LoRa technology uses a variable bandwidth to support different data rates. The bandwidth determines the channel width used for transmission and affects the sensitivity and interference resistance of the transmission. In a mobile environment, it is recommended to use a narrower bandwidth to reduce the effects of Doppler shift and other frequency variations caused by the transmitter's movement.

[A tecnologia LoRa usa uma largura de banda variável para suportar diferentes taxas de dados. A largura de banda determina a largura do canal usado para transmissão e afeta a sensibilidade e a resistência à interferência da transmissão. Em um ambiente móvel, recomenda-se usar uma largura de banda mais estreita para reduzir os efeitos do deslocamento Doppler e outras variações de frequência causadas pelo movimento do transmissor.]

De maneira simplificada, a largura de banda se refere a quantidade máxima de bits/s que a determinada transmissão suporta. Nesse caso, temos três valores normalmente comuns para o módulo LoRa, o mais utilizado de 125 KHz e as bandas de 240 KHz e 500 KHz.

```arduino
LoRa.setSignalBandwidth(signalBandwidth);
```

 * `signalBandwidth` - signal bandwidth in Hz, defaults to `125E3`.

Supported values are `7.8E3`, `10.4E3`, `15.6E3`, `20.8E3`, `31.25E3`, `41.7E3`, `62.5E3`, `125E3`, and `250E3`.

### Coding Rate

Change the coding rate of the radio.

LoRa technology uses a coding rate to provide forward error correction (FEC) and improve the reliability of the transmission. Higher coding rates provide better error correction but result in lower data rates. In a mobile environment, it is recommended to use higher coding rates to improve reliability.

[A tecnologia LoRa usa uma taxa de codificação para fornecer correção de erro antecipada (FEC) e melhorar a confiabilidade da transmissão. Taxas de codificação mais altas fornecem melhor correção de erros, mas resultam em taxas de dados mais baixas. Em um ambiente móvel, é recomendável usar taxas de codificação mais altas para melhorar a confiabilidade.]

O Coding Rate avalia o quanto do sinal transmitido se torna útil, os coding rate normalmente utilizados em módulos LoRa são: 4/5, 4/6, 4/7, 4/8.

```arduino
LoRa.setCodingRate4(codingRateDenominator);
```

 * `codingRateDenominator` - denominator of the coding rate, defaults to `5`

Supported values are between `5` and `8`, these correspond to coding rates of `4/5` and `4/8`. The coding rate numerator is fixed at `4`.

### Preamble Length

Change the preamble length of the radio.

```arduino
LoRa.setPreambleLength(preambleLength);
```

 * `preambleLength` - preamble length in symbols, defaults to `8`

Supported values are between `6` and `65535`.

### Sync Word

Change the sync word of the radio.

```arduino
LoRa.setSyncWord(syncWord);
```

 * `syncWord` - byte value to use as the sync word, defaults to `0x34`

### CRC

Enable or disable CRC usage, by default a CRC is not used.

```arduino
LoRa.enableCrc();

LoRa.disableCrc();
```

### iqInverted

Enable or disable iqInverted for TX/RX

```arduino
LoRa.disableInvertIQ();
LoRa.enableInvertIQ();
LoRa.enableTxInvertIQ();
LoRa.enableRxInvertIQ();

```

## Other functions

### Random

Generate a random byte, based on the Wideband RSSI measurement.

```
byte b = LoRa.random();
```

Returns random byte.
