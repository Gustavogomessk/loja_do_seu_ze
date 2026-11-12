
#include <stdio.h> // Uso para exibir informações e trabalhar com arquivos.
#include <stdlib.h> // Uso para alocação e liberação de memória.
#include <string.h> // Uso para trabalhar com textos.
#include <ctype.h> // Uso para comparar letras sem diferenciar maiúsculas e minúsculas.

// Criei uma estrutura para guardar os dados de cada produto.
typedef struct {
    int id;           // Guardo o identificador único do produto.
    char nome[50];     // Guardo o nome do produto com limite de 50 caracteres.
    char categoria[50]; // Guardo a categoria do produto.
    float preco;     // Guardo o preço do produto.
} Produto;

// Criei uma função para ler textos e remover a quebra de linha.
void lerTexto(char texto[], int tamanho) {
    fgets(texto, tamanho, stdin); // Aqui ele lê o texto digitado pelo usuário.
    texto[strcspn(texto, "\n")] = '\0'; // Removo o ENTER do final do texto.
}

// Criei uma função para comparar dois textos sem diferenciar letras maiúsculas.
int compararTexto(char texto1[], char texto2[]) {
    int i = 0; // Começo a comparação pela primeira posição.

    while (texto1[i] != '\0' && texto2[i] != '\0') {
        // Converto as letras para minúsculas antes de comparar.
        char letra1 = tolower((unsigned char)texto1[i]);
        char letra2 = tolower((unsigned char)texto2[i]);

        if (letra1 != letra2) {
            return letra1 - letra2; // Retorno a diferença entre as letras.
        }

        i++; // Avanço para a próxima posição.
    }

    // Verifico se os textos terminaram na mesma posição.
    return tolower((unsigned char)texto1[i]) -
           tolower((unsigned char)texto2[i]);
}

// Criei uma função para verificar se um texto contém outro texto.
int contemTexto(char texto[], char busca[]) {
    int i;
    int j;

    // Se a busca estiver vazia, considero que ela foi encontrada.
    if (busca[0] == '\0') {
        return 1;
    }

    // Percorro o texto principal.
    for (i = 0; texto[i] != '\0'; i++) {
        j = 0; // Reinicio a posição do texto que estou procurando.

        // Comparo as letras até encontrar uma diferença.
        while (busca[j] != '\0' &&
               texto[i + j] != '\0' &&
               tolower((unsigned char)texto[i + j]) ==
               tolower((unsigned char)busca[j])) {
            j++; // Avanço enquanto as letras forem iguais.
        }

        // Se cheguei ao final da busca, encontrei o texto.
        if (busca[j] == '\0') {
            return 1;
        }
    }

    return 0; // Retorno zero quando não encontro o texto.
}

// Criei uma função para verificar se o nome já está cadastrado.
int nomeExiste(Produto lista[], int total, char nome[], int posicao) {
    int i;

    // Percorro todos os produtos cadastrados.
    for (i = 0; i < total; i++) {
        // Ignoro a posição atual quando estou alterando um produto.
        if (i != posicao &&
            compararTexto(lista[i].nome, nome) == 0) {
            return 1; // Retorno um se encontrar um nome repetido.
        }
    }

    return 0; // Retorno zero quando o nome ainda não existe.
}

// Criei uma função para verificar se um ID já está sendo utilizado.
int idExiste(Produto lista[], int total, int id) {
    int i;

    // Percorro a lista para procurar o identificador.
    for (i = 0; i < total; i++) {
        if (lista[i].id == id) {
            return 1; // Encontrei um ID que já está cadastrado.
        }
    }

    return 0; // O ID ainda não está sendo utilizado.
}

// Criei uma função para gerar IDs automaticamente.
int gerarID(Produto lista[], int total) {
    int maior = 0;
    int i;

    // Procuro o maior ID existente na lista.
    for (i = 0; i < total; i++) {
        if (lista[i].id > maior) {
            maior = lista[i].id;
        }
    }

    // Começo a gerar o próximo ID incremental.
    maior++;

    // Verifico se o novo ID já está sendo utilizado.
    while (idExiste(lista, total, maior)) {
        maior++; // Gero outro número caso exista duplicidade.
    }

    return maior; // Retorno o ID disponível.
}

