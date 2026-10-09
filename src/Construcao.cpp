//João Pedro Seabra Nogueira
//25.1.4003
#include "Construcao.h"
#include "Aleatorio.h"
#include "Utilitarios.h"

#include <algorithm>  // std::min_element, std::sort, std::find
#include <numeric>    // std::iota
#include <limits>
#include <cstddef>
#include <map>
#include <cmath>

// ============================================================================
// [EXERCÍCIO] Vizinho Mais Próximo (guloso)
// Aula: Heurística Construtivas
// Slide ref: "PCV - Vizinho mais Próximo"
// Complexidade: O(n^2)
// ============================================================================
double constroiSolucaoGulosaVizinhoMaisProximo(const Instancia &inst,
                                              std::vector<int> &s)
{
    int n = inst.n;

    double fo = 0;
    // Cidades ainda não inseridas na rota (todas, exceto a cidade 0).
    std::vector<int> naoVisitadas(n - 1);

    // Preenche a lista com as cidades de 1 .. (n-1)
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    s.assign(n, -1);
    s[0] = 0; // a cidade origem é sempre a cidade 0

    //TODO
    int cidadeAtual = s[0];
    int indexProximaCidade = 0;
    int indexSolucao = 0;
    while(!naoVisitadas.empty()){ //enquanto não visitar todas as cidades

        double menorDist = __DBL_MAX__;
        double distancia = 0;

        for(size_t j = 0; j < naoVisitadas.size(); j++){
        //itera pelas cidades não visitadas procurando a menor distância
            distancia = inst.distancia(cidadeAtual, naoVisitadas[j]);
            if(distancia < menorDist){
                menorDist = distancia;
                indexProximaCidade = j;
            }
        }
        cidadeAtual = naoVisitadas[indexProximaCidade]; //visita a cidade de menor distância
        naoVisitadas.erase(naoVisitadas.begin() + indexProximaCidade);
        
        s[++indexSolucao] = cidadeAtual; //adiciona na solucao

        fo += menorDist;
        
    } 

    fo += inst.distancia(cidadeAtual, s[0]);

    return fo;
}

// ============================================================================
// [FRAMEWORK] Construção aleatória
// ============================================================================
double constroiSolucaoAleatoria(const Instancia &inst, std::vector<int> &s)
{
    int n = inst.n;

    // Cria a lista de cidades não visitadas
    std::vector<int> naoVisitadas(n - 1);

    // Preenche a lista com as cidades de 1 .. (n-1)
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    embaralhaVetor(naoVisitadas);

    // Redefine a solução com n posições
    s.assign(n, -1);

    // Insere a cidade de origem
    s[0] = 0;

    // Preenche com as demais cidades
    for (int j = 1; j < n; j++) {
        s[j] = naoVisitadas[j - 1];
    }

    return custo(inst, s);
}

// ============================================================================
// [EXERCÍCIO] Vizinho Mais Próximo parcialmente guloso (GRASP)
// ============================================================================
double constroiSolucaoParcialmenteGulosaVizinhoMaisProximo(const Instancia &inst,
                                                           std::vector<int> &s,
                                                           double alpha)
{
    int n = inst.n;
    double fo = 0;

    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    s.assign(n, -1);
    s[0] = 0;
    int iterS = 0;
    
    //TODO
    std::vector<std::pair<int, double>> g;
    int LCR = 0, ultimaInserida = 0;
    double gMin, gMax, gAlpha;
    while(!naoVisitadas.empty()){
        for(int cidade : naoVisitadas){
            g.push_back(std::pair<int, double>(cidade, inst.distancia(s[iterS], cidade)));
        }
        std::sort(g.begin(), g.end(), [](std::pair<int, double> &g1, std::pair<int, double> &g2){
            return g1.second < g2.second;
        });
        gMin = g.front().second;
        gMax = g.back().second;

        // LCR = std::ceil(std::max(1.0, alpha * g.size()));
        // ultimaInserida = inteiroAleatorio(0, LCR - 1);
        
        gAlpha = gMin + alpha * (gMax - gMin);
        LCR = 0;
        while(g.at(LCR).second < gAlpha) LCR++;

        ultimaInserida = inteiroAleatorio(0, LCR);

        s[++iterS] = g.at(ultimaInserida).first;
        fo += g.at(ultimaInserida).second;
        
        naoVisitadas.erase(naoVisitadas.cbegin() + ultimaInserida);        
    }

    return fo;
}

