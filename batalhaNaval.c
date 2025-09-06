#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define TAMANHO_HABILIDADE 7  // As habilidades ocupam uma área 7x7
#define CENTRO_HABILIDADE 3   // O centro fica na posição 3 (meio da matriz 7x7)

// Defino valores para representar cada elemento no tabuleiro
#define AGUA 0
#define NAVIO 3
#define AREA_HABILIDADE 5

// Os navios podem ser posicionados em 4 direções diferentes
typedef enum {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_PRINCIPAL,
    DIAGONAL_SECUNDARIA
} Orientacao;

// Existem 3 tipos de habilidades especiais no jogo
typedef enum {
    CONE,
    CRUZ,
    OCTAEDRO
} TipoHabilidade;

// Esta função deixa todo o tabuleiro com água no início
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Cria o padrão da habilidade CONE - é como um triângulo que vai crescendo para baixo
void criarHabilidadeCone(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Primeiro limpo tudo com zero
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz[i][j] = 0;
        }
    }
    
    // Agora crio o formato de cone - cada linha fica mais larga que a anterior
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int distanciaCentro = abs(j - CENTRO_HABILIDADE);
            
            // Se estou na linha i, o cone tem largura i+1 a partir do centro
            if (i <= CENTRO_HABILIDADE && distanciaCentro <= i) {
                matriz[i][j] = 1;
            }
        }
    }
}

// Cria o padrão da habilidade CRUZ - uma linha vertical e horizontal se cruzando
void criarHabilidadeCruz(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz[i][j] = 0;
        }
    }
    
    // A cruz é formada pela linha do meio (vertical) e coluna do meio (horizontal)
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (i == CENTRO_HABILIDADE || j == CENTRO_HABILIDADE) {
                matriz[i][j] = 1;
            }
        }
    }
}

// Cria o padrão da habilidade OCTAEDRO - fica parecendo um losango
void criarHabilidadeOctaedro(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz[i][j] = 0;
        }
    }
    
    // Para fazer o losango, uso a distância Manhattan (soma das distâncias)
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int distanciaLinhas = abs(i - CENTRO_HABILIDADE);
            int distanciaColunas = abs(j - CENTRO_HABILIDADE);
            int distanciaManhattan = distanciaLinhas + distanciaColunas;
            
            // Se a distância total for menor que o raio, faz parte do losango
            if (distanciaManhattan <= CENTRO_HABILIDADE) {
                matriz[i][j] = 1;
            }
        }
    }
}

// Função para converter o tipo da habilidade em texto
const char* habilidadeParaString(TipoHabilidade tipo) {
    switch (tipo) {
        case CONE: return "Cone";
        case CRUZ: return "Cruz";
        case OCTAEDRO: return "Octaedro";
        default: return "Desconhecida";
    }
}

// Esta é a função principal que aplica a habilidade no tabuleiro
void aplicarHabilidadeAoTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                 int matrizHabilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE],
                                 int origemLinha, int origemColuna, TipoHabilidade tipo) {
    
    printf("Aplicando habilidade %s no ponto (%d, %d)...\n", 
           habilidadeParaString(tipo), origemLinha, origemColuna);
    
    int posicionesAfetadas = 0;
    
    // Percorro cada posição da matriz de habilidade
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Se esta posição da habilidade está ativa (valor 1)
            if (matrizHabilidade[i][j] == 1) {
                // Calculo onde essa posição vai ficar no tabuleiro principal
                int tabuleiroLinha = origemLinha + (i - CENTRO_HABILIDADE);
                int tabuleiroColuna = origemColuna + (j - CENTRO_HABILIDADE);
                
                // Verifico se não vai sair dos limites do tabuleiro
                if (tabuleiroLinha >= 0 && tabuleiroLinha < TAMANHO_TABULEIRO &&
                    tabuleiroColuna >= 0 && tabuleiroColuna < TAMANHO_TABULEIRO) {
                    
                    // Só aplico o efeito se não tiver navio ali (não quero destruir navios)
                    if (tabuleiro[tabuleiroLinha][tabuleiroColuna] != NAVIO) {
                        tabuleiro[tabuleiroLinha][tabuleiroColuna] = AREA_HABILIDADE;
                        posicionesAfetadas++;
                    }
                }
            }
        }
    }
    
    printf("Habilidade %s aplicada com sucesso! Posições afetadas: %d\n", 
           habilidadeParaString(tipo), posicionesAfetadas);
}

