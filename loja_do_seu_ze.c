#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define ARQUIVO "produtos.txt"
#define TEMPORARIO "temp.txt"
#define CSV "produtos.csv"
#define TRACOS 96

#define W_ID 3
#define W_PRODUTO 24
#define W_CATEGORIA 16
#define W_PRECO 10
#define W_QTD 10
#define W_FORNECEDOR 14

typedef struct {
    int id;
    char nome[50];
    char categoria[30];
    float preco;
    int quantidade;
    char fornecedor[30];
} Produto;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler_texto(char *destino, int tamanho) {
    do {
        if (fgets(destino, tamanho, stdin) == NULL) {
            exit(0);
        }
        destino[strcspn(destino, "\r\n")] = '\0';
        for (int i = 0; destino[i] != '\0'; i++) {
            if (destino[i] == ';' || destino[i] == ',' || destino[i] == '|') {
                destino[i] = ' ';
            }
        }
        if (strlen(destino) == 0) {
            printf("O campo não pode ficar vazio. Digite novamente: ");
        }
    } while (strlen(destino) == 0);
}

int ler_inteiro() {
    int numero;
    int lidos = scanf("%d", &numero);
    if (lidos == EOF) {
        exit(0);
    }
    if (lidos != 1) {
        numero = -1;
    }
    limpar_buffer();
    return numero;
}

int ler_quantidade() {
    int q = ler_inteiro();
    while (q < 0) {
        printf("Quantidade inválida. Digite novamente: ");
        q = ler_inteiro();
    }
    return q;
}

float ler_preco() {
    float preco;
    while (scanf("%f", &preco) != 1 || preco < 0) {
        limpar_buffer();
        printf("Preço inválido. Digite novamente: ");
    }
    limpar_buffer();
    return preco;
}

void minusculo(char *destino, char *origem) {
    int i;
    for (i = 0; origem[i] != '\0'; i++) {
        destino[i] = tolower((unsigned char)origem[i]);
    }
    destino[i] = '\0';
}

void tirar_espacos(char *texto) {
    int inicio = 0;
    int fim = strlen(texto) - 1;
    int j = 0;
    while (texto[inicio] == ' ') {
        inicio++;
    }
    while (fim >= inicio && texto[fim] == ' ') {
        fim--;
    }
    for (int i = inicio; i <= fim; i++) {
        texto[j] = texto[i];
        j++;
    }
    texto[j] = '\0';
}

void copiar_campo(char *destino, char *origem, int tamanho) {
    tirar_espacos(origem);
    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

void escrever_traco(FILE *saida) {
    for (int i = 0; i < TRACOS; i++) {
        fprintf(saida, "-");
    }
    fprintf(saida, "\n");
}

void escrever_campo(FILE *saida, char *texto, int largura) {
    int letras = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if ((texto[i] & 0xC0) != 0x80) {
            letras++;
        }
    }
    fprintf(saida, " %s", texto);
    for (int i = letras; i < largura; i++) {
        fprintf(saida, " ");
    }
    fprintf(saida, " |");
}

void escrever_tabela(FILE *saida, Produto *lista, int total) {
    char texto[20];
    escrever_traco(saida);
    fprintf(saida, "|");
    escrever_campo(saida, "ID", W_ID);
    escrever_campo(saida, "Produto", W_PRODUTO);
    escrever_campo(saida, "Categoria", W_CATEGORIA);
    escrever_campo(saida, "Preço (R$)", W_PRECO);
    escrever_campo(saida, "Quantidade", W_QTD);
    escrever_campo(saida, "Fornecedor", W_FORNECEDOR);
    fprintf(saida, "\n");
    escrever_traco(saida);
    for (int i = 0; i < total; i++) {
        fprintf(saida, "|");
        sprintf(texto, "%d", lista[i].id);
        escrever_campo(saida, texto, W_ID);
        escrever_campo(saida, lista[i].nome, W_PRODUTO);
        escrever_campo(saida, lista[i].categoria, W_CATEGORIA);
        sprintf(texto, "%.2f", lista[i].preco);
        escrever_campo(saida, texto, W_PRECO);
        sprintf(texto, "%d", lista[i].quantidade);
        escrever_campo(saida, texto, W_QTD);
        escrever_campo(saida, lista[i].fornecedor, W_FORNECEDOR);
        fprintf(saida, "\n");
    }
    escrever_traco(saida);
}

