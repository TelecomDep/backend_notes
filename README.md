## Установка

Зависимости:
```bash
sudo apt install libsdl2-dev libgl1-mesa-dev libglew-dev
```

Клонируем:
```bash
git clone https://github.com/TelecomDep/backend_notes.git
git submodule update --recursive
```

Сборка\запуск:
```bash
mkdir build && cd build
cmake ../
make -j16
./main
```
