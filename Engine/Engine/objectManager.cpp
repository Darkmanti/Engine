// objectManager должен предоставлять доступ ко всем
// объектам находящимся в игровой сцене

// Генерация уникальных id
int genID()
{
    static int s_objectID(0);
    return s_objectID++;
}