int carregar(Produto *lista) {
    FILE *arq = fopen(ARQUIVO, "r");
    char linha[300];
    int total = 0;
    if (arq == NULL) {
        return 0;
    }
    while (total < MAX && fgets(linha, sizeof(linha), arq) != NULL) {
        if (linha[0] != '|') {
            continue;
        }
        char *id = strtok(linha, "|");
        char *nome = strtok(NULL, "|");
        char *categoria = strtok(NULL, "|");
        char *preco = strtok(NULL, "|");
        char *qtd = strtok(NULL, "|");
        char *fornecedor = strtok(NULL, "|");
        if (id != NULL && nome != NULL && categoria != NULL && preco != NULL
            && qtd != NULL && fornecedor != NULL && atoi(id) > 0) {
            lista[total].id = atoi(id);
            copiar_campo(lista[total].nome, nome, sizeof(lista[total].nome));
            copiar_campo(lista[total].categoria, categoria, sizeof(lista[total].categoria));
            lista[total].preco = atof(preco);
            lista[total].quantidade = atoi(qtd);
            copiar_campo(lista[total].fornecedor, fornecedor, sizeof(lista[total].fornecedor));
            total++;
        }
    }
    fclose(arq);
    return total;
}

int gravar(Produto *lista, int total) {
    FILE *temp = fopen(TEMPORARIO, "w");
    if (temp == NULL) {
        perror("Erro ao criar o arquivo temporário");
        return 0;
    }
    escrever_tabela(temp, lista, total);
    fclose(temp);
    remove(ARQUIVO);
    if (rename(TEMPORARIO, ARQUIVO) != 0) {
        perror("Erro ao substituir o arquivo");
        return 0;
    }
    return 1;
}

int gerar_id(Produto *lista, int total) {
    int maior = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id > maior) {
            maior = lista[i].id;
        }
    }
    return maior + 1;
}

int id_existe(Produto *lista, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) {
            return 1;
        }
    }
    return 0;
}

int nome_existe(Produto *lista, int total, char *nome, int ignorar_id) {
    char a[50], b[50];
    minusculo(a, nome);
    for (int i = 0; i < total; i++) {
        minusculo(b, lista[i].nome);
        if (strcmp(a, b) == 0 && lista[i].id != ignorar_id) {
            return 1;
        }
    }
    return 0;
}

int buscar_posicao(Produto *lista, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) {
            return i;
        }
    }
    return -1;
}

void listar(Produto *lista, int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\n");
    escrever_tabela(stdout, lista, total);
}

void criar_arquivo() {
    Produto lista[MAX];
    FILE *arq = fopen(ARQUIVO, "r");
    if (arq != NULL) {
        fclose(arq);
        printf("O arquivo %s já existe.\n", ARQUIVO);
        return;
    }
    if (gravar(lista, 0)) {
        printf("Arquivo %s criado com sucesso!\n", ARQUIVO);
    }
}

void inserir() {
    Produto lista[MAX];
    int total = carregar(lista);
    Produto novo;
    Produto *p = &novo;

    if (total >= MAX) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    p->id = gerar_id(lista, total);
    if (id_existe(lista, total, p->id)) {
        printf("ID duplicado!\n");
        return;
    }

    printf("Produto: ");
    ler_texto(p->nome, sizeof(p->nome));
    if (nome_existe(lista, total, p->nome, 0)) {
        printf("Já existe um produto com esse nome!\n");
        return;
    }
    printf("Categoria: ");
    ler_texto(p->categoria, sizeof(p->categoria));
    printf("Preço (R$): ");
    p->preco = ler_preco();
    printf("Quantidade: ");
    p->quantidade = ler_quantidade();
    printf("Fornecedor: ");
    ler_texto(p->fornecedor, sizeof(p->fornecedor));

    lista[total] = novo;
    total++;
    if (gravar(lista, total)) {
        printf("Produto cadastrado com sucesso! (ID %d)\n", p->id);
    }
}

