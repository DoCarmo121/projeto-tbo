Documentação Técnica: Sistema de Busca Cinematográfica

Este documento descreve o funcionamento e as regras de entrada do sistema de busca desenvolvido em C++. O sistema utiliza mapas de espalhamento (Hash Maps) para indexação e vetores ordenados para operações de conjuntos, garantindo alta performance em bases de dados volumosas.

## Arquitetura de Filtragem e Lógica Booleana

O sistema opera através de um acumulador de resultados linear. Isso significa que cada novo filtro aplicado interage com o resultado das operações anteriores.

### Precedência e o Novo Agrupamento de Gêneros
Para evitar que uma operação de UNIÃO (OR) descarte filtros restritivos aplicados anteriormente (como Ano ou Tipo), o sistema implementa uma lógica de agrupamento interno no filtro de gêneros:

1. Filtro Simples: O usuário define um critério (Ex: Tipo = movie) e o intercala com AND.
2. Agrupamento Interno (OR Local): Ao selecionar o filtro de gênero, o sistema permite que o usuário defina múltiplos gêneros simultaneamente. O sistema realiza a união desses gêneros *antes* de cruzar com o acumulador principal.
   - Isso permite realizar buscas como: `(Tipo: tvEpisode AND Ano: 2004) AND (Gênero: Comedy OR Talk-Show)`.

---

## Protocolo de Entrada de Dados

O sistema é sensível ao formato dos dados. O descumprimento das regras abaixo pode causar falhas na conversão de tipos (stoi/stof) ou loops no buffer do terminal.

### Regras de Formatação
- Valores Numéricos: Utilize apenas números inteiros. O sistema realiza o arredondamento ou truncamento interno. Não utilize vírgulas ou pontos.
- Separação de Entradas: Para filtros que exigem múltiplos valores (coordenadas ou intervalos), utilize apenas o Espaço ou a tecla Enter. Nunca utilize parênteses ou símbolos de pontuação.
- Case Sensitivity: O sistema diferencia maiúsculas de minúsculas conforme o padrão da base:
   - Atributos de formato (titleType): minúsculos (ex: movie, short, tvEpisode).
   - Categorias (genres): Iniciais maiúsculas (ex: Comedy, Drama, Sci-Fi).

---

## Filtros Disponíveis

### Entidade: Filmes
- Por Tipo: Filtra pela natureza da obra.
- Por Gênero (Atualizado): Permite a seleção de um gênero base e a inclusão opcional de um segundo gênero em lógica de união local.
- Por Duração: Exige limite mínimo e máximo em minutos.
- Por Ano: Exige ano inicial e final (para ano único, repita o valor).

### Entidade: Cinemas
- Filtros de Exibição: Localiza estabelecimentos que possuem filmes com Tipo, Gênero, Duração ou Ano específicos em sua grade.
- Por Distância: Localiza cinemas baseando-se em coordenadas cartesianas (X, Y) e um raio de busca.
- Por Preço Máximo: Filtra por valor inteiro de ingresso.
- Por Título: Busca exata por títulos originais ou primários.

---

## Exemplo de Fluxo Corrigido

Para buscar episódios de TV de 2004 que sejam de Comédia ou Talk-Show:

1. Entidade: Selecionar Filmes.
2. Primeiro Filtro: Tipo -> "tvEpisode".
3. Combinação: Selecionar 1 (AND).
4. Segundo Filtro: Ano -> "2004 2004".
5. Combinação: Selecionar 1 (AND).
6. Terceiro Filtro: Gênero -> Definir "Comedy" e, na solicitação de inclusão adicional, definir "Talk-Show".
7. Finalização: O sistema entregará apenas registros que atendam a todos os critérios cronológicos e de formato, respeitando a alternância de gêneros.