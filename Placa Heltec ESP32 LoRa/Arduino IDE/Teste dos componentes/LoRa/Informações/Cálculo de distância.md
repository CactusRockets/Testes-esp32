# Cálculo de distância para rádio

## Informações importantes

- Frequência
- Distância esperada
- Potência do módulo
- Ganho da 1ª antena
- Ganho da 2ª antena
- Sensibilidade do rádio (de acordo com a taxa de transferência por segundo)

<aside>
USE SEMPRE UMA MARGEM DE SEGURANÇA
</aside>

É bom utilizarmos uma margem de erro. Por exemplo, se o módulo tem sensibilidade de -72dB, com a margem de erro, o sinal deve chegar no receptor com pelo menos -62dB.

## Free space loss (perda do sinal através do ar)

![Untitled](Ca%CC%81lculo%20de%20dista%CC%82ncia/Untitled%200.png)

Onde:

- D ⇒ distância
- F ⇒ frequência
- C ⇒ Velocidade da luz

![Untitled](Ca%CC%81lculo%20de%20dista%CC%82ncia/Untitled%201.png)

[Free Space Path Loss Calculator](https://www.pasternack.com/t-calculator-fspl.aspx)

Calculadora de FSPL

## Cálculo

> Precisamos da distância e da frequência para o cálculo da FSPL
> 

> "Potência" do rádio em dBm + Ganho da 1ª antena (em dBi) - FSPL (em dB) + Ganho da 2ª antena (em dBi) = X ("Potência" que chegará no módulo receptor)
> 

O X deve ser comparado com o valor Y da sensibilidade do rádio (com a margem de erro). Ou seja, em nosso exemplo, devemos considerar:

- X ≥ Y ⇒ Ótimo
- X ≤ Y ⇒ Ruim

<aside>
⚠️ Não estamos considerando a perda de dB nos cabos de ligação do módulo com a antena e nem possíveis interferências.

</aside>

## Cálculo (exemplo do LoRa)

- Frequência: 433 MHz
- Distância: 500m
- Potência do rádio (LoRa): 14dBm
- Ganho da 1ª antena: 2dBi
- Ganho da 2ª antena: 2dBi
- Sensibilidade do rádio (LoRa): -121dBm (valor máximo possível)
- Margem de segurança: 10dB

### Passo-a-passo

1. Cálculo do FSPL (não levar em consideração ganho de antenas)
    
    FSPL = 79.15 dB
    
2. Fazer o cálculo total
    
    X = 14dBm + 2bBi - 79.15dB + 2dBi
    
    X = -61.15 ("Potência" que chegará no módulo receptor)
    
3. Comparar resultado com Y (sensibilidade)
    
    X > -121dBm + 10dB (margem de segurança) = -111dB
    

Isto é uma notícia boa, pois o sinal vai chegar bom (mesmo com a margem de segurança).

## Cálculo (exemplo do NRF24L01)

- Frequência: 2.4GHz
- Distância: 500m
- Potência do rádio (NRF24L01): 0dBm (potência mais alta)
- Ganho da 1ª antena: 2dBi
- Ganho da 2ª antena: 2dBi
- Sensibilidade do rádio: em 250Kbps (menor data rate possível) é de -104dBm
- Margem de segurança: 10dB

### Passo-a-passo

1. Cálculo do FSPL (não levar em consideração ganho de antenas)
    
    FSPL = 94.02dB
    
2. Fazer o cálculo total
    
    X = -90.02dB ("Potência" que chegará no módulo receptor)
    
3. Comparar resultado com Y (sensibilidade)
    
    X > (-104dBm + 10dB (margem de segurança)) (-94dB)
    

Isto é uma notícia boa, pois o sinal vai chegar bom (mesmo com a margem de segurança).

1. Cálculo do FSPL (não levar em consideração ganho de antenas)
    
    FSPL = 94.02dB
    
2. Fazer o cálculo total
    
    X = -90.02dB
    
3. Comparar resultado com Y (sensibilidade)
    
    X > -104dBm + 10dB (margem de segurança) = -94dB
    

Isto é uma notícia boa, pois o sinal vai chegar bom (mesmo com a margem de segurança).

## Ajuda

[NRF24L01 Estimated Distance](https://docs.google.com/spreadsheets/d/1lBhk5KKM7LpfV_I2WQb5lGSNA2zstBKjaFUvTXwOpKY/edit#gid=0)