// Criei uma função para salvar os dados em um arquivo temporário.
int salvarTemporario(Produto lista[], int total) {
    FILE *arquivo; // Crio um ponteiro para manipular o arquivo.
    int i;

    // Abro o arquivo temporário no modo de escrita.
    arquivo = fopen("produtos.tmp", "w");

    if (arquivo == NULL) {
        return 0; // Informo que não consegui criar o arquivo.
    }
    fprintf(arquivo, "========== LOJA DO SEU ZE ==========\n");
    fprintf(arquivo, "ID | NOME | CATEGORIA | PRECO\n"); // Adiciono o cabeçalho no arquivo.
    fprintf(arquivo, "-----------------------------------------------------------\n"); // Adiciono uma linha separadora.
    // Percorro todos os produtos cadastrados.
    for (i = 0; i < total; i++) {
        // Salvo os campos separados pelo caractere |.
        fprintf(arquivo, " %d | %s | %s | %.2f\n",
                lista[i].id,
                lista[i].nome,
                lista[i].categoria,
                lista[i].preco);
    }
    fprintf(arquivo, "\n -----------------------------------------------------------"); // Adiciono uma linha em branco no final.

    fclose(arquivo); // Fecho o arquivo depois de gravar os dados.

    // Removo o arquivo antigo antes de colocar o novo no lugar.
    remove("produtos.txt");

    // Renomeio o arquivo temporário para substituir o original.
    if (rename("produtos.tmp", "produtos.txt") != 0) {
        return 0; // Informo que houve erro na substituição.
    }

    return 1; // Informo que o salvamento foi realizado.
}

// Criei uma função para carregar os produtos quando o programa iniciar.
void carregarProdutos(Produto **lista, int *total) {
    FILE *arquivo;
    Produto produto;
    Produto *aux;

    // Abro o arquivo de produtos para leitura.
    arquivo = fopen("produtos.txt", "r");

    // Se o arquivo não existir, começo com a lista vazia.
    if (arquivo == NULL) {
        return;
    }

    // Leio os dados enquanto encontrar registros válidos.
    while (fscanf(arquivo, "%d|%49[^|]|%49[^|]|%f",
                  &produto.id,
                  produto.nome,
                  produto.categoria,
                  &produto.preco) == 4) {

        // Aumento o espaço de memória para guardar mais um produto.
        aux = realloc(*lista, (*total + 1) * sizeof(Produto));

        // Verifico se consegui reservar memória.
        if (aux == NULL) {
            break;
        }

        *lista = aux; // Atualizo o endereço da lista.
        (*lista)[*total] = produto; // Adiciono o produto ao vetor.
        (*total)++; // Aumento a quantidade de produtos.
    }

    fclose(arquivo); // Fecho o arquivo depois da leitura.
}

// Criei uma função para inserir um novo produto.
void inserirProduto(Produto **lista, int *total) {
    Produto novo;
    Produto *aux;

    printf("\nNome do produto: ");
    lerTexto(novo.nome, 50); // Leio o nome digitado.

    // Verifico se já existe um produto com esse nome.
    if (nomeExiste(*lista, *total, novo.nome, -1)) {
        printf("\nEsse produto ja esta cadastrado.\n");
        return;
    }

    printf("Categoria: ");
    lerTexto(novo.categoria, 50); // Leio a categoria.

    printf("Preco: ");
    scanf("%f", &novo.preco); // Leio o preço.
    while (getchar() != '\n'); // Limpo o ENTER do teclado.

    // Não permito cadastrar preços negativos.
    if (novo.preco < 0) {
        printf("\nPreco invalido.\n");
        return;
    }

    // Gero o ID automaticamente.
    novo.id = gerarID(*lista, *total);

    // Aumento o tamanho do vetor para receber o novo produto.
    aux = realloc(*lista, (*total + 1) * sizeof(Produto));

    if (aux == NULL) {
        printf("\nErro ao reservar memoria.\n");
        return;
    }

    *lista = aux; // Atualizo o endereço da lista.
    (*lista)[*total] = novo; // Adiciono o novo produto ao vetor.
    (*total)++; // Atualizo a quantidade de produtos.

    // Salvo os produtos no arquivo temporário e substituo o original.
    if (salvarTemporario(*lista, *total)) {
        printf("\nProduto cadastrado com sucesso! ID: %d\n", novo.id);
    } else {
        (*total)--; // Desfaço a inclusão na quantidade.
        printf("\nErro ao salvar o produto.\n");
    }
}

