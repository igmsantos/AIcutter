# AIcutter 

* O CEREBRO 

  O Cerebro obtem as caracteristicas e respostas do meio externo, em que no momento se dispoe, verifica as condicoes externas, apontando o alvo da concentracao e o objeto em tese.
 
  As caracteristicas provém originalmente dos problemas enfrentados no cotidiano da tarefa designada.
  Os problemas modelados são a identificação das linhas para formação dos artigos, e a identifação dos clientes relacionados aos artigos.
 
  Para solução dos problemas apresentados foram construidas extruturas similares as naturais, as quais por meio de ligações naturalmente criadas, em forma de redes neurais, controem a solução do problema a partir das caracteristicas apresentadas e respostas equivalentes.
 
  - Derivam-se os problemas:
    - Classificação das linhas:
 	   	Analise das caracteristicas de cada linha para definição de um tipo genérico descoberto a partir da repetição de caracteristicas visuais, espaciais ou estatisticas de todas as linhas do documento apresentado. Esta generalização é utilizada para condensar os tipos identificados em tipos basicos iguais aos outros do mesmo grugo, agrupados pela generalização.
 	   	
    - Classificação dos clientes
 
  Processo de analise do documento:
  - Griffon -> Pede ao cerebro para se concentrar no documento ( CCerebro::setAtencao() )
  - Griffon -> Pede ao cerebro para analisar os dados do doumento ( CCerebro::analisaCaracteristicas() )
  - Cerebro -> Pede ao cortex para analisar as caracteristicas ( CCortex::analisaCaracteristicas() )
  - Cortex  -> Pede ao cerebro as caracteristicas doque ele esta com a atenção focada ( CCerebro::getCaracteristicasVisuais(),CCerebro::getCaracteristicasEspaciais(),CCerebro::getCaracteristicasEstatisticas(),CCerebro::getCaracteristicasClassificatorias() )
  - Cerebro -> Retira as caracteristicas do documento e passa ao cortex ( CDocumentoIA::getCaracteristicasVisuais(),CDocumentoIA::getCaracteristicasEspaciais(),CDocumentoIA::getCaracteristicasEstatisticas(),CDocumentoIA::getCaracteristicasClassificatorias() )
  - Cortex  -> Aloca bloco e monta os dados para os lobos com as caracteristicas obtidas do cerebro ( CCortex::getDados() )
  - Cortex  -> Pede aos lobos para analisarem os dados montados ( CCortex::analisaCaracteristicas() )
  - Lobo    -> Analisa dados montados e treinando a rede neural ( CLobo::analisaDados() )
  - Cortex  -> Retorna status da analise ( 0 ou 1 ) para o cerebro ( CCortex::analisaCaracteristicas() )
  - Cerebro -> Retorna status da analise ( 0 ou 1 ) para o griffon ( CCerebro::analisaCaracteristicas() )
 
 
* O CORTEX
 
   A classe Cortex é responsável pelo enlace das diversas redes neurais encapsuladas pelas classes Lobo.
   O córtex é responsavel por extrair as caracteristicas Visuais, Espaciais, Estatísticas e Classificatórias do documento, e integra-las de forma que os lobos as utilizem sem a necessidade da manutenção da memória, ou modificações no código no caso da modificação do numero de entradas ou saidas de cada lobo (rede neural).
   De acordo com as caracteristicas de area, espaco entre a linha anterior, fonte, negrito, italico, letra inicial e final da linha forto, numero ou letra, qual numero ou qual letra, o "cerebro" é estimulado em treinamento, observando como resultado a largura , altura, posicionamento vertical e horizontal por exemplo. Quando testado com as caracteristicas previamente estimuladas em treinamento, a resposta é uma geometria e posicionamento obtidos através das caracteristicas marcantes de cada linha, como o negrito, o inicio sempre com a letra P, etc... criando uma linha de largura, altura e posicionamento medianos em relação a outras linhas com caracteristicas similares. Esta amostra é utilizada para generalizar os tipos de linha em grupos definidos por suas caracteristicas, para posteriormente serem classificadas.
 
  As caracteristicas do documento estão divididas em grupos:
 
  - Caracteristicas Visuais:
    - Margem superior
    - Margem inferior
    - Presença de negrito
    - Presença de italico
    - Presença de link
 
  - Caracteristicas Espaciais:
    - Largura
    - Altura
    - Area
    - Posição X
    - Posição Y
 
  - Caracteristicas Estatisticas:
    - Inicio em numero
    - Final em numero
    - Quantidade de ocorrencias de cada caracter
 
  - Caracteristicas Classificatórias:
    - Tipos de linha
 
  - Caracteristicas Seletivas:
    - Presença do nome do cliente em questão
    - Presença do nome do cliente em questão em algum cabeçalho abaixo
    - Presença do nome de outros clientes com mais palavras na mesma posição
    - Presença de alguma palavra chave que caracterize subdistrito ou comarca
    - Presença de alguma variação do nome do cliente em questão (nome de pessoas, etc...)
 
* OS LOBOS
 
  A classe Lobo é responsavel por gerenciar as funções de treinamento e utilização da rede neural, e também de manter a herança de memória entre os documentos iguais de períodos diferentes.
