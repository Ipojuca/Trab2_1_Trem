#include "trem.h"
#include <semaphore.h>
#include <QtCore>



sem_t Regions[3]; // semaforo das Regioes Criticas  (0)Trilhos(1,3,4), (1)Trilhos (2,5,6), (2) (4,5,7)
sem_t BigReg[2]; // Semaforo para agrupar as Regioes criticas (0) Regions[0,2] (1) Regions [1,2]
sem_t Mutex[3]; // Garante a atomicidade das transações (Regions e Trilhos)

sem_t mutex;

//criar trilhos com seu ID e Posições ()
Trilho *trilho1;
Trilho *trilho2;
Trilho *trilho3;
Trilho *trilho4;
Trilho *trilho5;
Trilho *trilho6;
Trilho *trilho7;

//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    //this->x = x;
    //this->y = y;
    this->xMin = x;
    this->xMax = (x + 230);
    this->yMin = y;
    this->yMax = y+120;
    velocidade = 500;
    this->ligado = true;

}

//Função a ser executada após executar trem->START
void Trem::run()
{
    while(true)
    {
        if (y == yMin && x <xMax)
            x+=10;
        else if (x >= xMax && y < yMax)
            y+=10;
        else if (x > xMin && y >= yMax)
            x-=10;
        else
            y-=10;
        bloqTrilho();
        emit updateGUI(ID, x,y);    //Emite um sinal
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
    for (int i = 0; i < 3; ++i) {
        sem_init(&Mutex[i], 0, 1);
        sem_init(&Regions[i], 0, 2);
    }
    sem_init(&BigReg[0], 0, 3);
    sem_init(&BigReg[1], 0, 3);

    trilho1 = new Trilho(1, 240, 30, 240, 150);
    trilho2 = new Trilho(2, 470, 30, 470, 150);
    trilho3 = new Trilho(3, 120,150, 240, 150);
    trilho4 = new Trilho(4, 240,150, 350, 150);
    trilho5 = new Trilho(5, 350,150, 470, 150);
    trilho6 = new Trilho(6, 470,150, 580, 150);
    trilho7 = new Trilho(7, 350,150, 350, 270);

}

void Trem::bloqTrilho()
{
    if (this->ID ==1)
    {
        if (this->x+10 == trilho1->x1 && this->y == trilho1->y1 )
        {
            sem_wait(&BigReg[0]);
            sem_wait(&Regions[0]);
            sem_wait(&trilho1->semaforo);
            trilho1->state = RED;
        }
        //trilho 3
        if (this->x == trilho3->x2 && this->y+10 == trilho3->y2 )
        {
            sem_wait(&trilho3->semaforo);
            trilho3->state = RED;
        }
    }
    if (this->ID == 2) //Trem 2
    {
        //Trilho 1
        if (this->x-20 == trilho1->x2 && this->y == trilho1->y2 )
        {
            sem_wait(&trilho1->semaforo);
            trilho1->state = RED;
        } else
        //trilho 2
        if (this->x+20 == trilho2->x1 && this->y == trilho2->y1 )
        {
            sem_wait(&BigReg[1]);
            sem_wait(&Regions[1]);
            sem_wait(&trilho2->semaforo);
            trilho2->state = RED;
        } else
        //trilho 5
        if (this->x == trilho5->x2 && this->y == trilho5->y2-20 )
        {
            sem_wait(&BigReg[0]);
            sem_wait(&Regions[2]);
            sem_wait(&trilho5->semaforo);
            trilho5->state = RED;
        } else
        //trilho4
        if (this->x == trilho4->x2+20 && this->y == trilho4->y2 )
        {
            sem_wait(&Regions[0]);
            sem_wait(&trilho4->semaforo);
            trilho4->state = RED;
        }
    }
    if (this->ID == 3)
    {
        //Trilho 6
        if (this->x == trilho6->x2+10 && this->y == trilho6->y2 )
        {
            sem_wait(&BigReg[1]);
            sem_wait(&Regions[1]);
            sem_wait(&trilho6->semaforo);
            trilho6->state = RED;
        }
        //Trilho2
        if (this->x == trilho2->x2+10 && this->y == trilho2->y2 )
        {
            sem_wait(&trilho2->semaforo);
            trilho2->state = RED;
        }

    }
    if (this->ID == 4)
    {
        if (this->x == trilho3->x1 && this->y == trilho3->y1+10 )
        {
            sem_wait(&BigReg[0]);
            sem_wait(&Regions[0]);
            sem_wait(&trilho3->semaforo);
            trilho3->state = RED;
        }
        if (this->x+10 == trilho4->x1 && this->y == trilho4->y1 )
        {
            sem_wait(&BigReg[1]);
            sem_wait(&Regions[2]);
            sem_wait(&trilho4->semaforo);
            trilho4->state = RED;
        }
        //Trilho 7
        if (this->x == trilho7->x1-10 && this->y == trilho7->y1 )
        {
            sem_wait(&trilho7->semaforo);
            trilho7->state = RED;
        }

    }
    if (this->ID == 5)
    {
        if (this->x == trilho5->x1 && this->y == trilho5->y1+10 )
        {
            sem_wait(&trilho5->semaforo);
            sem_wait(&Regions[1]);
            trilho5->state = RED;
        }
        if (this->x+10 == trilho6->x1 && this->y == trilho6->y1 )
        {
            sem_wait(&trilho6->semaforo);
            trilho6->state = RED;
        }
        //Trilho 7
        if (this->x == trilho7->x2+10 && this->y == trilho7->y2 )
        {
            sem_wait(&BigReg[0]);
            sem_wait(&BigReg[1]);
            sem_wait(&Regions[2]);
            sem_wait(&trilho7->semaforo);
            trilho7->state = RED;
        }
    }
}
void Trem::libTrilho()
{
    sem_wait(&mutex);

    if (this->ID ==1)
    {
        if (this->x == (trilho1->x2-30) && this->y == trilho1->y2 && trilho1->state == RED )
        {
            trilho1->state = GREEN;
            sem_post(&trilho1->semaforo);
            sem_post(&Regions[0]);
            sem_post(&BigReg[0]);
        }
        if (this->x == (trilho3->x1-30) && this->y == trilho3->y1 && trilho3->state == RED )
        {
            trilho3->state = GREEN;
            sem_post(&trilho3->semaforo);
        }

    }
    if (this->ID ==2)
    {
        //Trilho 1
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

            sem_post(&BigReg[1]);
        }
        //Trilho 5
        if (this->x == (trilho5->x1-20) && this->y == trilho5->y1 && trilho5->state == RED )
        {
            trilho5->state = GREEN;
            sem_post(&trilho5->semaforo);
            sem_post(&Regions[1]);
            sem_post(&Regions[2]);
        }
        //trilho 4
        if ( this->x == (trilho4->x1) && this->y == trilho4->y1-20 && trilho4->state == RED )
        {
            trilho4->state = GREEN;
            sem_post(&trilho4->semaforo);
            sem_post(&Regions[0]);
            sem_post(&BigReg[0]);
        }

    }
    if (this->ID ==3)
    {
        //Trilho 6
        if ( this->x == (trilho6->x1) && this->y == trilho6->y1-30 && trilho6->state == RED )
        {
            trilho6->state = GREEN;
            sem_post(&trilho6->semaforo);
            sem_post(&Regions[1]);
            sem_post(&BigReg[1]);
        }
        //Trilho 2
        if ( this->x == (trilho2->x1+30) && this->y == trilho2->y1 && trilho2->state == RED )
        {
            trilho2->state = GREEN;
            sem_post(&trilho2->semaforo);
        }


    }
    if (this->ID == 4)
    {
        if ( this->x == (trilho3->x2+30) && this->y == trilho3->y2 && trilho3->state == RED )
        {
            trilho3->state = GREEN;
            sem_post(&trilho3->semaforo);
        }
        if ( this->x == (trilho4->x2) && this->y == trilho4->y2+30 && trilho4->state == RED )
        {
            trilho4->state = GREEN;
            sem_post(&trilho4->semaforo);
            sem_post(&Regions[0]);
            sem_post(&BigReg[0]);
        }
        if ( this->x == (trilho7->x2-30) && this->y == trilho7->y2 && trilho7->state == RED )
        {
            trilho7->state = GREEN;
            sem_post(&trilho7->semaforo);
            sem_post(&Regions[2]);        }
            sem_post(&BigReg[1]);
    }
    if (this->ID == 5)
    {
        if ( this->x == (trilho5->x2+30) && this->y == trilho5->y2 && trilho5->state == RED )
        {
            trilho5->state = GREEN;
            sem_post(&trilho5->semaforo);
            sem_post(&Regions[2]);
            sem_post(&BigReg[1]);

        }
        if ( this->x == (trilho6->x2) && this->y == trilho6->y2+30 && trilho6->state == RED )
        {
            trilho6->state = GREEN;
            sem_post(&trilho6->semaforo);
            sem_post(&Regions[1]);

        }
        if ( this->x == (trilho7->x1+30) && this->y == trilho7->y1 && trilho7->state == RED )
        {
            trilho7->state = GREEN;
            sem_post(&trilho7->semaforo);
            sem_post(&BigReg[0]);

        }
    }
    sem_post(&mutex);

}





