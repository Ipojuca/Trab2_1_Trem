#ifndef TREM_H
#define TREM_H

#include <QThread>
#include "trilho.h"

#define NORMAL 0
#define CRITICAL 1
/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função TERMINATE.
 *
*/
class Trem: public QThread{
 Q_OBJECT
public:
    Trem(int,int,int);  //construtor
    void run();         //função a ser executada pela thread
    void AjustarVelocidade(int);
    void InicializarTrilhos();
    void bloqTrilho(); //bloquear trilho
    void libTrilho();  //liberar trilho
    int x;           //posição X do trem na tela
    int y;           //posição Y do trem na tela


//Cria um sinal
signals:
    void updateGUI(int,int,int);

private:
   int xMin;        //posição mínima do x
   int yMin;        //posição mínima do y
   int xMax;        //posição Máxima do X
   int yMax;        //posicao Máxima do Y
   int ID;          //ID do trem
   int velocidade;  //Velocidade. É o tempo de dormir em milisegundos entre a mudança de posição do trem
   bool ligado;

};

#endif // TREM_H