// Função para mostrar como fica a matriz de cada habilidade
void exibirMatrizHabilidade(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], TipoHabilidade tipo) {
    printf("\n=== MATRIZ DE HABILIDADE: %s ===\n", habilidadeParaString(tipo));
    printf("Tamanho: %dx%d (1 = área afetada, 0 = não afetada)\n", TAMANHO_HABILIDADE, TAMANHO_HABILIDADE);
    
    // Cabeçalho das colunas
    printf("   ");
    for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
        printf(" %d ", j);
    }
    printf("\n");
    
    // Linha separadora
    printf("   ");
    for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
        printf("---");
    }
    printf("\n");
    
    // Exibir a matriz linha por linha
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        printf("%d |", i);
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            printf(" %d ", matriz[i][j]);
        }
        printf("|\n");
    }
    
    printf("   ");
    for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
        printf("---");
    }
    printf("\n");
}

// Verifica se o navio cabe no tabuleiro sem sair das bordas
bool validarCoordenadas(int linha, int coluna, Orientacao orientacao) {
    if (linha < 0 || linha >= TAMANHO_TABULEIRO || coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
        return false;
    }
    
    // Cada orientação tem suas próprias regras de limite
    switch (orientacao) {
        case HORIZONTAL:
            return (coluna + TAMANHO_NAVIO <= TAMANHO_TABULEIRO);
        case VERTICAL:
            return (linha + TAMANHO_NAVIO <= TAMANHO_TABULEIRO);
        case DIAGONAL_PRINCIPAL:
            return (linha + TAMANHO_NAVIO <= TAMANHO_TABULEIRO && 
                    coluna + TAMANHO_NAVIO <= TAMANHO_TABULEIRO);
        case DIAGONAL_SECUNDARIA:
            return (linha + TAMANHO_NAVIO <= TAMANHO_TABULEIRO && 
                    coluna - TAMANHO_NAVIO + 1 >= 0);
        default:
            return false;
    }
}

// Verifica se o novo navio não vai ocupar o mesmo lugar de outro
bool verificarSobreposicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                          int linha, int coluna, Orientacao orientacao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int lin, col;
        
        // Calculo cada posição que o navio vai ocupar
        switch (orientacao) {
            case HORIZONTAL:
                lin = linha;
                col = coluna + i;
                break;
            case VERTICAL:
                lin = linha + i;
                col = coluna;
                break;
            case DIAGONAL_PRINCIPAL:
                lin = linha + i;
                col = coluna + i;
                break;
            case DIAGONAL_SECUNDARIA:
                lin = linha + i;
                col = coluna - i;
                break;
            default:
                return false;
        }
        
        // Se já tem algo diferente de água, não pode posicionar
        if (tabuleiro[lin][col] != AGUA) {
            return false;
        }
    }
    return true;
}

// Função principal que coloca o navio no tabuleiro
bool posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                     int linha, int coluna, Orientacao orientacao, int numeroNavio) {
    if (!validarCoordenadas(linha, coluna, orientacao)) {
        printf("Erro: Coordenadas inválidas para o Navio %d!\n", numeroNavio);
        return false;
    }
    
    if (!verificarSobreposicao(tabuleiro, linha, coluna, orientacao)) {
        printf("Erro: Navio %d se sobrepõe a outro navio existente!\n", numeroNavio);
        return false;
    }
    
    // Se passou nas verificações, coloco o navio no tabuleiro
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int lin, col;
        
        switch (orientacao) {
            case HORIZONTAL:
                lin = linha;
                col = coluna + i;
                break;
            case VERTICAL:
                lin = linha + i;
                col = coluna;
                break;
            case DIAGONAL_PRINCIPAL:
                lin = linha + i;
                col = coluna + i;
                break;
            case DIAGONAL_SECUNDARIA:
                lin = linha + i;
                col = coluna - i;
                break;
        }
        
        tabuleiro[lin][col] = NAVIO;
    }
    
    return true;
}

// Função que mostra o tabuleiro completo na tela
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n=== TABULEIRO DE BATALHA NAVAL COM HABILIDADES ===\n");
    
    // Cabeçalho com números das colunas
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf(" %d ", j);
    }
    printf("\n");
    
    // Linha separadora superior
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("---");
    }
    printf("\n");
    
    // Mostro cada linha do tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Aqui eu uso símbolos diferentes para cada tipo de elemento
            char simbolo;
            switch (tabuleiro[i][j]) {
                case AGUA: simbolo = '0'; break;            
                case NAVIO: simbolo = '3'; break;           
                case AREA_HABILIDADE: simbolo = '1'; break; 
                default: simbolo = '?'; break;              
            }
            printf(" %c ", simbolo);
        }
        printf("|\n");
    }
    
    // Linha separadora inferior
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("---");
    }
    printf("\n");
    
    printf("\nLegenda: 0 = Água, 3 = Navio, 1 = Área de Habilidade\n");
}