// Criei uma função para listar os produtos em formato de tabela.
void listarProdutos(Produto lista[], int total) {
    int i;

    // Verifico se existe algum produto cadastrado.
    if (total == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    // Exibo o cabeçalho da tabela.
    printf("\n%-8s %-25s %-20s %-10s\n",
           "ID", "NOME", "CATEGORIA", "PRECO");

    // Percorro o vetor e mostro os dados de cada produto.
    for (i = 0; i < total; i++) {
        printf("%-8d %-25s %-20s R$ %.2f\n",
               lista[i].id,
               lista[i].nome,
               lista[i].categoria,
               lista[i].preco);
    }
}

// Criei uma função para alterar os dados de um produto.
void alterarProduto(Produto lista[], int total) {
    int id;
    int i;
    Produto original; // Guardo os dados antigos para poder recuperar.

    printf("\nDigite o ID do produto: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    // Procuro o produto pelo ID informado.
    for (i = 0; i < total; i++) {
        if (lista[i].id == id) {

            original = lista[i]; // Faço uma cópia dos dados originais.

            printf("Novo nome: ");
            lerTexto(lista[i].nome, 50);

            // Verifico se o novo nome já pertence a outro produto.
            if (nomeExiste(lista, total, lista[i].nome, i)) {
                lista[i] = original; // Recupero os dados antigos.
                printf("\nEsse nome ja esta cadastrado.\n");
                return;
            }

            printf("Nova categoria: ");
            lerTexto(lista[i].categoria, 50);

            printf("Novo preco: ");
            scanf("%f", &lista[i].preco);
            while (getchar() != '\n');

            // Verifico se o preço informado é válido.
            if (lista[i].preco < 0) {
                lista[i] = original;
                printf("\nPreco invalido.\n");
                return;
            }

            // Atualizo o arquivo usando o arquivo temporário.
            if (salvarTemporario(lista, total)) {
                printf("\nProduto alterado com sucesso.\n");
            } else {
                lista[i] = original; // Recupero os dados antigos.
                printf("\nErro ao atualizar o arquivo.\n");
            }

            return;
        }
    }

    printf("\nProduto nao encontrado.\n");
}

// Criei uma função para excluir um produto pelo ID.
void excluirProduto(Produto lista[], int *total) {
    int id;
    int i;
    int j;
    Produto removido;

    printf("\nDigite o ID do produto: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    // Procuro o produto que será excluído.
    for (i = 0; i < *total; i++) {
        if (lista[i].id == id) {

            removido = lista[i]; // Guardo uma cópia do produto.

            // Desloco os produtos seguintes uma posição para trás.
            for (j = i; j < *total - 1; j++) {
                lista[j] = lista[j + 1];
            }

            (*total)--; // Diminuo a quantidade de produtos.

            // Salvo a lista atualizada no arquivo temporário.
            if (salvarTemporario(lista, *total)) {
                printf("\nProduto excluido com sucesso.\n");
            } else {
                // Recupero os elementos caso o salvamento falhe.
                for (j = *total; j > i; j--) {
                    lista[j] = lista[j - 1];
                }

                lista[i] = removido;
                (*total)++;

                printf("\nErro ao excluir o produto.\n");
            }

            return;
        }
    }

    printf("\nProduto nao encontrado.\n");
}

// Criei uma função para buscar produtos pelo nome.
void buscarProduto(Produto lista[], int total) {
    char nome[50];
    int i;
    int encontrado = 0;

    printf("\nDigite o nome do produto: ");
    lerTexto(nome, 50);

    // Percorro os produtos procurando o texto informado.
    for (i = 0; i < total; i++) {
        if (contemTexto(lista[i].nome, nome)) {

            // Exibo os dados do produto encontrado.
            printf("\nID: %d\n", lista[i].id);
            printf("Nome: %s\n", lista[i].nome);
            printf("Categoria: %s\n", lista[i].categoria);
            printf("Preco: R$ %.2f\n", lista[i].preco);

            encontrado = 1; // Marco que encontrei um resultado.
        }
    }

    // Informo caso nenhum produto tenha sido encontrado.
    if (encontrado == 0) {
        printf("\nProduto nao encontrado.\n");
    }
}

// Criei uma função para buscar produtos pela categoria.
void buscarCategoria(Produto lista[], int total) {
    char categoria[50];
    int i;
    int encontrado = 0;

    printf("\nDigite a categoria: ");
    lerTexto(categoria, 50);

    // Percorro todos os produtos procurando a categoria.
    for (i = 0; i < total; i++) {
        if (contemTexto(lista[i].categoria, categoria)) {

            // Exibo os dados dos produtos encontrados.
            printf("\nID: %d\n", lista[i].id);
            printf("Nome: %s\n", lista[i].nome);
            printf("Categoria: %s\n", lista[i].categoria);
            printf("Preco: R$ %.2f\n", lista[i].preco);

            encontrado = 1;
        }
    }

    // Informo quando não encontro nenhum resultado.
    if (encontrado == 0) {
        printf("\nCategoria nao encontrada.\n");
    }
}

// Criei uma função para ordenar os produtos usando Bubble Sort.
void ordenarProdutos(Produto lista[], int total) {
    Produto aux; // Uso uma variável auxiliar para trocar os produtos.
    int opcao;
    int i;
    int j;
    int troca;

    // Verifico se existem pelo menos dois produtos.
    if (total < 2) {
        printf("\nNao ha produtos suficientes para ordenar.\n");
        return;
    }

    // Apresento as quatro formas de ordenação solicitadas.
    printf("\n1 - Nome A-Z\n");
    printf("2 - Nome Z-A\n");
    printf("3 - Preco menor para maior\n");
    printf("4 - Preco maior para menor\n");
    printf("Escolha: ");

    scanf("%d", &opcao);
    while (getchar() != '\n');

    // Verifico se a opção escolhida é válida.
    if (opcao < 1 || opcao > 4) {
        printf("\nOpcao invalida.\n");
        return;
    }

    // O primeiro for controla as passagens do Bubble Sort.
    for (i = 0; i < total - 1; i++) {

        // O segundo for compara os produtos vizinhos.
        for (j = 0; j < total - i - 1; j++) {

            troca = 0; // Começo considerando que não precisa trocar.

            // Ordeno os nomes de A até Z.
            if (opcao == 1 &&
                compararTexto(lista[j].nome, lista[j + 1].nome) > 0) {
                troca = 1;
            }

            // Ordeno os nomes de Z até A.
            if (opcao == 2 &&
                compararTexto(lista[j].nome, lista[j + 1].nome) < 0) {
                troca = 1;
            }

            // Ordeno os preços do menor para o maior.
            if (opcao == 3 &&
                lista[j].preco > lista[j + 1].preco) {
                troca = 1;
            }

            // Ordeno os preços do maior para o menor.
            if (opcao == 4 &&
                lista[j].preco < lista[j + 1].preco) {
                troca = 1;
            }

            // Se precisar trocar, inverto a posição dos produtos.
            if (troca == 1) {
                aux = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
        }
    }

    // Exibo o resultado sem salvar a nova ordem no arquivo.
    printf("\nProdutos ordenados:\n");
    listarProdutos(lista, total);
}

// Criei uma função para exportar os produtos para CSV.
void exportarCSV(Produto lista[], int total) {
    FILE *arquivo;
    int i;

    // Abro o arquivo CSV para escrita.
    arquivo = fopen("produtos.csv", "w");

    if (arquivo == NULL) {
        printf("\nErro ao criar o arquivo CSV.\n");
        return;
    }

    // Escrevo o cabeçalho separado por vírgulas.
    fprintf(arquivo, "ID,Nome,Categoria,Preco\n");

    // Percorro os produtos e exporto seus dados.
    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d,%s,%s,%.2f\n",
                lista[i].id,
                lista[i].nome,
                lista[i].categoria,
                lista[i].preco);
    }

    fclose(arquivo); // Fecho o arquivo depois da exportação.

    printf("\nArquivo CSV exportado com sucesso.\n");
}

// Função principal do programa.
int main() {
    Produto *lista = NULL; // Começo com o vetor vazio.
    int total = 0; // Inicializo a quantidade de produtos.
    int opcao;

    // Carrego os produtos salvos anteriormente.
    carregarProdutos(&lista, &total);

    // Utilizo o do while para manter o menu funcionando.
    do {
        printf("\n========== LOJA DO SEU ZE ==========\n");
        printf("1 - Inserir produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Alterar produto\n");
        printf("4 - Excluir produto\n");
        printf("5 - Buscar por nome\n");
        printf("6 - Buscar por categoria\n");
        printf("7 - Ordenar produtos\n");
        printf("8 - Exportar CSV\n");
        printf("0 - Sair\n");

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        // Utilizo o switch para executar a opção escolhida.
        switch (opcao) {

            case 1:
                inserirProduto(&lista, &total);
                break;

            case 2:
                listarProdutos(lista, total);
                break;

            case 3:
                alterarProduto(lista, total);
                break;

            case 4:
                excluirProduto(lista, &total);
                break;

            case 5:
                buscarProduto(lista, total);
                break;

            case 6:
                buscarCategoria(lista, total);
                break;

            case 7:
                ordenarProdutos(lista, total);
                break;

            case 8:
                exportarCSV(lista, total);
                break;

            case 0:
                printf("\nPrograma encerrado.\n");
                break;

            default:
                printf("\nOpcao invalida.\n");
        }

    } while (opcao != 0); // Repito o menu até escolher sair.

    free(lista); // Libero a memória que reservei para os produtos.

    return 0; // Encerro o programa.
}
