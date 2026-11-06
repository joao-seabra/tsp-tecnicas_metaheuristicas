// João Pedro Seabra Nogueira
// 25.1.4003
#include "ILS.h"
#include "BuscaLocal.h"
#include "Utilitarios.h"
#include "Aleatorio.h"

#include <algorithm> // std::swap
#include <iostream>

std::vector<int> perturbacao(const Instancia &inst, const std::vector<int> &s, int nivel){
    std::vector<int> sLinha = s;
    int n = inst.n;
    
    int nModificacoes = nivel + 1;
    int cont = 0;
    int i = 0, j = 0;
    while(cont < nModificacoes){
            i = inteiroAleatorio(0, n-1);
            do{
                j = inteiroAleatorio(0, n-1);
            }while(i == j);
            std::swap(sLinha[i], sLinha[j]);

        cont++;
    }

    return sLinha;
}


// ============================================================================
// [EXERCÍCIO]
// ============================================================================
double ils(const Instancia &inst, std::vector<int> &s, int vezesNivel, int ilsMax)
{

    int n = inst.n;
    
    double fo;
    int iter = 0, melhorIter = 0;
    int nivel = 1, contNivel = 1;
    
    fo = descidaPrimeiroMelhora(inst, s);
    std::vector<int> sLinha;
    double foLinha;
    while (iter - melhorIter < ilsMax){
        iter++;
        sLinha = perturbacao(inst, s, nivel);
        foLinha = descidaPrimeiroMelhora(inst, sLinha);
        if(foLinha < fo){
            s = sLinha;
            fo = foLinha;
            melhorIter = iter;
            nivel = 1;
            contNivel = 1;
        }
        else if(++contNivel > vezesNivel){
            nivel++;
            contNivel = 1;
        }

    }
    

    return fo;
}
