# ChessOxygine
Шахматы, написанные на oxygine

## Сборка проекта

### Требования
- CMake 3.10+
- Компилятор с поддержкой C++17
- SDL2
- OpenGL
- Библиотеки: JPEG, PNG, Zlib

### Инструкция по сборке

1. Клонируйте репозиторий в папку (желательно назвать ее "GameProject" во избежании возможных ошибок сборки): https://github.com/C-plus-plus-Developer/ChessOxygine.git
2. Клонируйте репозиторий самого движка в ту же папку: https://github.com/oxygine/oxygine-framework.git
3. Ваша структура должна выглядеть так: 
<img width="746" height="305" alt="image" src="https://github.com/user-attachments/assets/e550f541-e39c-4567-919e-d1c3754d47a7" />
4. Далее, в этой папке откроем терминал и выполним: 
mkdir build && cd build
cmake .. && make
./GameProject
Ожидаемый результат: 
<img width="952" height="686" alt="image" src="https://github.com/user-attachments/assets/f3a86f01-4e35-4d1b-a628-eda17f054b53" />

