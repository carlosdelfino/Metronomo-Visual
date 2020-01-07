TODO Metrônomo Severiano
========================

## 11 - [STARTED] iniciar versão 1.5

## 10 - [DONE] liberar versão 1.4

## 9 - [STARTED] Remover variáveis dúblicas em relação ao Struct de memory

Algumas variáveis são conseitualmente duplicadas, já que se existem no struct memory, não precisa existir externamente no algortimo mesmo durante a execução.
[STARTED] 07/01/2020

## 1 - [DONE] Exibir tipo de sensor ao iniciar

[DONE]

## 2 - [DONE] Corrigir nome da função readMemory

[DONE] 07/01/2019


## 3 - [DONE] Memorizar BPM

Quando no potenciometro, usar left e right, up e down para memorizar quatro possíveis BPM, memoria 1 a 4.
Quando nos botões, usar left e right para memorizar duas possíveis BPM, memoria 1 e 2;
Quando apertado pra memorizar deve se segurar por 5 segundos
[DONE] 07/01/2019


## 4 - [DONE] Corrigir seleção de sensor BPM

Verificar a função checkButtonSetup, ela não está mudando o sensor de ajuste do BPM.
[DONE] 07/01/2019

## 5 - Resetar para configurações e memoria padrão

Ao ligar o Metronomo com o botão segurando Left, deve resetar a memoria para usar a configuração padrão;

## 6 - Limite de escrita na EEPROM

Implementar contagem de gravações na EEPROM, limitando a 100.000 gravações.
Implementar variável que aponta o endereço onde está gravado a  última gravação valida.
Atentar para o limite da memória com a função EEPROM.length()

## 7 - Melhor gerência de tempo da inicialização

A inicialização usa em demasia a fução delay, e deve ser adotado algum mecanismo para que os tempos sejam interajustados de forma que o aumento de um reduza outro que seja para auxiliar na qualidade da exibição, assim se gastar mais tmepo para ler a eeprom, enquanto isso o display pode já ir contantto a inicialização com caracter #, por exemplo uma porcentagem do tempo de leitura.

## 8 - Testar LEDs durante o setup quando testa o LCD.

Fazer uma função que faça os leds piscarem sequêncialmente para testes dos LEDs.

## Versão 1.6 - BIP audivel

Adicionar um BIP audivel ativado via setup, segurando o botão Righ durante a inicialização deve ativar ou desativar o BIP.
Para evitar acidentes de acionar o BIP durante um evento, este só pode ser ativado quando se liga/reseta o aparelho, evitando acidentes.
 
## Versão 1.7 - Circuito de teste liberado

[PREVISAO] 11/01/2020

## Versão 2 - PWM nos LEDs

Na versão 2 deve ser adicionado PWM aos LEDs para controlar luminosidade da fita.
precionando 3 vezes o botão select, entra no modo ajustar brilho, então este é feito no potenciometro ou nos botões conforme seleção dos sensor bpm no setup.

[PREVISAO] 16/01/2020

## Versão 2.* - prototipo final liberado

[PREVISAO] 20/01/2020

## Versão 3 - Display LCD maior

Na versão 3 estudar o uso de um display maior 

## Versão 4 - Display TFT

Adotar um display Touch Screen com teclado alfanúmerico com lista de rítimos nomeados