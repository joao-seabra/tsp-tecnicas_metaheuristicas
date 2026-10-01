//João Pedro Seabra Nogueira
//25.1.4003

#include "MultiStart.h"
#include "Construcao.h"
#include "BuscaLocal.h"
#include "Utilitarios.h"

// ============================================================================
// [EXERCÍCIO]
// ============================================================================
double multiStart(const Instancia &inst, std::vector<int> &s, int itermax)
{
    // Primeira tentativa: construção gulosa, usada como referência inicial.
    std::vector<int> sEstrela;
    double foEstrela = __DBL_MAX__;
    
    //TODO

    double fo;

    //constroi a solucao gulosa inicial caso não tenha sido passada
    if(s.empty()) fo = constroiSolucaoGulosaInsercaoMaisBarata(inst, s);

    else fo = custo(inst, s);

    int iter = 0;
    while(iter < itermax){ //Máximo de iterações sem melhora
        iter++;
        foEstrela = constroiSolucaoAleatoria(inst, sEstrela);
        foEstrela = descidaPrimeiroMelhora(inst, sEstrela);
        //foEstrela = descidaCompleta(inst, sEstrela);
        //foEstrela = descidaRandomica(inst, sEstrela, 0.7*inst.n*(inst.n - 1)/2);
        if(foEstrela < fo){
            fo = foEstrela;
            s = sEstrela;
            //Zera o contador a cada melhora
            iter = 0; 
        }
        
    }

    //s = sEstrela;
    return fo;
}
