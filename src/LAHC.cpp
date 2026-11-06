// João Pedro Seabra Nogueira
// 25.1.4003
#include "LAHC.h"
#include "Utilitarios.h"
#include "BuscaLocal.h"
#include "Aleatorio.h"

#include <algorithm>
#include <iostream>

// ============================================================================
// [EXERCÍCIO - método futuro do cronograma]
// ============================================================================
double lahc(const Instancia &inst, std::vector<int> &s, int tamanhoLista, int lahcMax)
{
    double fo = custo(inst, s);
    int n = inst.n;

    std::vector<int> sLinha = s;
    double foLinha = fo;
    std::vector<int> sEstrela = s;
    double foEstrela = fo;
    
    std::vector<double> lista(tamanhoLista); //lista de soluções antigas geradas
    lista.assign(tamanhoLista, fo);
    
    int cont = 0;
    int p = 0;
    int i = 0, j = 0;
    double delta1, delta2;
    while(cont < lahcMax){
        i = inteiroAleatorio(0, n-1);
        do{
            j = inteiroAleatorio(0, n-1);
        }while(i == j);

        delta1 = calculaDelta(inst, sLinha, i, j);
        std::swap(sLinha[i], sLinha[j]);
        delta2 = calculaDelta(inst, sLinha, i, j);
        foLinha = foLinha - delta1 + delta2;

        if(foLinha <= fo || foLinha <= lista.at(p)){
            if(foLinha < fo){
                cont = 0;
            }
            s = sLinha;
            fo = foLinha;
            if(fo < foEstrela){
                sEstrela = s;
                foEstrela = fo;
            }
        }
        else{ // desfaz
            std::swap(sLinha[i], sLinha[j]);
            foLinha = fo;
        }

        lista.at(p) = fo;
        p = (p+1) % tamanhoLista;
        cont++;
    }

    s = sEstrela;
    return foEstrela;
}
