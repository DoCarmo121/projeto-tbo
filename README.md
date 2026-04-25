## Fluxo de Navegação

O sistema é interativo e funciona através de menus no terminal. O fluxo padrão é:
1. Escolher a entidade principal que deseja buscar (**Filmes** ou **Cinemas**).
2. Aplicar o **primeiro filtro**.
3. O sistema mostrará a quantidade de resultados encontrados.
4. Você poderá escolher **adicionar um novo filtro** (combinando com os resultados atuais) ou **encerrar e exibir** os resultados.

### Combinando Filtros (Operadores Lógicos)
Ao adicionar o segundo filtro em diante, o sistema perguntará como você deseja combiná-los:
* **1. E (AND - Interseção):** O resultado deve satisfazer *ambos* os filtros (Ex: Comédia **E** Lançado em 2004). Reduz a lista.
* **2. OU (OR - União):** O resultado deve satisfazer *pelo menos um* dos filtros (Ex: Comédia **OU** Ação). Expande a lista.

---

## Filtros Disponíveis e Formato de Entrada

> **ATENÇÃO: Particularidades de Entrada no Terminal**
> Como o sistema roda direto no terminal (C++), ele é muito rigoroso com o tipo de dado digitado. Para evitar que o programa trave ou entre em um loop infinito, siga estas **regras de ouro**:
> * **Zero pontuação em números:** Não utilize vírgulas ou pontos para decimais. Se o ingresso custa R$ 15,50, digite apenas `15`.
> * **Múltiplos valores apenas com ESPAÇO ou ENTER:** Nunca use parênteses, chaves ou vírgulas para separar valores (como coordenadas).
    >   * Errado: `(20019, 510301)` ou `20019, 510301`
>   * Certo: `20019 510301 1000`
> * **Sensibilidade a Maiúsculas (Case Sensitive):** Digite os termos exatamente como formatados na base de dados (ex: `movie` todo minúsculo, mas gêneros como `Comedy` com a primeira letra maiúscula).

### 1. Filtros para Filmes

| Opção no Menu | O que faz | Como digitar (Exemplos) |
| :--- | :--- | :--- |
| **1. Por Tipo** | Filtra pelo formato da obra (`titleType`). | `movie`, `short`, `tvEpisode`, `video` *(Sem espaços, minúsculo)* |
| **2. Por Gênero** | Filtra pela categoria (`genres`). | `Comedy`, `Drama`, `Documentary`, `Talk-Show` *(Primeira letra maiúscula)* |
| **3. Por Duração** | Exige um limite inferior e superior em minutos. | `30 120` *(Digite o mínimo, Espaço/Enter, digite o máximo)* |
| **4. Por Ano** | Exige um ano inicial e final. | **Intervalo:** `2000 2010`<br>**Ano Específico:** `2004 2004` *(Digite o mesmo ano duas vezes)* |

### 2. Filtros para Cinemas

Os filtros de 1 a 3 e 6 procuram cinemas baseados nas características dos filmes que eles estão exibindo.

| Opção no Menu | O que faz | Como digitar (Exemplos) |
| :--- | :--- | :--- |
| **1. Por Tipo de Filme** | Cinemas exibindo um formato específico. | `movie`, `short`, `tvEpisode` |
| **2. Por Gênero de Filme**| Cinemas exibindo uma categoria específica. | `Comedy`, `Action`, `Horror` |
| **3. Por Duração do Filme**| Cinemas exibindo filmes dentro de um tempo. | `90 150` *(Mínimo e máximo em minutos, separados por espaço)* |
| **4. Por Distância** | Cinemas em um raio de distância da sua posição. | `20019 510301 1000` *(Coordenada X, Espaço/Enter, Coordenada Y, Espaço/Enter, Distância)* |
| **5. Por Preço Máximo** | Cinemas com ingressos até o valor estipulado. | `15` *(Apenas números inteiros, sem vírgulas)* |
| **6. Por Ano do Filme** | Cinemas exibindo filmes de uma época. | `2020 2024` *(Ano min e max)* |
| **7. Por Título Específico**| Procura onde um filme exato está passando. | `The Matrix`, `O Auto da Compadecida` *(Pode conter espaços. Busca pelo título original ou primário)* |

---

## Exemplos de Uso Prático

**Desafio 1:** "Consultar filmes do tipo 'tvEpisode' feitos no ano 2004, do gênero Comedy ou Talk-Show."
1. Selecione **1. Filtrar Filmes**
2. Escolha **1. Por Tipo** -> Digite `tvEpisode`
3. Combine com **1. E (AND)** -> Escolha **4. Por Ano** -> Digite `2004 2004` (ou `2004`, aperte Enter, depois `2004`)
4. Combine com **1. E (AND)** -> Escolha **2. Por Gênero** -> Digite `Comedy`
5. Combine com **2. OU (OR)** -> Escolha **2. Por Gênero** -> Digite `Talk-Show`
6. Escolha **0. Finalizar busca** para ver a lista de IDs e Nomes.

**Desafio 2:** "Consultar cinemas que tenham filme de gênero 'Documentary', distância de no máximo 1000 do ponto (20019, 510301), com preço no máximo 15."
1. Selecione **2. Filtrar Cinemas**
2. Escolha **2. Por Gênero de Filme** -> Digite `Documentary`
3. Combine com **1. E (AND)** -> Escolha **4. Por Distância** -> Digite `20019 510301 1000` (separados apenas por espaço ou Enter, sem parênteses)
4. Combine com **1. E (AND)** -> Escolha **5. Por Preço Máximo** -> Digite `15` (sem casas decimais)
5. Escolha **0. Finalizar busca** para listar os cinemas, seus IDs e valores de ingresso.