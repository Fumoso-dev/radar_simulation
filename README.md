# Radar Simulation

Simulazione di un radar sviluppata con Qt6 e C++.

## Funzionalità

- Interfaccia grafica con `mainwindow.ui`
- Esecuzione di suoni (es. `bip.wav`)
- Sistema modulare con `SoundWorker` separato
- Gestione delle risorse tramite `resources.qrc`

## Requisiti

- Qt 6.5+ (testato con `Desktop_Qt_6_9_1_MinGW_64_bit`)
- CMake (versione 3.15+)
- Un compilatore C++ (MinGW o equivalente)

## Come compilare

### Opzione 1: Qt Creator (consigliato)

1. Apri il progetto (`CMakeLists.txt`) con Qt Creator
2. Compila e avvia con il kit `Qt 6.9.1 MinGW 64-bit`

### Opzione 2: Da terminale

```bash
mkdir build
cd build
cmake ..
cmake --build .
