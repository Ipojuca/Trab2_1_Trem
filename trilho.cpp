#include "trilho.h"
#include <semaphore.h>
Trilho::Trilho(int ID, int x1, int y1, int x2, int y2)
{
    this->ID = ID;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    sem_init(&semaforo, 0, 1);


}
