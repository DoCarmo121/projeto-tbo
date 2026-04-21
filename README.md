# Primeiro passo:
Modelar as classes de acordo com as colunas de filmes e cinemas

# Segundo passo
Fazer uma varredura pelo arquivo de filmes e de cinemas e passar eles para a memória;
Cada linha representa um filme no arquivo filmesCrop.txt
Cada linha representa um cinema no arquivo cinemas(1).txt
Montar uma tabela hash, onde o tconst vai ser normalizado e virará o novo índice


# Terceiro Passo
Organizar os dados com técnicas de ordenação
Trabalhar com  a tabela de índices, não de classes

# Quarto Passo
Implementar os filtros listados (Uso de operadores lógicos "e" ou "ou")
Para filmes:
  - Filmes de um ou mais tipos (titleType)
  - Filmes de um ou mais gêneros (genres)
  - Filmes com duração específica entre um limite inferior e superior (em minutos)
  - Filme lançado em um ano ou em um intervalo de anos
Para cinemas:
  - Cinemas que contém filmes de um ou mais tipos (titleType)
  - Cinemas que contém filmes que pertencem a um ou mais gêneros (genres)
  - Cinemas que contém filmes com uma duração específica entre um limite inferior e superior
  - Cinemas em uma localização de até uma distância definida do local
  - Cinemas com preços até um limite superior de unidades monetárias
  - Cinemas que contém filmes lançados em um ano específico ou em um intervalo de anos
  - Procurar cinema que contém um filme de título específico

# Exemplos de consultas:

Consultar cinemas que tenham filme de gênero "Documentary" ou "Short", e distância de no máximo 1000 do ponto (20019, 510301), com preço na máximo 15, 00.

Consultar filmes do tipo "tvEpisode" feitos no ano 2004, do gênero Comedy ou Talk-Show.

# Observações
Caso o cinema referencie um código de filme que não existe, pegue o filme com o código maior mais próximo do código do filme

O trabalho deve mostrar o tempo utilizado, tanto ao carregar a base de filmes, quanto ao mostrar as buscas. É preferível um tempo considerável ao carregar, que ao consultar

## Índice Espacial Secundário (Grid / Spatial Hashing)

Esta tabela hash agrupa cinemas geograficamente próximos no mesmo "balde" (*bucket*), permitindo buscas de "cinemas ao redor" em tempo `O(1)`. Isso elimina a necessidade de varrer arrays inteiros e calcular distâncias euclidianas para todo o dataset.

### Mecanismo de Funcionamento

1. **Discretização (Grid):** As coordenadas brutas (`X`, `Y`) são divididas por uma constante (ex: `1000`). A divisão inteira nativa do C++ trunca as casas decimais, forçando coordenadas próximas a resultarem no mesmo valor de quadrante (`X_Bucket` e `Y_Bucket`).

2. **Chave Única (Multiplicativa):** Para utilizar um único mapa e evitar instâncias pesadas de strings, os dois eixos são fundidos em um número inteiro através da fórmula:
   `Chave = (X_Bucket * FATOR) + Y_Bucket`

   > **Aviso de Implementação:** O `FATOR` (ex: `100000`) deve ser estritamente maior que o valor máximo possível de `Y_Bucket` para evitar colisões cruzadas entre quadrantes diferentes. A chave resultante utiliza o tipo `long long` para prevenir estouro de limite numérico (*overflow*).

### Estrutura de Dados

* **Tipo:** `std::unordered_map`
* **Chave (`long long`):** ID único do quadrante gerado pela matemática de discretização.
  *(Ex: `(X_Bucket * 100000LL) + Y_Bucket`)*
* **Valor (`std::vector<int>`):** Lista contendo os índices normalizados do array principal onde os objetos dos cinemas estão armazenados na memória.