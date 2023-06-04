/* COMFIGURAÇÕES FILTRO DE KALMAN */

#ifndef _KalmanAltitude_h
#define _KalmanAltitude_h

#include <BasicLinearAlgebra.h>
using namespace BLA;  /* definindo o uso da sigla bla para a algebra linear da biblioteca */

class KalmanAltitude {
public:
    double Altitude, Velocity;

    /* Calculando a altitude */
    KalmanAltitude() {
        F = {1, 0.003,              /*F = Matriz de transição de estados: [[1, Tclock], [0, 1]], só para auxilio do calculo*/
             0, 1};  
        G = {0.5*0.003*0.003,       /*G = Matriz pra descrever os calculos, [0.5*Tclock^2,  Tclock]*/
            0.003           };
        H = {1, 0};                 /*H = Matriz de auxilio pro calculo*/
        I = {1, 0,                  /*I = matriz identidade, para  descrever o calculo*/
             0, 1};
        Q = G * ~G*10*10;           /*Q = Matriz de covariância do ruído do processo, representa o erro inerente do processo, no nosso caso é assumido 10cm/s^2 de desvio padrao pro acelerometro*/    
        R = {30*30};                /*R = Incerteza da mediçao, 30cm de desvio padrao é assumido aqui*/
        P = {0, 0,                  /*P = Matriz de covariância de erro de estimativa, ela fornece o erro para a estimativa corrente*/
             0, 0};
        S = {0,                     /*S = Vetor de estados, [altitude_kalman, velocidade_kalman]*/
             0};
    };

    /* uso da altitude com o barômetro */
    void update(double AccZInertial, double AltitudeBarometer, double dt) {
        F = {1, dt,
          0, 1     };  
        G = {0.5*dt*dt,
            dt           };
        H = {1, 0};
        I = {1, 0,
            0, 1};
        Q = G * ~G*10*10;

        Acc = {AccZInertial};       /*Acc é a aceleracao, no nosso caso aceleracao vertical medida pelo acelerômetro*/
        S=F*S+G*Acc;
        P=F*P*~F+Q;
        L=H*P*~H+R;                 /*L = Somente uma matriz de auxilio pro calculo*/
        K=P*~H*Invert(L);           /*K é a Matriz de ganho de Kalman, ela que decide que ajusta as estimativas*/
        M = {AltitudeBarometer};    /*M = Matriz que guarda a altura com base no barometro*/
        S=S+K*(M-H*S);

        Altitude=S(0,0);  // Kalman
        Velocity=S(1,0);  // Kalman
        P=(I-K*H)*P;
    }

private:
    BLA::Matrix<2,2> F;
    BLA::Matrix<2,2> P;
    BLA::Matrix<2,1> S;
    BLA::Matrix<2,2> I;
    BLA::Matrix<2,1> K;
    BLA::Matrix<1,1> L;

    BLA::Matrix<2,1> G;
    BLA::Matrix<2,2> Q;
    BLA::Matrix<1,2> H;
    BLA::Matrix<1,1> Acc;
    BLA::Matrix<1,1> R;
    BLA::Matrix<1,1> M;
};

#endif