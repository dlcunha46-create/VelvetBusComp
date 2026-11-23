VELVETBUS COMP — PROJECTO VST3 PARA WINDOWS 64-BIT
=================================================

Este ZIP contém o projecto-fonte completo do plugin VelvetBus Comp.
Não contém ainda uma DLL/VST3 compilada. O ficheiro VST3 é gerado no Windows
através dos passos abaixo.

REQUISITOS
----------
- Windows 10/11 de 64 bits
- Visual Studio 2022 (workload "Desktop development with C++")
- CMake 3.22 ou superior
- Ligação à Internet na primeira compilação (o CMake descarrega JUCE 8.0.6)

COMPILAR
--------
1. Extrai a pasta VelvetBusComp_VST3_Project.
2. Abre "x64 Native Tools Command Prompt for VS 2022".
3. Entra na pasta extraída, por exemplo:
   cd C:\...\VelvetBusComp_VST3_Project
4. Executa:
   cmake -B build -S . -A x64
5. Depois:
   cmake --build build --config Release

INSTALAÇÃO
----------
O projecto está configurado para copiar o VST3 após a compilação.
Se não for copiado automaticamente, procura:
   build\VelvetBusComp_artefacts\Release\VST3\VelvetBus Comp.vst3

Copia "VelvetBus Comp.vst3" para:
   C:\Program Files\Common Files\VST3

No Reaper: Options > Preferences > Plug-ins > VST > Re-scan.

CONTROLOS
---------
INPUT       Ganho antes do compressor (-24 a +24 dB)
ATTACK      Tempo de ataque (0,1 a 100 ms)
RELEASE     Tempo de recuperação (10 a 1000 ms)
OUTPUT      Ganho depois do compressor (-24 a +24 dB)
RATIO       Relação de compressão (1:1 a 10:1)
ANALOG COLOR Mistura das saturações Tube / Transformer / Console (0 a 100%)

NOTAS DE ÁUDIO
--------------
- Compressor de uso geral com detector ligado entre os canais.
- Threshold interno fixo em -18 dBFS; controla a intensidade com INPUT e RATIO.
- As três colorações são processamentos digitais inspirados em características
  gerais de saturação; não são cópias nem emulações oficiais de circuitos SSL,
  válvulas ou transformadores específicos.
- Esta é uma versão inicial. Recomenda-se testar com ganho moderado e comparar
  o nível de saída com bypass para evitar confundir volume com melhoria sonora.

LICENÇAS
--------
JUCE é obtido do repositório oficial durante a configuração e está sujeito
à licença aplicável do JUCE. Consulta https://juce.com/legal/ antes de distribuir
ou comercializar builds do plugin.
