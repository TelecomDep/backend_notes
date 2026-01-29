## Установка

Зависимости:
```bash
sudo apt install libsdl2-dev libgl1-mesa-dev libglew-dev
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
