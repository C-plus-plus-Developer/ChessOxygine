# ChessOxygine
Шахматы, написанные на oxygine

## Сборка проекта

### Требования
- CMake 3.10+
- Компилятор с поддержкой C++17
- SDL2
- OpenGL

### Инструкция по сборке

1. Клонируйте репозиторий: git clone https://github.com/C-plus-plus-Developer/ChessOxygine.git
2. Клонируйте репозиторий самого движка в уже скопированный репозиторий: git clone https://github.com/oxygine/oxygine-framework.git
3. Ваша структура должна выглядеть так: 

<img width="746" height="305" alt="image" src="https://github.com/user-attachments/assets/e550f541-e39c-4567-919e-d1c3754d47a7" />

4. Далее, в этой папке откроем терминал и выполним: 

mkdir build && cd build

cmake .. && make

./GameProject

Ожидаемый результат: 


<img width="951" height="681" alt="image" src="https://github.com/user-attachments/assets/b7f61cfe-c217-40ee-ab1f-6052cb1e5fa3" />

