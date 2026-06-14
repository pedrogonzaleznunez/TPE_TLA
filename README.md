# Flex-Bison-Compiler

A base compiler example, developed with Flex and Bison.

- [Requirements](#requirements)
- [Configuration](#configuration)
- [Commands](#commands)
- [CI/CD](#cicd)
- [Recommended Extensions](#recommended-extensions)

## Requirements

- [Docker v28.3.2](https://www.docker.com/)

## Configuration

Set the following environment variables to control and configure the behaviour of the application:


| Name                  | Default | Description                                                                                                                                                           |
| --------------------- | ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ENVIRONMENT`         | `Local` | The active environment name. The available environments are: `Local`, `Development` and `Production`.                                                                 |
| `LOG_IGNORED_LEXEMES` | `true`  | When `true`, logs all of the ignored lexemes found with Flex at `DEBUGGING` level. To remove those logs from the console output set it to `false`.                    |
| `LOGGING_LEVEL`       | `ALL`   | The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`. |


*Docker Compose* can read the variables from an `.env` file too (see `compose.yaml` file).

## Commands

### Start

Rises an ephemeral container, ready to start development:

```bash
docker compose run --rm compiler
```

### Build

Builds or rebuilds the entire compiler:

```bash
src/main/bash/build.sh
```

### Run

Compiles a program:

```bash
src/main/bash/run.sh <program>
```

where `<program>` is the path to the file that represents its entry-point.

### Test

Executes every available unit-test under `src/test/c` folder:

```bash
src/main/bash/test.sh
```

### Stop

Logout, destroy the ephemeral containers and shutdowns the cluster:

```bash
exit
docker compose down
```

### Docker


| Command                                 | Description                                             |
| --------------------------------------- | ------------------------------------------------------- |
| `docker builder prune --all`            | Removes all builds and complete build cache.            |
| `docker compose --progress=plain build` | Forces a build or rebuild of the images in the cluster. |
| `docker image prune`                    | Removes all of the dangling images from Docker.         |
| `docker network prune`                  | Removes unused networks from Docker.                    |
| `docker volume prune`                   | Removes unused volumes from Docker.                     |


## CI/CD

To trigger an automatic integration on every push or PR (*Pull Request*), you must activate *GitHub Actions* in the *Settings* tab. Use the following configuration:


| Key                                                        | Value                                               |
| ---------------------------------------------------------- | --------------------------------------------------- |
| `Actions permissions`                                      | `Allow all actions and reusable workflows`          |
| `Allow GitHub Actions to create and approve pull requests` | `false`                                             |
| `Artifact and log retention`                               | `30 days`                                           |
| `Fork pull request workflows from outside collaborators`   | `Require approval for all outside collaborators`    |
| `Workflow permissions`                                     | `Read repository contents and packages permissions` |


## Recommended Extensions

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)


## Convención de nombres en el código generado

Cada construcción EZduino tiene una representación exacta en el `.ino` generado.
El nombre del componente o variable (`NAME`) se convierte a mayúsculas para los `#define` de pines.

### Componentes de hardware

| Componente EZduino | EZduino de entrada | C++ generado (global) | C++ generado (setup) |
| --- | --- | --- | --- |
| `led` | `led NAME on PIN;` | `#define PIN_NAME PIN` | `pinMode(PIN_NAME, OUTPUT);` |
| `buzzer` | `buzzer NAME on PIN;` | `#define PIN_NAME PIN` | `pinMode(PIN_NAME, OUTPUT);` |
| `button` | `button NAME on PIN;` | `#define PIN_NAME PIN` | `pinMode(PIN_NAME, INPUT);` |
| `potentiometer` | `potentiometer NAME on PIN;` | `#define PIN_NAME PIN` | _(ninguno)_ |
| `servo` | `servo NAME on PIN;` | `#include <Servo.h>` + `#define PIN_NAME PIN` + `Servo NAME;` | `NAME.attach(PIN_NAME);` |
| `ultrasonic` | `ultrasonic NAME on (trig: T, echo: E);` | `#define PIN_NAME_TRIG T` + `#define PIN_NAME_ECHO E` + helper `readUltrasonicDistance` | `pinMode(PIN_NAME_TRIG, OUTPUT);` + `pinMode(PIN_NAME_ECHO, INPUT);` |
| `dht11` | `dht11 NAME on PIN;` | `#include <DHT.h>` + `#define PIN_NAME PIN` + `DHT NAME(PIN_NAME, DHT11);` | `NAME.begin();` |
| `lcd` | `lcd NAME on (rs:R, en:E, d4:D4, d5:D5, d6:D6, d7:D7);` | `#include <LiquidCrystal.h>` + `#define PIN_NAME_rs R` … `#define PIN_NAME_d7 D7` + `LiquidCrystal NAME(PIN_NAME_rs, …);` | `NAME.begin(16, 2);` |

### Variables en routine

| Caso | EZduino de entrada | C++ generado (global) | C++ generado (loop) |
| --- | --- | --- | --- |
| Literal entero | `var x = 42` | `int x = 42;` | _(ninguno, ya inicializada)_ |
| Literal float | `var x = 3.14` | `float x = 3.14;` | _(ninguno)_ |
| Literal bool | `var x = true` | `bool x = true;` | _(ninguno)_ |
| Literal string | `var x = "hola"` | `String x = "hola";` | _(ninguno)_ |
| Runtime (método de componente) | `var temp = sensor.read_temperature()` | `float temp = 0.0;` | `temp = sensor.readTemperature();` |
| Runtime (método entero) | `var d = sonar.distance` | `int d = 0;` | `d = readUltrasonicDistance(PIN_SONAR_TRIG, PIN_SONAR_ECHO);` |

### Ejemplo real: `19-dht11-lcd.ezd`

```ezduino
hardware {
    dht11 climate_sensor on 4;
    lcd screen on (rs: 12, en: 11, d4: 5, d5: 6, d6: 3, d7: 2);
}
routine {
    repeat every 2s {
        var temp = climate_sensor.read_temperature();
        screen.print(temp);
    }
}
```

Genera:

```cpp
#include <LiquidCrystal.h>
#include <DHT.h>

// ============================================================
// EZduino Generated Code
// DO NOT EDIT the generated sections manually.
// You can add custom code in the marked extension points.
// ============================================================

#define PIN_CLIMATE_SENSOR 4
#define PIN_SCREEN_rs 12
#define PIN_SCREEN_en 11
#define PIN_SCREEN_d4 5
#define PIN_SCREEN_d5 6
#define PIN_SCREEN_d6 3
#define PIN_SCREEN_d7 2

DHT climate_sensor(PIN_CLIMATE_SENSOR, DHT11);
LiquidCrystal screen(PIN_SCREEN_rs, PIN_SCREEN_en, PIN_SCREEN_d4, PIN_SCREEN_d5, PIN_SCREEN_d6, PIN_SCREEN_d7);

unsigned long last_millis_0 = 0;
float temp = 0.0;

void setup() {
  climate_sensor.begin();
  screen.begin(16, 2);
  // === Add custom setup code here ===
}

void loop() {
  if (millis() - last_millis_0 >= 2000) {
    last_millis_0 = millis();
    temp = climate_sensor.readTemperature();
    screen.print(temp);
  }
  // === Add custom loop code here ===
}
```
