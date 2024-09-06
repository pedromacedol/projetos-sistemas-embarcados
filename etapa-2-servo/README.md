## ▶️ Executar projeto

1. Clone o repositório para sua máquina local:

   ```bash
   git clone https://github.com/pedromacedol/projetos-sistemas-embarcados.git
   ```

2. Navegue até o diretório da etapa 2 projeto:

   ```bash
   cd projetos-sistemas-embarcados/etapa-2-servo
   ```

3. Abra o ESP-IDF

4. Selecione: **Configure ESP-IDF Extension**

5. Selecione: **Use Existing Setup**

6. Selecione: **Advanced / Add .vscode subdirectory files**

7. Selecione: **Build**

8. Feche o ESP-IDF

9. Verifique o status do build

## 📁 Estrutura do Projeto

- `components/`: Diretório para componentes customizados ou de terceiros.

  - `imu_tools/`: Componente para ferramentas IMU.
    - `includes/`: Arquivos de cabeçalho para o componente de ferramentas IMU.
      - `imu_tools.h`: Arquivo de cabeçalho para as funções de ferramentas IMU.
    - `imu_tools.c`: Arquivo fonte para as funções de ferramentas IMU.
    - `CMakeLists.txt`: Arquivo CMake específico para o componente de ferramentas IMU.
  - `sensor_imu/`: Componente para sensor IMU.
    - `includes/`: Arquivos de cabeçalho para o componente de sensor IMU.
      - `sensor_imu.h`: Arquivo de cabeçalho para as funções de sensor IMU.
    - `sensor_imu.c`: Arquivo fonte para as funções de sensor IMU.
    - `CMakeLists.txt`: Arquivo CMake específico para o componente de sensor IMU.

- `documentation/`: Diretório contendo a documentação do projeto.

  - `documentation.pdf`: Documentação para firmware e bibliotecas.
  - `state_machine.png`: Diagrama de máquina de estados para o firmware.
  - `circuit_diagram.jpg`: Diagrama de circuito para o firmware.

- `main/`: Código fonte principal do projeto.

  - `CMakeLists.txt`: Arquivo CMake para a aplicação principal.
  - `main.c`: Arquivo fonte da aplicação principal.

- `state_machine/`: Pasta contendo as máquinas de estado do firmware.

  - `imu_tools/`: Máquina de estado de imu_tools.
  - `sensor_tools/`: Máquina de estado de sensor_tools.

- `CMakeLists.txt`: Arquivo CMake principal para todo o projeto.

- `sdkconfig`: Arquivo de configuração (gerado/gerenciado pelo "make menuconfig").
