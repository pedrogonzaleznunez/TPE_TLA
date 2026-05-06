# How to run

## Build

```bash
docker compose run --rm compiler bash src/main/bash/build.sh
```

Regenerates Flex/Bison sources and compiles the binary into `.build/`.

## Run a file

```bash
docker compose run --rm compiler bash src/main/bash/run.sh src/test/c/accept/09-blink-led
```

Exits with `0` on success, non-zero on parse error.

## Run all tests

```bash
docker compose run --rm compiler bash src/main/bash/test.sh
```

## Interactive shell

```bash
docker compose run --rm compiler
```