// ============================================================================
// [EXERCÍCIO] Inserção Mais Barata (gulosa)
// ============================================================================
double constroiSolucaoGulosaInsercaoMaisBarata(const Instancia &inst,
                                              std::vector<int> &s)
{
    int n = inst.n;

    double fo = 0;
    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    std::vector<int> rota;
    rota.reserve(n);
    rota.push_back(0); // a cidade origem é sempre a cidade 0

    // Monta uma subrota inicial com 3 cidades usando o vizinho mais
    // próximo (mais simples do que aplicar inserção mais barata em uma
    // rota com menos de 3 cidades).

    int cidadeAtual = rota[0];
    for(size_t i = 0; i < 3; i++){
        int indexProximaCidade = 0;
        double menorDist = __DBL_MAX__;
        double distancia = 0;
        for(size_t j = 0; j < naoVisitadas.size(); j++){
        //itera pelas cidades não visitadas procurando a menor distância
            distancia = inst.distancia(cidadeAtual, naoVisitadas[j]);
            if(distancia < menorDist){
                menorDist = distancia;
                indexProximaCidade = j;
            }
        }
        cidadeAtual = naoVisitadas[indexProximaCidade]; //visita a cidade de menor distância
        naoVisitadas.erase(naoVisitadas.begin() + indexProximaCidade);
        
        rota.push_back(cidadeAtual); //adiciona na rota

        fo += menorDist;
        
    }

    fo += inst.distancia(cidadeAtual, rota[0]);

    //TODO

    // A cada passo, insere a cidade k -- entre duas cidades i e j
    // consecutivas já presentes na rota -- que resulta no menor custo de
    // inserção: d(i,k) + d(k,j) - d(i,j).

    //TODO

    
    while(!naoVisitadas.empty()){
        
        int indexProximaCidade = 0;
        int indexCidadeJaVisitada = 0;
        double menorCusto = __DBL_MAX__;
        double custoInsercao = 0;
        
        for(size_t pos = 0; pos < rota.size(); pos++){

            int i = rota[pos];
            int j = rota[(pos+1) % rota.size()]; //vizinho de i na rota atual (volta para a cidade inicial depois da utlima)
            for(size_t k = 0; k < naoVisitadas.size(); k++){
                int cidadeK = naoVisitadas[k];

                custoInsercao = inst.distancia(i, cidadeK) + inst.distancia(cidadeK, j) - inst.distancia(i, j);
                if(custoInsercao < menorCusto){
                    menorCusto = custoInsercao;
                    indexCidadeJaVisitada = pos;
                    indexProximaCidade = k;
                }
            }
        }
        //insere a proxima cidade na rota, entre a cidade i e a j
        rota.emplace(rota.begin() + indexCidadeJaVisitada + 1, naoVisitadas[indexProximaCidade]);
        
        naoVisitadas.erase(naoVisitadas.begin() + indexProximaCidade);
        
        fo += menorCusto;
    }
    
    s = rota;
    
    return fo;
}

// ============================================================================
// [EXERCÍCIO] Inserção Mais Barata parcialmente gulosa (GRASP)
// ============================================================================
// ============================================================================
double constroiSolucaoParcialmenteGulosaInsercaoMaisBarata(const Instancia &inst,
                                                           std::vector<int> &s,
                                                           double alpha)
{
    int n = inst.n;
    double fo = 0;

    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    std::vector<int> rota;
    rota.reserve(n);
    rota.push_back(0);

    int cidadeAtual = rota[0];
    for(size_t i = 0; i < 3; i++){
        int indexProximaCidade = 0;
        double menorDist = __DBL_MAX__;
        double distancia = 0;
        for(size_t j = 0; j < naoVisitadas.size(); j++){
        //itera pelas cidades não visitadas procurando a menor distância
            distancia = inst.distancia(cidadeAtual, naoVisitadas[j]);
            if(distancia < menorDist){
                menorDist = distancia;
                indexProximaCidade = j;
            }
        }
        cidadeAtual = naoVisitadas[indexProximaCidade]; //visita a cidade de menor distância
        naoVisitadas.erase(naoVisitadas.begin() + indexProximaCidade);
        
        rota.push_back(cidadeAtual); //adiciona na rota

        fo += menorDist;
        
    }

    fo += inst.distancia(cidadeAtual, rota[0]);
    //TODO

    


    return fo;
}
