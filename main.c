#include <stdio.h>
#include <stdlib.h>

typedef enum {  //Possiveis direcoes para seguir
    CIMA,
    DIREITA,
    BAIXO,
    ESQUERDA
}DIRECOES;
enum POSSIBILIDADES {   //Facilitador de leitura para alguns retornos de possibilidade
    NAO,
    SIM
};
enum ERROR_TYPE{
    ERROR_OPEN_FILE_READ,
    ERROR_ALLOC_VARIABLE,
    ERROR_FIND_PATH
};

typedef struct {
    int M, N;
    int X, Y;
    int currentX, currentY;
    int nCaminhos, nPessoas, nCaminhosVisitados;
    double percentExploracao;
    FILE *f;
    char *nome_arquivo;
    char **labirinto;
}_LABIRINTO;

char *lerEntrada(FILE *ptr);
char **lerLabirinto(_LABIRINTO *l);;
int checarParadaLinha(char c);
int caminhoPossivel(_LABIRINTO *p, int x, int y, DIRECOES dir);
int escapou(_LABIRINTO *p, int x, int y);
void liberarLabirinto (_LABIRINTO *p);;
void contarCaminhos(_LABIRINTO *p);
void contarPessoas(_LABIRINTO *p);
void contarCaminhosVisitados(_LABIRINTO *p);
void acharCaminho (_LABIRINTO *p, int x, int y);
void imprimirLabirinto(_LABIRINTO *p);
FILE *abrirArquivoLeitura(char *nomeArq, char* modoAbertura);

int main(void) {
    _LABIRINTO *ptr;
    ptr = (_LABIRINTO *)calloc(sizeof(_LABIRINTO), 1 * sizeof(_LABIRINTO));

    ptr->nome_arquivo = lerEntrada(stdin);
    ptr->f = abrirArquivoLeitura(ptr->nome_arquivo, "r");
    fscanf(ptr->f, "%d %d", &ptr->M, &ptr->N);
    fscanf(ptr->f, "%d %d", &ptr->X, &ptr->Y);

    ptr->labirinto = lerLabirinto(ptr);

    contarCaminhos(ptr);
    contarPessoas(ptr);

    acharCaminho(ptr, ptr->X, ptr->Y);

    contarCaminhosVisitados(ptr);
    imprimirLabirinto(ptr);
    ptr->percentExploracao =  (double)(ptr->nCaminhosVisitados * 100 /ptr->nCaminhos);

    printf("\nVoce escapou de todos! Ninguem conseguiu te segurar!\n");
    printf("Veja abaixo os detalhes da sua fuga:\n");
    printf("----Pessoas te procurando: %d\n", ptr->nPessoas);
    printf("----Numero total de caminhos validos:   %d\n", ptr->nCaminhos);
    printf("----Numero total de caminhos visitados: %d\n", ptr->nCaminhosVisitados);
    printf("----Exploracao total do labirinto: %.1lf%%\n", ptr->percentExploracao);

    liberarLabirinto(ptr);
    return 0;
}

int checarParadaLinha(char c) {
    return (c != '\n' &&  c != '\r' && c != '\0' && c != EOF);
}

char *lerEntrada(FILE *ptr) {
    char c, *line = NULL;
    int count;
    for(count = 0, c = getc(ptr); checarParadaLinha(c) ; count++) {
        line = (char *)realloc(line, (count + 1) * sizeof(char));
        line[count] = c;
        c = getc(ptr);
    }
    line = (char *)realloc(line, (count + 1) * sizeof(char));
    line[count] = '\0';
    return line;
}