void alterar() {
    Produto lista[MAX];
    int total = carregar(lista);
    char nome[50];

    listar(lista, total);
    if (total == 0) {
        return;
    }

    printf("ID do produto a alterar: ");
    int id = ler_inteiro();
    int pos = buscar_posicao(lista, total, id);
    if (pos == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    Produto *p = &lista[pos];
    printf("Novo nome: ");
    ler_texto(nome, sizeof(nome));
    if (nome_existe(lista, total, nome, p->id)) {
        printf("Já existe um produto com esse nome!\n");
        return;
    }
    strcpy(p->nome, nome);
    printf("Nova categoria: ");
    ler_texto(p->categoria, sizeof(p->categoria));
    printf("Novo preço (R$): ");
    p->preco = ler_preco();
    printf("Nova quantidade: ");
    p->quantidade = ler_quantidade();
    printf("Novo fornecedor: ");
    ler_texto(p->fornecedor, sizeof(p->fornecedor));

    if (gravar(lista, total)) {
        printf("Produto alterado com sucesso!\n");
    }
}

void excluir() {
    Produto lista[MAX];
    int total = carregar(lista);

    listar(lista, total);
    if (total == 0) {
        return;
    }

    printf("ID do produto a excluir: ");
    int id = ler_inteiro();
    int pos = buscar_posicao(lista, total, id);
    if (pos == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    for (int i = pos; i < total - 1; i++) {
        lista[i] = lista[i + 1];
    }
    total--;

    if (gravar(lista, total)) {
        printf("Produto excluído com sucesso!\n");
    }
}

void buscar_nome() {
    Produto lista[MAX];
    Produto achados[MAX];
    int total = carregar(lista);
    int n = 0;
    char busca[50], nome[50];

    printf("Nome (ou parte dele): ");
    ler_texto(busca, sizeof(busca));
    minusculo(busca, busca);

    for (int i = 0; i < total; i++) {
        minusculo(nome, lista[i].nome);
        if (strstr(nome, busca) != NULL) {
            achados[n] = lista[i];
            n++;
        }
    }
    listar(achados, n);
}

void buscar_categoria() {
    Produto lista[MAX];
    Produto achados[MAX];
    int total = carregar(lista);
    int n = 0;
    char busca[30], categoria[30];

    printf("Categoria (ou parte dela): ");
    ler_texto(busca, sizeof(busca));
    minusculo(busca, busca);

    for (int i = 0; i < total; i++) {
        minusculo(categoria, lista[i].categoria);
        if (strstr(categoria, busca) != NULL) {
            achados[n] = lista[i];
            n++;
        }
    }
    listar(achados, n);
}

int deve_trocar(Produto *a, Produto *b, int criterio) {
    char x[50], y[50];
    minusculo(x, a->nome);
    minusculo(y, b->nome);
    switch (criterio) {
        case 1: return strcmp(x, y) > 0;
        case 2: return strcmp(x, y) < 0;
        case 3: return a->preco > b->preco;
        case 4: return a->preco < b->preco;
    }
    return 0;
}

void ordenar(Produto *lista, int total, int criterio) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (deve_trocar(&lista[j], &lista[j + 1], criterio)) {
                Produto aux = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
        }
    }
    listar(lista, total);
}

void exportar_csv() {
    Produto lista[MAX];
    int total = carregar(lista);
    FILE *csv = fopen(CSV, "w");
    if (csv == NULL) {
        perror("Erro ao criar o CSV");
        return;
    }
    fprintf(csv, "id,produto,categoria,preco,quantidade,fornecedor\n");
    for (int i = 0; i < total; i++) {
        fprintf(csv, "%d,%s,%s,%.2f,%d,%s\n", lista[i].id, lista[i].nome,
                lista[i].categoria, lista[i].preco, lista[i].quantidade,
                lista[i].fornecedor);
    }
    fclose(csv);
    printf("Dados exportados para %s\n", CSV);
}

int main() {
    Produto lista[MAX];
    int total;
    int opcao;

    do {
        printf("\n===== Loja do Seu Zé =====\n");
        printf("1  - Criar arquivo\n");
        printf("2  - Listar produtos\n");
        printf("3  - Inserir produto\n");
        printf("4  - Alterar produto\n");
        printf("5  - Excluir produto\n");
        printf("6  - Buscar por nome\n");
        printf("7  - Buscar por categoria\n");
        printf("8  - Ordenar por nome (A-Z)\n");
        printf("9  - Ordenar por nome (Z-A)\n");
        printf("10 - Ordenar por preço (menor -> maior)\n");
        printf("11 - Ordenar por preço (maior -> menor)\n");
        printf("12 - Exportar para CSV\n");
        printf("0  - Sair\n");
        printf("Opção: ");
        opcao = ler_inteiro();

        switch (opcao) {
            case 1:  criar_arquivo(); break;
            case 2:
                total = carregar(lista);
                listar(lista, total);
                break;
            case 3:  inserir(); break;
            case 4:  alterar(); break;
            case 5:  excluir(); break;
            case 6:  buscar_nome(); break;
            case 7:  buscar_categoria(); break;
            case 8:
                total = carregar(lista);
                ordenar(lista, total, 1);
                break;
            case 9:
                total = carregar(lista);
                ordenar(lista, total, 2);
                break;
            case 10:
                total = carregar(lista);
                ordenar(lista, total, 3);
                break;
            case 11:
                total = carregar(lista);
                ordenar(lista, total, 4);
                break;
            case 12: exportar_csv(); break;
            case 0:  printf("Até logo!\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}