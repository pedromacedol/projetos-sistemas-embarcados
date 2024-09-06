
# Projetos de Sistemas Embarcados

Este repositório contém diversos projetos desenvolvidos como parte do curso de sistemas embarcados. Cada projeto foca em um subsistema ou funcionalidade específica de sistemas embarcados.

## Estrutura do Repositório

O repositório é dividido em múltiplas etapas, cada uma representando um projeto específico. Abaixo está a descrição de cada etapa:

### 1. [Etapa 1 - IMU](./etapa-1-imu/)

Este projeto implementa o controle e leitura de sensores IMU (Unidade de Medida Inercial), com foco na obtenção de dados de aceleração e giroscópio.

#### Funcionalidades
- Leitura de sensores IMU.
- Tratamento e filtragem de dados do acelerômetro e giroscópio.
- Implementação de funções de controle do IMU.

### 2. [Etapa 2 - Servo](./etapa-2-servo/)

Este projeto implementa o controle de um servo motor, incluindo a abstração de hardware e o desenvolvimento de ferramentas para o controle do servo.

#### Funcionalidades
- Controle de servo motor.
- Abstração de hardware para controle de posição
- Implementação de ferramentas de controle customizadas para servo.

## Requisitos

- **SDK:** Necessário instalar o SDK de desenvolvimento específico para sistemas embarcados utilizado neste projeto (como o ESP-IDF, por exemplo).
- **Compilador CMake:** Certifique-se de ter o CMake instalado para compilar e gerenciar o build.
- **Plataforma:** Projetos testados em sistemas Linux ou macOS. Adaptável para outros sistemas operacionais.

## Como Compilar

1. Clone o repositório:
   ```bash
   git clone https://github.com/seu-usuario/projetos-sistemas-embarcados.git
   cd projetos-sistemas-embarcados
   ```

2. Navegue para a etapa desejada (por exemplo, `etapa-1-imu`):
   ```bash
   cd etapa-1-imu
   ```

3. Siga os comandos do README.md do diretório.


## Contribuição

Sinta-se à vontade para contribuir com melhorias ou novas funcionalidades. Para isso, siga os seguintes passos:

1. Faça um fork do projeto.
2. Crie uma nova branch para suas modificações:
   ```bash
   git checkout -b minha-nova-funcionalidade
   ```
3. Faça suas modificações e adicione os commits.
4. Envie um pull request com suas alterações.

## Licença

Este projeto é licenciado sob a licença MIT - veja o arquivo [LICENSE](./LICENSE) para mais detalhes.
