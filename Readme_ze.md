# 🏪 Sistema de Gerenciamento - Loja do Seu Zé

O projeto **Loja do Seu Zé** é um sistema acadêmico de gerenciamento comercial desenvolvido inteiramente na linguagem C. Ele funciona como um controlador de estoque e cadastro de produtos, operando diretamente via terminal de linha de comando. O sistema utiliza uma abordagem de persistência de dados em arquivos de texto plano, dispensando o uso de bancos de dados complexos para manter a simplicidade estrutural e focar no aprendizado da linguagem.

## 👥 Autores e Contexto Acadêmico

Este sistema foi desenvolvido como um trabalho universitário pelos seguintes alunos:

| Aluno | RGM | Curso | 
| ----- | ----- | ----- | 
| Guilherme Hiroshi dos Santos Okamoto | 47554703 | Engenharia de Software | 
| Gustavo Gomes Silva | 47436808 | Engenharia de Software | 
| Pedro Henrique Ruola Coiado | 47589124 | Ciência da Computação | 

## 📂 Estrutura de Arquivos e Arquitetura

* `loja_do_seu_ze.c`: Código-fonte principal. Contém toda a lógica do programa, como a estrutura de repetição do menu principal, as definições de variáveis (utilizando `structs` para agrupar as propriedades de um produto) e as funções de manipulação (CRUD: Criar, Ler, Atualizar e Deletar).
* `produtos.csv` e `produtos.txt`: São os "bancos de dados" do sistema. O programa lê e grava as informações nestes arquivos para garantir que os dados não sejam perdidos ao fechar o aplicativo. A estrutura obrigatória das colunas é: `ID,Nome,Categoria,Preco`.
* `.vscode/tasks.json`: Arquivo de configuração para o editor Visual Studio Code. Automatiza a tarefa de compilar o arquivo `.c` e gerar o executável.
* `loja_do_seu_ze.exe`: Arquivo executável final compilado, pronto para ser rodado no Windows sem a necessidade de um compilador.

## ⚙️ Como o Sistema Funciona (Lógica de Execução)

O fluxo padrão do sistema quando executado segue os seguintes passos:

1. **Carregamento (Leitura):** O programa abre os arquivos `.csv` / `.txt` e carrega os produtos já cadastrados para a memória (geralmente para um array de structs).
2. **Interface do Usuário:** Um menu interativo em texto é exibido no terminal, oferecendo as opções do sistema (cadastrar, listar, buscar, etc.).
3. **Manipulação (Operações):** Ao registrar um novo item, o programa solicita o preenchimento dos dados básicos (ID, Nome, Categoria e Preço).
4. **Persistência (Escrita):** Utilizando a manipulação nativa de arquivos em C (através de ponteiros `FILE *`), o sistema salva as alterações e atualiza o arquivo `produtos.csv` para garantir que o inventário atualizado persista para a próxima vez que o programa for aberto.

## 🚀 Passo a Passo para Execução

### 1. Pré-requisitos
Para rodar este projeto, você precisará de:
* Um compilador C instalado na sua máquina (como **GCC** via MinGW no Windows ou nativo no Linux/Mac).
* Opcional: **Visual Studio Code** com as extensões de C/C++ instaladas.

### 2. Compilando o Código
Abra o terminal na pasta raiz do projeto (`loja_do_seu_ze-master`) e digite o seguinte comando para compilar:
```bash
gcc loja_do_seu_ze.c -o loja_do_seu_ze.exe
```
*(Nota: Se você estiver usando o VS Code, pode simplesmente usar as tarefas de build automáticas configuradas pressionando Ctrl+Shift+B).*

### 3. Executando o Sistema
Após a compilação, inicie o programa com o comando:

**No Windows:**
```bash
.\loja_do_seu_ze.exe
```

**No Linux/macOS:**
```bash
./loja_do_seu_ze.exe
```

## 💡 Dicas para Bons Resultados e Manipulação

Para garantir que o sistema funcione perfeitamente e evitar corrupção de dados:

* **Estrutura do CSV:** Nunca apague ou altere a primeira linha de cabeçalho (`ID,Nome,Categoria,Preco`) do arquivo de texto, pois o código provavelmente a ignora de forma intencional ou a usa como referência.
* **Entrada de Dados:** Evite usar vírgulas (`,`) nos nomes das categorias ou dos produtos ao digitar no terminal. Como o arquivo é um CSV (*Comma-Separated Values*), uma vírgula extra no nome do produto vai quebrar a formatação da linha inteira.
* **Encerramento:** Sempre saia do programa usando a opção "Sair" do menu, para garantir que as funções de salvamento em arquivo (`fclose`) sejam executadas corretamente antes do programa fechar.

---
Feito com 💻 e ☕ pela equipe.