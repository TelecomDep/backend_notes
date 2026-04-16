## Установка

Зависимости:
```bash
sudo apt install libsdl2-dev libgl1-mesa-dev libglew-dev libpq-dev curl libstb-dev  libcurl4-openssl-dev libpq-dev

```

Клонируем:
```bash
git clone --recursive https://github.com/TelecomDep/backend_notes.git
```

Сборка\запуск:
```bash
mkdir build && cd build
cmake ../
make -j16
./main
```
