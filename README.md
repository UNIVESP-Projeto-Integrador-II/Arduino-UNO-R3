# Projeto de Automação de Aquário com Arduino e ESP8266

Este projeto tem como objetivo automatizar o controle de um aquário usando um **Arduino** e o módulo **ESP8266** para monitorar e controlar a temperatura, luminosidade, alimentação dos peixes e outros parâmetros através de um servidor web. O sistema inclui sensores de temperatura e luminosidade, além de um servo motor para alimentar os peixes e controle de iluminação.

## Funcionalidades

- **Sensores de Temperatura e Luminosidade**: Monitora a temperatura da água e a intensidade da luz no aquário.
- **Controle de Alimentação**: Servo motor para alimentar os peixes em intervalos regulares.
- **Controle de Iluminação**: Ajuste da intensidade da luz do aquário com base nos parâmetros de luminosidade.
- **Envio de Dados para o Servidor**: Envia os dados dos sensores para um servidor web via **Wi-Fi** utilizando o módulo **ESP8266**.
- **Interface LCD**: Exibe informações como temperatura, intensidade da luz e estado do alimentador no display LCD.

## Componentes Necessários

- **Arduino Uno** (ou outro modelo compatível)
- **ESP8266** (Módulo Wi-Fi)
- **LCD 16x2** com interface I2C
- **Servo Motor**
- **Sensores de Temperatura** (LM35 ou similar)
- **Sensor de Luminosidade** (LDR ou similar)
- **Resistores e fios**

## Descrição do Código

O código implementa a lógica para controle e monitoramento do aquário. As principais funções do código são:

1. **Leitura de Sensores**: 
   - A temperatura da água é lida através de um sensor de temperatura analógico.
   - A luminosidade é lida através de um sensor LDR (Light Dependent Resistor).
   
2. **Controle do Servo Motor**: 
   - O servo motor é acionado para alimentar os peixes a cada intervalo de tempo definido.
   
3. **Controle de Iluminação**: 
   - A intensidade da luz no aquário é ajustada de acordo com a leitura do sensor LDR.

4. **Envio de Dados via Wi-Fi**:
   - Usando o módulo ESP8266, os dados de temperatura, luminosidade e outras informações são enviados a um servidor web via uma requisição HTTP POST.

5. **Interface LCD**: 
   - O LCD exibe informações como a temperatura da água, intensidade da luz e o tempo do alimentador.

## Configuração do Wi-Fi

- O código utiliza o módulo **ESP8266** para se conectar à rede Wi-Fi.
- As informações de rede (SSID e senha) precisam ser inseridas no código:
  
