/* COMFIGURAÇÕES FILTRO DE KALMAN */

#ifndef _KalmanAltitude_h
#define _KalmanAltitude_h

#include <BasicLinearAlgebra.h>

/* definindo o uso da sigla bla para a algebra linear da biblioteca */
using namespace BLA;  

class KalmanAltitude {
  public:
    double Altitude, Velocity;

    /* Calculando a altitude */
    KalmanAltitude() {
      /*F = Matriz de transição de estados: [[1, Tclock], [0, 1]], só para auxilio do calculo*/
      F = {1, 0.003, 0, 1};
      /*G = Matriz pra descrever os calculos, [0.5*Tclock^2,  Tclock]*/
      G = {0.5 * 0.003 * 0.003, 0.003};
      /*H = Matriz de auxilio pro calculo*/
      H = {1, 0};
      /*I = matriz identidade, para  descrever o calculo*/
      I = {1, 0, 0, 1};
      /*
      Q = Matriz de covariância do ruído do processo, representa o erro inerente do processo,
      no nosso caso é assumido 0.7m/s^2 de desvio padrao pro acelerometro
      */
      float desvioPadraoAcc = 0.7;
      Q = (G * ~G) * (desvioPadraoAcc * desvioPadraoAcc);
      /*R = Incerteza da mediçao, 0.25m de desvio padrao é assumido aqui*/
      R = {0.25 * 0.25};
      /*P = Matriz de covariância de erro de estimativa, ela fornece o erro para a estimativa corrente*/
      P = {0, 0, 0, 0};
      /*S = Vetor de estados, [altitude_kalman, velocidade_kalman]*/
      S = {0, 0};
    };
  
    /* uso da altitude com o barômetro */
    void update(double AccZInertial, double AltitudeBarometer, double dt) {
      F = {1, dt, 0, 1};  
      G = {0.5 * dt * dt, dt};
      H = {1, 0};
      I = {1, 0, 0, 1};
      float desvioPadraoAcc = 0.7;
      Q = G * ~G * desvioPadraoAcc * desvioPadraoAcc;

      /*Acc é a aceleracao, no nosso caso aceleracao vertical medida pelo acelerômetro*/
      Acc = {AccZInertial};
      S=F*S+G*Acc;
      P=F*P*~F+Q;

      /*L = Somente uma matriz de auxilio pro calculo*/
      L=H*P*~H+R;
      /*K é a Matriz de ganho de Kalman, ela que decide que ajusta as estimativas*/
      K=P*~H*Inverse(L);
      /*M = Matriz que guarda a altura com base no barometro*/
      M = {AltitudeBarometer};
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