int main() {
    // Declaro as matrizes que vou usar no programa
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int matrizCone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int matrizCruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int matrizOctaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    
    inicializarTabuleiro(tabuleiro);
    
    printf("=== BATALHA NAVAL - HABILIDADES ESPECIAIS E ÁREAS DE EFEITO ===\n");
    printf("Tabuleiro: %dx%d\n", TAMANHO_TABULEIRO, TAMANHO_TABULEIRO);
    printf("Tamanho das habilidades: %dx%d\n", TAMANHO_HABILIDADE, TAMANHO_HABILIDADE);
    
    // FASE 1: Coloco os navios nas posições escolhidas
    printf("\n### FASE 1: POSICIONAMENTO DE NAVIOS ###\n");
    
    // Defino onde cada navio vai ficar (posição e orientação)
    struct {
        int linha, coluna;
        Orientacao orientacao;
        const char* descricao;
    } navios[] = {
        {1, 1, HORIZONTAL, "Horizontal"},
        {7, 2, VERTICAL, "Vertical"},
        {1, 6, DIAGONAL_PRINCIPAL, "Diagonal Principal"},
        {6, 8, DIAGONAL_SECUNDARIA, "Diagonal Secundária"}
    };
    
    int naviosValidos = 0;
    for (int i = 0; i < 4; i++) {
        printf("\nPosicionando Navio %d (%s) na posição (%d,%d)...\n", 
               i + 1, navios[i].descricao, navios[i].linha, navios[i].coluna);
        
        if (posicionarNavio(tabuleiro, navios[i].linha, navios[i].coluna, 
                           navios[i].orientacao, i + 1)) {
            printf("✓ Navio %d posicionado com sucesso!\n", i + 1);
            naviosValidos++;
        } else {
            printf("✗ Falha ao posicionar o Navio %d!\n", i + 1);
        }
    }
    
    // FASE 2: Crio as matrizes das três habilidades
    printf("\n### FASE 2: CRIAÇÃO DAS MATRIZES DE HABILIDADES ###\n");
    
    printf("\nCriando habilidade CONE...\n");
    criarHabilidadeCone(matrizCone);
    
    printf("Criando habilidade CRUZ...\n");
    criarHabilidadeCruz(matrizCruz);
    
    printf("Criando habilidade OCTAEDRO...\n");
    criarHabilidadeOctaedro(matrizOctaedro);
    
    // Mostro como ficou cada matriz de habilidade
    exibirMatrizHabilidade(matrizCone, CONE);
    exibirMatrizHabilidade(matrizCruz, CRUZ);
    exibirMatrizHabilidade(matrizOctaedro, OCTAEDRO);
    
    // FASE 3: Aplico as habilidades no tabuleiro principal
    printf("\n### FASE 3: APLICAÇÃO DAS HABILIDADES AO TABULEIRO ###\n");
    
    // Escolho onde aplicar cada habilidade
    struct {
        int linha, coluna;
        TipoHabilidade tipo;
        int (*matriz)[TAMANHO_HABILIDADE];
    } habilidades[] = {
        {2, 4, CONE, matrizCone},
        {6, 1, CRUZ, matrizCruz},
        {7, 6, OCTAEDRO, matrizOctaedro}
    };
    
    for (int i = 0; i < 3; i++) {
        printf("\n");
        aplicarHabilidadeAoTabuleiro(tabuleiro, habilidades[i].matriz,
                                   habilidades[i].linha, habilidades[i].coluna,
                                   habilidades[i].tipo);
    }
    
    // FASE 4: Mostro o resultado final
    printf("\n### RESULTADO FINAL ###\n");
    exibirTabuleiro(tabuleiro);
    
    // Conto quantos elementos de cada tipo ficaram no tabuleiro
    int contadores[6] = {0}; 
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] >= 0 && tabuleiro[i][j] < 6) {
                contadores[tabuleiro[i][j]]++;
            }
        }
    }
    
    printf("\n=== ESTATÍSTICAS FINAIS ===\n");
    printf("Navios posicionados: %d/4\n", naviosValidos);
    printf("Posições com água: %d\n", contadores[AGUA]);
    printf("Posições com navios: %d\n", contadores[NAVIO]);
    printf("Posições afetadas por habilidades: %d\n", contadores[AREA_HABILIDADE]);
    printf("Total de posições: %d\n", TAMANHO_TABULEIRO * TAMANHO_TABULEIRO);
    
    printf("\nHabilidades aplicadas com sucesso!\n");
    printf("Tabuleiro pronto para batalha com áreas de efeito especiais!\n");
    
    return 0;
}

// - Esse código foi feito até o "Nível Mestre"!