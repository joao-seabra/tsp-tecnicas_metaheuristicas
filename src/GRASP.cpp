#include "GRASP.h"
#include "Construcao.h"
#include "BuscaLocal.h"
#include "Utilitarios.h"

#include <limits>

// ============================================================================
// [EXERCÍCIO]
// ============================================================================
// Observação para o professor: no código original em C, a construção por
// Inserção Mais Barata (tipoConstrucao == 2) vinha DESABILITADA em
// GRASP.cpp por causa de um bug na heurística correspondente. Como esse
// bug foi corrigido em Construcao.cpp (veja o comentário lá), aqui as
// duas opções de construção já funcionam normalmente.
// ============================================================================
double grasp(const Instancia &inst, std::vector<int> &s,
             double alpha, int graspMax, int tipoConstrucao)
{
    std::vector<int> sEstrela;
    double foEstrela = std::numeric_limits<double>::max();

    double fo;
    //TODO
    int iter = 0;
    while (iter < graspMax){
        iter++;
        switch(tipoConstrucao){
            case 1:
                constroiSolucaoParcialmenteGulosaVizinhoMaisProximo(inst, s, alpha);
                break;
            case 2:
                constroiSolucaoParcialmenteGulosaInsercaoMaisBarata(inst, s, alpha);
                break;
            default:
                std::exit(1);
                break;
        }
        fo = descidaCompleta(inst, s);
        if(fo < foEstrela){
            sEstrela = s;
            foEstrela = fo;
            iter = 0;
        }

    }

    s = sEstrela;
    return foEstrela;
}
