#include "trem.h"
#include <QtCore>

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
        if (y == yMin && x <xMax)
            x+=10;
        else if (x >= xMax && y < yMax)
            y+=10;
        else if (x > xMin && y >= yMax)
            x-=10;
        else
            y-=10;
        emit updateGUI(ID, x,y);    //Emite um sinal

        //        switch(ID){
        //        case 1:     //Trem 1
        //            if (y == 150 && x <355)
        //                x+=10;
        //            else if (x >= 355 && y < 270)
        //                y+=10;
        //            else if (x > 125 && y >= 270)
        //                x-=10;
        //            else
        //                y-=10;
        //            emit updateGUI(ID, x,y);    //Emite um sinal
        //            break;
        //        case 2: //Trem 2
        //            if (y == 150 && x <585)
        //                x+=10;
        //            else if (x >= 585 && y < 270)
        //                y+=10;
        //            else if (x > 355 && y >= 270)
        //                x-=10;
        //            else
        //                y-=10;
        //            emit updateGUI(ID, x,y);    //Emite um sinal
        //            break;
        //        default:
        //            break;

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
// verificando o commit and push




