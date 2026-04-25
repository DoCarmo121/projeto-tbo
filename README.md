# Guia de Uso: Sistema de Busca Cinematográfica

Este guia descreve como interagir com o menu do sistema, inserir dados corretamente e aplicar combinações de filtros para realizar buscas na base de dados.

## Regras de Preenchimento
Para evitar erros de leitura ou falhas no menu, siga rigorosamente as regras abaixo ao digitar os comandos:

* **Valores Numéricos:** Utilize apenas números inteiros. Não insira vírgulas ou pontos.
* **Separação de Múltiplos Valores:** Quando o menu solicitar mais de um valor na mesma linha (como intervalos de ano, duração ou coordenadas), digite o primeiro valor, dê um **espaço em branco**, digite o segundo valor e pressione Enter. Nunca utilize hifens, parênteses ou pontuação.
* **Sensibilidade a Maiúsculas e Minúsculas:** O sistema é exato na busca por texto:
    * **Tipos de formato:** Digite tudo em minúsculas (ex: `movie`, `short`, `tvEpisode`).
    * **Gêneros:** Digite sempre com a primeira letra maiúscula (ex: `Comedy`, `Drama`, `Sci-Fi`).

---

## Navegação e Opções de Busca

Ao iniciar o programa, você deverá escolher qual entidade deseja consultar: **1. Filmes** ou **2. Cinemas**. A partir dessa escolha, você entrará na "Área de Trabalho", onde poderá criar e combinar filtros.

### Filtros Disponíveis para Filmes
Ao criar um novo bloco de busca para Filmes, você pode filtrar por:
* **Por Tipo:** Busca pelo formato da obra.
* **Por Gênero:** Permite buscar por um gênero específico. O sistema também oferece a opção de incluir um segundo gênero simultaneamente (ex: obras que sejam de Comédia OU Drama).
* **Por Duração:** Exige a digitação do limite mínimo e máximo de minutos (separados por espaço).
* **Por Ano:** Exige o ano inicial e o ano final. Para buscar um ano exato, repita o valor (ex: `2004 2004`).

### Filtros Disponíveis para Cinemas
Ao criar um novo bloco de busca para Cinemas, você pode filtrar por:
* **Filtros de Exibição:** Localiza cinemas que possuam na sua grade filmes com um determinado Tipo, Gênero, Duração ou Ano.
* **Por Distância:** Exige a digitação da coordenada X, coordenada Y e a distância máxima (raio), separados por espaços.
* **Por Preço Máximo:** Busca cinemas com valor de ingresso até o número inteiro estipulado.
* **Por Título do Filme:** Busca exata pelo nome do filme para encontrar os cinemas que o exibem.

---

## Área de Trabalho: Como Combinar Filtros

O menu funciona através de um sistema de "blocos". Cada filtro criado gera um bloco com um ID. Para buscas complexas, você deve cruzar esses blocos.

**Exemplo Passo a Passo:** Buscar episódios de TV de 2004 que sejam de Comédia ou Talk-Show.

1. No Menu Principal, selecione **1 (Filmes)**.
2. Na Área de Trabalho, escolha **1 (Criar novo filtro base)**. Selecione "Tipo" e digite: `tvEpisode`.
3. Escolha novamente **1 (Criar novo filtro base)**. Selecione "Ano" e digite: `2004 2004`.
4. Crie o terceiro filtro base **(Opção 1)**. Selecione "Gênero". Digite `Comedy` e, quando o sistema perguntar pelo gênero adicional, digite `Talk-Show`.
5. Agora vamos cruzar os dados: Escolha **2 (Combinar com AND)** e informe o ID do bloco do Tipo e o ID do bloco do Ano. O sistema criará um novo bloco resultante dessa junção.
6. Escolha **2 (Combinar com AND)** novamente. Informe o ID do bloco resultante do passo anterior e o ID do bloco de Gênero.
7. Com o filtro finalizado, escolha **4 (Ver e Exportar resultados)** e digite o ID do seu último bloco criado. O sistema imprimirá a lista formatada e será encerrado.