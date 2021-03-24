#ifndef TRILHO_H
#define TRILHO_H
#include <semaphore.h>

#define GREEN 0
#define RED 1
class Trilho
{
public:
    Trilho(int ID, int x1, int y1, int x2, int y2);
    sem_t semaforo;
    int state = GREEN;
    int x1; // define as extremidades dos trilhos
    int y1;
    int x2;
    int y2;

private:
    int ID;
};

#endif // TRILHO_H
