## Informações Institucionais

- **Nome da disciplina:** Projeto de Sistemas Computacionais Embarcados  
- **Nome do docente:** Emerson Carlos Pedrino  
- **Nome do curso:** Engenharia de Computação  
- **Departamento:** Departamento de Computação - UFSCar  

---

# embarcados-2025
jogo stock car

Jogo de Corrida Stock Car para Microcontrolador 8051 e GLCD
Este repositório contém a implementação de um jogo de corrida simples desenvolvido para uma plataforma embarcada, utilizando o microcontrolador 8051 e um display gráfico LCD (GLCD). O objetivo principal é simular uma experiência de corrida com pista em rolagem, onde o jogador deve desviar de obstáculos e paredes para alcançar a maior distância possível ou a vitória.

Visão Geral do Projeto
O jogo é uma demonstração prática de programação de baixo nível para sistemas embarcados, abordando:

Manipulação de GLCD: Desenho de gráficos pixel a pixel e gerenciamento da tela.

Lógica de Jogo em Tempo Real: Atualização contínua da pista, movimento do jogador e geração de obstáculos.

Interação Hardware-Software: Processamento de entradas de botões para controle do veículo.

Geração Pseudo-Aleatória: Para a criação dinâmica de obstáculos e variações na pista.

Efeitos Visuais: Implementação de um "modo túnel" que altera a visibilidade do ambiente de jogo.

Funcionalidades e Regras do Jogo
O jogo oferece as seguintes funcionalidades e segue as regras básicas:

Pista em Rolagem: A pista se move continuamente de cima para baixo, simulando o avanço do veículo.

Controle do Veículo: O jogador controla um carro que pode se mover lateralmente (esquerda/direita) na pista.

Obstáculos: Veículos inimigos surgem aleatoriamente na pista e se movem em direção ao jogador.

Paredes Laterais: A pista possui paredes que delimitam o caminho, e suas posições podem variar ligeiramente.

Detecção de Colisão: O jogo detecta colisões do veículo do jogador com os veículos inimigos ou com as paredes laterais.

Pontuação: Um contador de distância percorrida é exibido na tela, aumentando conforme o jogador avança.

Modo Túnel: Após atingir uma certa distância (15.0 km), o jogo entra em um "modo túnel", onde a tela escurece e a visibilidade é limitada a um cone de luz ao redor do veículo do jogador. Os inimigos só são visíveis dentro deste cone.

Condições de Fim de Jogo:

Game Over: Ocorre se o jogador colidir com um veículo inimigo ("ACIDENTE!!") ou com uma parede lateral ("VAIDENOVO!").

Vitória: Ocorre se o jogador alcançar uma distância predefinida (20.1 km) sem colidir ("VC VENCEU!").

Hardware Necessário
Para executar este jogo, você precisará de:

Microcontrolador 8051: O núcleo do sistema.

Display Gráfico LCD (GLCD): Para a saída visual do jogo.

Botões de Controle: Conectados ao microcontrolador para a entrada do jogador (simulados como BTN_LEFT e BTN_RIGHT).

Software Necessário
Proteus: Para simulação do circuito completo do microcontrolador 8051 e do GLCD.

Compilador C para 8051: (e.g., Keil uVision, SDCC) para compilar o código-fonte.

Como Simular no Proteus
Carregue o Projeto: Abra o arquivo do projeto Proteus (se disponível) ou configure o circuito com o microcontrolador 8051 e o GLCD.

Compile o Código: Utilize um compilador C para 8051 (como o Keil uVision) para compilar o código-fonte (.c) e gerar o arquivo .hex.

Carregue o Firmware: No Proteus, carregue o arquivo .hex gerado para o microcontrolador 8051 na simulação.

Inicie a Simulação: Execute a simulação no Proteus.

Comportamento Esperado na Simulação:
Início: O GLCD deve exibir a pista em rolagem com o veículo do jogador centralizado e a pontuação "0.0km".

Movimento da Pista: A pista se moverá continuamente, com novas linhas e barreiras laterais aparecendo.

Controle do Jogador: Pressione os botões virtuais BTN_LEFT e BTN_RIGHT no Proteus para mover o veículo lateralmente. O carro não deve ultrapassar as barreiras.

Inimigos: Veículos inimigos aparecerão aleatoriamente na pista.

Modo Túnel: Após 15.0 km, a tela escurecerá e o efeito de farol será ativado. Inimigos só serão visíveis dentro do cone de luz.

Colisão: Se houver colisão com parede ou inimigo, a simulação pausará e exibirá a mensagem de "GAME OVER" com o motivo.

Vitória: Ao atingir 20.1 km, a simulação pausará e exibirá "VC VENCEU!".

Estrutura do Código
O código-fonte é organizado em funções que gerenciam diferentes aspectos do jogo:

Funções GLCD: Para controle de baixo nível do display (enviar comandos, dados, desenhar glifos).

displayScore(): Formata e exibe a pontuação.

pseudoRand(): Gerador de números pseudo-aleatórios para aleatoriedade no jogo.

scrollTrackDownAndAddTopLine(): Gerencia o movimento da pista e adiciona novas linhas.

handlePlayerInput(): Processa a entrada dos botões para mover o veículo do jogador.

generateNextTrackLine(): Cria o conteúdo de uma nova linha da pista, incluindo obstáculos e paredes.

renderTrackOnGlcd(): Desenha a pista completa no GLCD, aplicando os efeitos visuais (dia/noite, farol).

checkGameCollision(): Verifica colisões do jogador com obstáculos ou paredes.

showVictoryScreen() / showGameOverScreen(): Exibem as telas de vitória ou fim de jogo.

main(): A função principal que orquestra o ciclo de jogo.

Contribuições e Melhorias Futuras
Sinta-se à vontade para explorar, modificar e melhorar este projeto. Algumas ideias para futuras melhorias incluem:

Adicionar mais tipos de obstáculos.

Implementar efeitos sonoros simples.

Otimizar a renderização para taxas de quadros mais elevadas.

Adicionar diferentes níveis de dificuldade.
