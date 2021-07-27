### Display LCD 16x4 e teclado matricial

O projeto se trata de um teclado matricial 4x4, onde as 3 primeias colunas seguem o padrão <br>
de teclados númericos de celulares, o botão da primeira linha e quarta coluna fornece um campo de digitção <br>
para que o usuário insira a posição na primeira linha para onde se deseja a apontar o cursor do display <br>
e consecutivamente ler o dado escrito naquela posição (posições disponiveis atualmente para leitura vão de 0 a 9). <br>

Para o display de LCD temos a implementação do modo de leitura (somente para 8 vias de dados) e escrita (tanto para 4 <br>
quanto para 8 vias). Inicialmente o projeto mostra uma mensagem para o usuário e depois de um tempo fornece um campo para <br>
digitação de um número telefônico que é formatado automaticamente para visulização do DDD e ambas as partes do número <br>
divididas por um hífen. <br>

Quando o valor presente no display é equivalente ao tamanho de um número de telefone o display para de imprimir novos <br>
digítos. Sempre que o usuário apertar o '#' que fica na linha 4 e coluna 3 o número é submetido, caso seja válido, mostra <br>
uma mensagem indicando que a ligação está sendo feita, caso contrário é exibida uma mensagem de erro. <br>

Outra tecla de ação é o '\*' na linha 4 e coluna 1, sempre que esse botão é apertado a tela é limpa um novo número pode <br>
ser inserido.<br>

As tecnologias implementadas no projeto foram: 
* Teclado Matricial 4x4 <br>
* Display LCD 16x4 <br>
* Placa STM32 NUCLEO-L053R8 <br>
* STM32CubeMX <br>
* Keil uVision 5 <br>