FILE *abrirArquivoLeitura(char *nomeArq, char* modoAbertura) {
    FILE *ptr;
    ptr = fopen(nomeArq, modoAbertura);
    if(ptr == NULL) {
        printf("File <%s> could not be opened on %s", nomeArq, modoAbertura);
        exit(ERROR_OPEN_FILE_READ);
    }
    return ptr;
}
char **lerLabirinto(_LABIRINTO *l) {
    char **ptr = NULL;
    int i, j;
    ptr = (char **)calloc(sizeof(char *), l->M * sizeof(char *));
    if(ptr == NULL) exit(ERROR_ALLOC_VARIABLE);
    for(i = 0; i < l->M; i++) {
        ptr[i] = (char *)calloc(sizeof(char), l->N * sizeof(char));
        if(ptr[i] == NULL) exit(ERROR_ALLOC_VARIABLE);
        getc(l->f);
        for(j = 0; j < l->N; j++) {
            ptr[i][j] = getc(l->f);
        }
    }
    return ptr;
}
void liberarLabirinto (_LABIRINTO *p) {
    fclose(p->f);
    free(p->nome_arquivo);
    for(int i = 0; i < p->M; i++) {
        free(p->labirinto[i]);
    }
    free(p->labirinto);
    free(p);
}
void contarCaminhos(_LABIRINTO *p) {
    p->nCaminhos = 0;
    int i, j;
    for(i = 0; i < p->M; i++) {
        for(j = 0; j < p->N; j++) {
            if (p->labirinto[i][j] == '.'){
                p->nCaminhos++;
            }
        }
    }
}
void contarCaminhosVisitados(_LABIRINTO *p) {
    p->nCaminhosVisitados = 0;
    int i, j;
    for(i = 0; i < p->M; i++) {
        for(j = 0; j < p->N; j++) {
            if (p->labirinto[i][j] == '*'){
                p->nCaminhosVisitados++;
            }
        }
    }
}
void contarPessoas(_LABIRINTO *p) {
    p->nPessoas = 0;
    int i, j;
    for(i = 0; i < p->M; i++) {
        for(j = 0; j < p->N; j++) {
            if (p->labirinto[i][j] == '#'){
                p->nPessoas++;
            }
        }
    }
}
/*
    Me permite checar se a direcao dir é possivel de se prosseguir
*/
int caminhoPossivel(_LABIRINTO *p, int x, int y, DIRECOES dir) {
    if(dir == CIMA){
        if(x != 0 && p->labirinto[x-1][y] != '#' && p->labirinto[x-1][y] != '*')
            return SIM;
        else
            return NAO;
    } else if(dir == DIREITA){
        if(y != (p->N - 1) && p->labirinto[x][y+1] != '#' && p->labirinto[x][y+1] != '*')
            return SIM;
        else
            return NAO;
    } else if(dir == BAIXO){
        if(x != (p->M - 1) && p->labirinto[x+1][y] != '#' && p->labirinto[x+1][y] != '*')
            return SIM;
        else
            return NAO;
    } else if(dir == ESQUERDA){
        if(y != 0 && p->labirinto[x][y-1] != '#' && p->labirinto[x][y-1] != '*')
            return SIM;
        else
            return NAO;
    } else {
        printf("Erro ao checar caminho possivel");
        exit(ERROR_FIND_PATH);
    }
}
/*
    Checa se o usuario atingiu um ponto de saida do labirinto
*/
int escapou(_LABIRINTO *p, int x, int y) {
    if(x == 0  || x == p->M - 1)
        return 1;
    else if(y == 0  || y == p->N - 1)
        return 1;
    else 
        return 0;
}
/* 
    Algoritmo recursivo para checar se o ponteiro ja saiu do labirinto ou se ele ficou empacado
*/
void acharCaminho (_LABIRINTO *p, int x, int y) {
    p->currentX = x;
    p->currentY = y;
    p->labirinto[x][y] = '*';    
    if (!escapou(p, p->currentX, p->currentY)) {
        if(caminhoPossivel(p, x, y, CIMA) && !escapou(p, p->currentX, p->currentY)) {
            acharCaminho(p, x-1, y);
        }
        if(caminhoPossivel(p, x, y, DIREITA) && !escapou(p, p->currentX, p->currentY)) {
            acharCaminho(p, x, y+1);
        }
        if(caminhoPossivel(p, x, y, BAIXO) && !escapou(p, p->currentX, p->currentY)) {
            acharCaminho(p, x+1, y);
        }
        if(caminhoPossivel(p, x, y, ESQUERDA) && !escapou(p, p->currentX, p->currentY)) {
            acharCaminho(p, x, y-1);
        }
    }
}
void imprimirLabirinto(_LABIRINTO *p) {
    int i, j;
    for(i = 0; i < p->M; i++) {
        for(j = 0; j < p->N; j++) {
            printf("%c", p->labirinto[i][j]);
        }
        printf("\n");
    }
}