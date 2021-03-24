#include "trem.h"
#include <semaphore.h>
#include <QtCore>
//#include "trilho.h"


sem_t s[7]; // semaforo dos trilhos
sem_t mutex;
int state[7]; // estado do trilho

//criar trilhos com seu ID e Posições ()
Trilho *trilho1;
Trilho *trilho2;
Trilho *trilho3;
Trilho *trilho4;

//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->xMin = x;
    this->xMax = (x + 230);
    this->yMin = y;
    this->yMax = y+120;
    velocidade = 500;
    this->ligado = true;

}

//Função a ser executada após executar trem->START
void Trem::run(){
    while(true){
        bloqTrilho();
        emit updateGUI(ID, x,y);    //Emite um sinal
        if (y == yMin && x <xMax)
            x+=10;
        else if (x >= xMax && y < yMax)
            y+=10;
        else if (x > xMin && y >= yMax)
            x-=10;
        else
            y-=10;
        libTrilho();
        msleep(velocidade);
    }
}

void Trem::AjustarVelocidade(int Valor){
    if(Valor == 0){
       this->terminate();
       ligado = false;
    }
    else
    {
    if (!ligado)
        { ligado = true;
        this->start();

    }//verifacar se a threat está parada
    velocidade = 1000 - Valor;
    }
}

void Trem::InicializarTrilhos()
{
    sem_init(&mutex, 0, 1);
    trilho1 = new Trilho(1, 240, 30, 240, 150);
    trilho2 = new Trilho(2, 470, 30, 470, 150);
    trilho3 = new Trilho(3, 120,150, 240, 150);
    trilho4 = new Trilho(4, 240,150, 350, 150);

}

void Trem::verTrilho()
{
 // sem_wait(&mutex);
  bloqTrilho();
  emit updateGUI(ID, x,y);    //Emite um sinal
  //sem_post(&mutex);

}

void Trem::bloqTrilho()
{
    if (this->ID ==1)
    {
        if (this->x+20 == trilho1->x1 && this->y == trilho1->y1 )
        {
            sem_wait(&trilho1->semaforo);
            trilho1->state = RED;
        }
        //trilho 3
        if (this->x == trilho3->x2 && this->y+20 == trilho3->y2 )
        {
            sem_wait(&trilho3->semaforo);
            trilho3->state = RED;
        }
    }
    if (this->ID ==2)
    {
        if (this->x-20 == trilho1->x2 && this->y == trilho1->y2 )
        {
            sem_wait(&trilho1->semaforo);
            trilho1->state = RED;
        }
        //trilho 2
        if (this->x+20 == trilho2->x1 && this->y == trilho2->y1 )
        {
            sem_wait(&trilho2->semaforo);
            trilho2->state = RED;
        }
        //trilho4
        if (this->x-20 == trilho4->x2 && this->y == trilho4->y2 )
        {
            sem_wait(&trilho4->semaforo);
            sem_wait(&trilho3->semaforo);
            trilho4->state = RED;
            trilho3->state = RED;
        }

    }
    if (this->ID == 3)
    {
        if (this->x-20 == trilho2->x2 && this->y == trilho2->y2 )
        {
            sem_wait(&trilho2->semaforo);
            trilho2->state = RED;
        }
    }
    if (this->ID == 4)
    {
        if (this->x == trilho3->x1 && this->y == trilho3->y1 )
        {
            sem_wait(&trilho3->semaforo);
            sem_wait(&trilho4->semaforo);
            trilho3->state = RED;
            trilho4->state = RED;
        }
    }


}
void Trem::libTrilho()
{
    sem_wait(&mutex);

    if (this->ID ==1)
    {
        if (this->x == (trilho1->x2-20) && this->y == trilho1->y2 && trilho1->state == RED )
        {
            trilho1->state = GREEN;
            sem_post(&trilho1->semaforo);
        }
        if (this->x == (trilho3->x1-20) && this->y == trilho3->y1 && trilho3->state == RED )
        {
            trilho3->state = GREEN;
            sem_post(&trilho3->semaforo);
        }

    }
    if (this->ID ==2)
    {
        if ( this->x == (trilho1->x1+20) && this->y == trilho1->y1 && trilho1->state == RED )
        {
            trilho1->state = GREEN;
            sem_post(&trilho1->semaforo);
        }
        //trilho 2
        if ( this->x == (trilho2->x2-20) && this->y == trilho2->y2 && trilho2->state == RED )
        {
            trilho2->state = GREEN;
            sem_post(&trilho2->semaforo);
        }
        //trilho 4 e 3
        if ( this->x == (trilho1->x2) && this->y == trilho1->y2-20 && trilho4->state == RED )
        {
            trilho3->state = GREEN;
            trilho4->state = GREEN;
            sem_post(&trilho3->semaforo);
            sem_post(&trilho4->semaforo);
        }
    }
    if (this->ID ==3)
    {
        if ( this->x == (trilho2->x1+20) && this->y == trilho2->y1 && trilho2->state == RED )
        {
            trilho2->state = GREEN;
            sem_post(&trilho2->semaforo);
        }
    }
    if (this->ID == 4)
    {
        if ( this->x == (trilho3->x2+20) && this->y == trilho3->y2 && trilho3->state == RED )
        {
            trilho3->state = GREEN;
            sem_post(&trilho3->semaforo);
        }
        if ( this->x == (trilho4->x2) && this->y == trilho4->y2+20 && trilho4->state == RED )
        {
            trilho4->state = GREEN;
            sem_post(&trilho4->semaforo);
        }
    }
    sem_post(&mutex);

}





