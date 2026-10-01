//João Pedro Seabra Nogueira
//25.1.4003
#include "SimulatedAnnealing.h"
#include "BuscaLocal.h"   // calculaDelta
#include "Utilitarios.h"
#include "Registro.h"
#include "Aleatorio.h"

#include <cmath>      // std::exp
#include <algorithm>  // std::swap
#include "Cronometro.h"

double vizinhoSimulatedAnnealing(const Instancia &inst, std::vector<int> &s, double fo,int &i, int &j);

// ============================================================================
// [EXERCÍCIO]
// ============================================================================
double simulatedAnnealing(const Instancia &inst, std::vector<int> &s,
                           double alpha, int saMax,
                           double tempInicial, double tempFinal)
{
    int n = inst.n;
    std::vector<int> sEstrela = s;

    double temperatura = tempInicial;
    double fo = custo(inst, s);
    double foEstrela = fo;

    const std::string arquivoLog = "SAsaida.txt";
    limpaArquivo(arquivoLog);
    registraProgresso(arquivoLog, 0.0, 0, foEstrela);

    Cronometro cron;

    double chanceAceitacao, delta;
    double aceitacao;
    double foVizinho = fo;

    int i, j;
    while (temperatura > tempFinal){
        
        for(int iter = 0; iter < saMax; iter++){
            foVizinho = vizinhoSimulatedAnnealing(inst, s, fo, i, j);

            if(foVizinho < fo){
                fo = foVizinho;
                std::swap(s[i], s[j]);
                if(fo < foEstrela){
                    foEstrela = fo;
                    sEstrela = s;
                }
            }
            else{
                aceitacao = realAleatorio(0.0, 1);

                delta = foVizinho - fo;
                chanceAceitacao = std::exp(-delta / temperatura);

                if(aceitacao < chanceAceitacao){
                    fo = foVizinho;
                    std::swap(s[i], s[j]);
                }
                
            }

        }
        temperatura *= alpha;
    }

    s = sEstrela;
    registraProgresso(arquivoLog, cron.segundosDecorridos(), 0, foEstrela);
    return foEstrela;
}

double vizinhoSimulatedAnnealing(const Instancia &inst, std::vector<int> &s, double fo, int &i, int &j){
    i = inteiroAleatorio(0, inst.n - 1);
    do{
        j = inteiroAleatorio(0, inst.n - 1);
    }while(i==j);

    double delta1 = calculaDelta(inst, s, i, j);
    std::swap(s[i], s[j]);
    double delta2 = calculaDelta(inst, s, i, j);
    std::swap(s[i], s[j]);

    double foVizinho = fo - delta1 + delta2;

    return foVizinho;

}


// ============================================================================
// [FRAMEWORK]
// ============================================================================
double calculaTemperaturaInicial(const Instancia &inst, std::vector<int> &s,
                                  double beta, double gamma, int saMax)
{
    int n = inst.n;
    double temperatura = 10.0; // chute inicial

    bool continua = true;
    while (continua) {
        int aceitos = 0;

        for (int iterT = 0; iterT < saMax; iterT++) {
            int i = inteiroAleatorio(0, n - 1);
            int j;
            do {
                j = inteiroAleatorio(0, n - 1);
            } while (j == i);

            double delta1 = calculaDelta(inst, s, i, j);
            std::swap(s[i], s[j]);
            double delta2 = calculaDelta(inst, s, i, j);
            double delta = delta2 - delta1;

            if (delta < 0) {
                aceitos++;
            } else if (realAleatorio(0.0, 1.0) < std::exp(-delta / temperatura)) {
                aceitos++;
            }

            std::swap(s[i], s[j]); // desfaz o movimento (aqui só testamos)
        }

        if (aceitos < gamma * saMax) {
            temperatura *= beta; // ainda aceitando pouco: aquece mais
        } else {
            continua = false;
        }
    }
    return temperatura;
}
