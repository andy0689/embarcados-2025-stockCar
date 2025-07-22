#include <REGX52.H>

// --- Definições de Hardware para o GLCD ---
#define GlcdDataBus P1    // Porta de dados do display GLCD
sbit RS  = P2^0;          // Register Select (Comando/Dados)
sbit RW  = P2^1;          // Read/Write (Escrita no display)
sbit EN  = P2^2;          // Enable (Pulso para operação)
sbit CS1 = P2^3;          // Chip Select 1 (Seleção da metade esquerda do GLCD)
sbit CS2 = P2^4;          // Chip Select 2 (Seleção da metade direita do GLCD)

// --- Entradas de Controle do Jogador ---
sbit BTN_LEFT  = P3^2;    // Botão para mover o veículo para a esquerda
sbit BTN_RIGHT = P3^3;    // Botão para mover o veículo para a direita

// --- Enumerações para maior clareza e legibilidade ---
// Define tipos de glifos para o jogo
enum GlyphType {
    GLYPH_EMPTY_SPACE = 0,
    GLYPH_FILLED_BLOCK = 1, // Usado para o fundo escuro do túnel
    GLYPH_ROAD_SIDE_DAY = 2, // Lado da pista (dia)
    GLYPH_PLAYER_CAR_DAY = 3, // Veículo do jogador (dia)
    GLYPH_ENEMY_CAR = 4, // Veículo inimigo
    GLYPH_PLAYER_CAR_NIGHT = 5, // Veículo do jogador (noite)
    GLYPH_ROAD_SIDE_NIGHT = 6, // Lado da pista (noite) - atualmente não usado, mas pode ser para variações
    GLYPH_HEADLIGHT = 7 // Desenho do farol do veículo
};

// Define tipos de colisão para o game over
enum CollisionType {
    COLLISION_WALL = 0,
    COLLISION_ENEMY = 1
};

// --- Desenhos Gráficos Personalizados (Glifos) ---
// Cada glifo é uma matriz de 6 bytes, representando 6 colunas de 8 pixels de altura.
// O bit 0 é o pixel superior, o bit 7 é o pixel inferior.
code unsigned char GLYPHS[][6] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // GLYPH_EMPTY_SPACE
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // GLYPH_FILLED_BLOCK
    {0x1F, 0x00, 0x00, 0x00, 0x00, 0x00}, // GLYPH_ROAD_SIDE_DAY
    {0x0A, 0x1F, 0x12, 0x12, 0x1F, 0x0A}, // GLYPH_PLAYER_CAR_DAY
    {0x0A, 0x1F, 0x12, 0x12, 0x1F, 0x0A}, // GLYPH_ENEMY_CAR
    {0xF5, 0xE0, 0xED, 0xED, 0xE0, 0xF5}, // GLYPH_PLAYER_CAR_NIGHT
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // GLYPH_ROAD_SIDE_NIGHT
    {0x18, 0x10, 0x00, 0x00, 0x10, 0x18}, // GLYPH_HEADLIGHT
};

// --- Conjunto de Caracteres para Mensagens (LETRAS) ---
// Cada glifo é uma matriz de 6 bytes. Comentários agora refletem o glifo visual.
//Os conjuntos de caracteres formam a frase "VAIDENOVO!"
code unsigned char LETRAS[13][6] = {
    {0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00},    
    {0x7F, 0x02, 0x0C, 0x30, 0x7F, 0x00},    
    {0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00},    
    {0x7F, 0x49, 0x49, 0x49, 0x41, 0x00},    
    {0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00},    
    {0x3E, 0x41, 0x41, 0x41, 0x22, 0x00},    
    {0x01, 0x01, 0x7F, 0x01, 0x01, 0x00},    
    {0x41, 0x41, 0x7F, 0x41, 0x41, 0x00},    
    {0x02, 0x01, 0x59, 0x09, 0x06, 0x00},    
    {0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00},    
    {0x00, 0x00, 0x5F, 0x00, 0x00, 0x00},    
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    
    {0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00},    
};

// --- Conjunto de Caracteres para Números (NUMEROS) ---
code unsigned char NUMEROS[13][6] = {
    {0x3E, 0x45, 0x49, 0x51, 0x3E, 0x00}, // 0: Dígito '0'
    {0x00, 0x21, 0x7F, 0x01, 0x00, 0x00}, // 1: Dígito '1'
    {0x23, 0x45, 0x49, 0x51, 0x21, 0x00}, // 2: Dígito '2'
    {0x42, 0x41, 0x51, 0x69, 0x46, 0x00}, // 3: Dígito '3'
    {0x0C, 0x14, 0x24, 0x7F, 0x04, 0x00}, // 4: Dígito '4'
    {0x72, 0x51, 0x51, 0x51, 0x4E, 0x00}, // 5: Dígito '5'
    {0x1E, 0x29, 0x49, 0x49, 0x06, 0x00}, // 6: Dígito '6'
    {0x40, 0x47, 0x48, 0x50, 0x60, 0x00}, // 7: Dígito '7'
    {0x36, 0x49, 0x49, 0x49, 0x36, 0x00}, // 8: Dígito '8'
    {0x30, 0x49, 0x49, 0x4A, 0x3C, 0x00}, // 9: Dígito '9'
    {0x00, 0x00, 0x06, 0x06, 0x00, 0x00}, // 10: Caractere '.' (ponto decimal)
    {0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // 11: Caractere 'k'
    {0xFE, 0x60, 0x18, 0x60, 0xFE, 0x00}, // 12: Caractere 'M'
};

// --- Dados da Pista (Representação em Linhas) ---
// Agora usamos uma matriz 2D para as linhas da pista, o que simplifica o acesso e a manipulação.
#define TRACK_HEIGHT 8 // Número de linhas visíveis da pista
#define TRACK_WIDTH 10 // Número de colunas da pista
idata unsigned char track[TRACK_HEIGHT][TRACK_WIDTH];

// --- Posição do Veículo do Jogador ---
unsigned char playerCarPosition = 4; // Coluna inicial do veículo (centro da pista)

// --- Limites Dinâmicos da Pista (Paredes) ---
unsigned char leftBoundary = 0;  // Coluna da parede esquerda
unsigned char rightBoundary = 9; // Coluna da parede direita

// --- Variáveis de Controle do Jogo ---
unsigned char enemySpawnCounter = 0; // Contador para espaçar a aparição de inimigos
unsigned char randomSeed = 3;        // Semente para geração pseudo-aleatória
enum CollisionType collisionType = COLLISION_WALL; // Indica o tipo de colisão
unsigned int distanceTraveled = 0;   // Distância percorrida em décimos de quilômetro (ex: 12 = 1.2km)

// --- Estado do Ambiente de Jogo ---
bit tunnelModeActive = 0; // Sinaliza se o jogo está no modo túnel (noite)

// --- Constantes de Jogo ---
#define TUNNEL_MODE_DISTANCE 150 // Distância para ativar o modo túnel (15.0 km)
#define VICTORY_DISTANCE 201     // Distância para a vitória (20.1 km)

// --- Rotina de Atraso de Tempo ---
// Cria um atraso simples, útil para controlar a velocidade das operações do GLCD e do jogo.
void customDelay(int milliseconds) {
    int i, j; // Declarado no início da função
    for(i = 0; i < milliseconds; i++)
        for(j = 0; j < 10; j++); // Loop de calibração para o atraso
}

// --- Funções de Controle do GLCD ---
// Seleciona o chip de controle do GLCD para a página esquerda (primeira metade).
void Glcd_SelectLeftPage() { CS1 = 0; CS2 = 1; }
// Seleciona o chip de controle do GLCD para a página direita (segunda metade).
void Glcd_SelectRightPage() { CS1 = 1; CS2 = 0; }

// Envia um comando para o GLCD.
void Glcd_SendCommand(char command) {
    GlcdDataBus = command; // Carrega o comando no barramento de dados
    RS = 0; RW = 0;        // Seleciona modo Comando e Escrita
    EN = 1; customDelay(1); EN = 0; customDelay(1); // Pulso de Enable
}

// Envia dados para o GLCD.
void Glcd_SendData(char dataByte) {
    GlcdDataBus = dataByte; // Carrega os dados no barramento
    RS = 1; RW = 0;        // Seleciona modo Dados e Escrita
    EN = 1; customDelay(1); EN = 0; customDelay(1); // Pulso de Enable
}

// Desenha um caractere/símbolo personalizado no GLCD.
void Glcd_DrawGlyph(unsigned char *symbolData) {
    int i; // Declarado no início da função
    for(i = 0; i < 6; i++) // Itera pelas 6 colunas do glifo
        Glcd_SendData(symbolData[i]);
}

// Renderiza uma linha completa da pista no GLCD.
void Glcd_RenderTrackLine(unsigned char *lineData, int lineLength) {
    int i; // Declarado no início da função
    for(i = 0; i < lineLength; i++) // Percorre os elementos da linha
        Glcd_DrawGlyph(GLYPHS[lineData[i]]); // Desenha o glifo correspondente
}

// Inverte os bits de um byte (útil para adaptar caracteres ao GLCD se necessário).
// Renomeada para 'invertByteBits' para clareza.
unsigned char invertByteBits(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// Exibe a pontuação (distância percorrida) na parte direita do GLCD.
void displayScore() {
    unsigned char scoreDigits[6]; // Buffer para os caracteres da pontuação "12.3km"
    unsigned int tempDistance = distanceTraveled; // Usa uma cópia para manipulação
    int i, j; // Declarado no início da função

    // Converte a distância em dígitos individuais
    scoreDigits[0] = tempDistance / 100;
    scoreDigits[1] = (tempDistance / 10) % 10;
    scoreDigits[2] = 10; // Índice para o '.' nos NUMEROS
    scoreDigits[3] = tempDistance % 10;
    scoreDigits[4] = 11; // Índice para 'k' nos NUMEROS
    scoreDigits[5] = 12; // Índice para 'm' nos NUMEROS

    Glcd_SelectRightPage(); // Seleciona a página direita do GLCD
    Glcd_SendCommand(0x40); // Define a coluna inicial
    Glcd_SendCommand(0xBA); // Define a linha (page) inicial

    // Desenha cada caractere do placar
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 6; j++) {
            Glcd_SendData(invertByteBits(NUMEROS[scoreDigits[i]][j]));
        }
    }
}

// --- Gerenciamento da Pista ---
// Desloca todas as linhas existentes para baixo e adiciona uma nova linha no topo da pista.
// Agora opera sobre a matriz 2D 'track'.
void scrollTrackDownAndAddTopLine(unsigned char *newTrackLine) {
    int i, j; // Declarado no início da função
    // Desloca as linhas existentes para baixo
    for(i = TRACK_HEIGHT - 1; i > 0; i--) {
        for(j = 0; j < TRACK_WIDTH; j++) {
            track[i][j] = track[i-1][j];
        }
    }
    // Adiciona a nova linha no topo (índice 0)
    for(j = 0; j < TRACK_WIDTH; j++) {
        track[0][j] = newTrackLine[j];
    }
}

// Atualiza a posição lateral do veículo do jogador com base nos botões.
void handlePlayerInput() {
    static bit prevBtnLeftState = 1;  // Estado anterior do botão esquerdo
    static bit prevBtnRightState = 1; // Estado anterior do botão direito

    // Detecta borda de descida no botão esquerdo (pressionado)
    if (BTN_LEFT == 0 && prevBtnLeftState == 1) {
        if (playerCarPosition > 0) { // Garante que não saia da tela
            playerCarPosition--; // Move o veículo para a esquerda
        }
    }
    prevBtnLeftState = BTN_LEFT;

    // Detecta borda de descida no botão direito (pressionado)
    if (BTN_RIGHT == 0 && prevBtnRightState == 1) {
        if (playerCarPosition < TRACK_WIDTH - 1) { // Garante que não saia da tela
            playerCarPosition++; // Move o veículo para a direita
        }
    }
    prevBtnRightState = BTN_RIGHT;
}

// Gera o conteúdo de uma nova linha da pista, incluindo paredes e possíveis inimigos.
void generateNextTrackLine(unsigned char *generatedLine) {
    int i; // Declarado no início da função
    unsigned char objectPosition; // Posição para objetos na nova linha

    // Inicializa a nova linha com espaços vazios
    for(i = 0; i < TRACK_WIDTH; i++) generatedLine[i] = GLYPH_EMPTY_SPACE;

    // --- Geração Pseudo-Aleatória Aprimorada ---
    // Usa uma combinação de fatores para uma semente mais variada.
    // O operador XOR (^) e adição com um número primo podem aumentar a aleatoriedade.
    randomSeed = (randomSeed * 17 + playerCarPosition * 31 + enemySpawnCounter * 23 + track[1][4] + 13) % 251;
    enemySpawnCounter++; // Incrementa o contador de frames para controle de inimigos

    // --- Lógica de Movimento das Paredes da Pista ---
    // Faz a parede esquerda mover-se entre as colunas 0 e 1
    if ((randomSeed % 5) < 2 && leftBoundary < 1) leftBoundary++;
    else if ((randomSeed % 5) > 3 && leftBoundary > 0) leftBoundary--;

    // Faz a parede direita mover-se entre as colunas 8 e 9
    if ((randomSeed % 7) < 2 && rightBoundary < 9) rightBoundary++;
    else if ((randomSeed % 7) > 4 && rightBoundary > 8) rightBoundary--;

    // Coloca os glifos de parede na nova linha
    generatedLine[leftBoundary]  = GLYPH_ROAD_SIDE_DAY; // Ou GLYPH_ROAD_SIDE_NIGHT se aplicável
    generatedLine[rightBoundary] = GLYPH_ROAD_SIDE_DAY;

    // --- Geração de Veículos Inimigos ---
    // A cada poucos frames (controlado por enemySpawnCounter e randomSeed), gera um inimigo
    if(enemySpawnCounter >= (randomSeed % 3) + 5) {
        enemySpawnCounter = 0; // Reseta o contador
        // Posição aleatória para o inimigo (evita paredes e a posição do jogador na próxima linha)
        // Garante que objectPosition esteja sempre dentro de [1, TRACK_WIDTH - 2]
        // O limite de (TRACK_WIDTH - 2) para o módulo garante que não apareça nas paredes externas.
        objectPosition = 1 + (randomSeed % (TRACK_WIDTH - 2)); 
        
        generatedLine[objectPosition] = GLYPH_ENEMY_CAR;  // Coloca o glifo de veículo inimigo
    }

    distanceTraveled++; // Incrementa a distância percorrida (0.1km por linha)

    // Ativa o modo túnel após uma certa distância
    if(distanceTraveled >= TUNNEL_MODE_DISTANCE) {
        tunnelModeActive = 1;
    }
}

// --- Renderização da Pista no GLCD ---
// Desenha todas as linhas da pista, aplicando os efeitos de dia/noite e farol.
// Agora usa a matriz 2D 'track' para renderização.
void renderTrackOnGlcd() {
    int row, col; // Declarado no início da função
    unsigned char tempDisplayLine[TRACK_WIDTH]; // Buffer temporário para a linha a ser exibida
    int distanceToPlayerLine; // Declarado no início da função
    unsigned char safePlayerPosOffset; // Variável para armazenar o offset seguro

    Glcd_SelectLeftPage(); // As linhas da pista são desenhadas na página esquerda

    // Processa e desenha as linhas da pista (da linha 0 à TRACK_HEIGHT-1)
    for(row = 0; row < TRACK_HEIGHT; row++) {
        // Copia a linha atual para um buffer temporário para modificação
        for(col = 0; col < TRACK_WIDTH; col++) {
            tempDisplayLine[col] = track[row][col];
        }

        // --- Efeitos Visuais do Modo Túnel ---
        if (tunnelModeActive) {
            // Transforma espaços vazios em pixels preenchidos (escurece o fundo)
            for(col = 0; col < TRACK_WIDTH; col++) {
                if (tempDisplayLine[col] == GLYPH_EMPTY_SPACE) {
                    tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
                }
            }

            // Esconde veículos inimigos fora do alcance do farol
            // A linha do jogador é a última (TRACK_HEIGHT - 1)
            // Distância é calculada a partir da linha do jogador para cima.
            distanceToPlayerLine = (TRACK_HEIGHT - 1) - row;

            for(col = 0; col < TRACK_WIDTH; col++) {
                if (track[row][col] == GLYPH_ENEMY_CAR) { // Verifica o glifo original da pista
                    bit isVisible = 0;
                    // Lógica de visibilidade do farol (cone gradual)
                    if (distanceToPlayerLine == 1 && col == playerCarPosition) {
                        isVisible = 1;
                    } else if (distanceToPlayerLine == 2) {
                        // Proteção para playerCarPosition - 1
                        safePlayerPosOffset = (playerCarPosition > 0) ? (playerCarPosition - 1) : 0;
                        if (col >= safePlayerPosOffset && col <= playerCarPosition + 1) isVisible = 1;
                    } else if (distanceToPlayerLine == 3) {
                        // Proteção para playerCarPosition - 2
                        safePlayerPosOffset = (playerCarPosition > 1) ? (playerCarPosition - 2) : 0;
                        if (col >= safePlayerPosOffset && col <= playerCarPosition + 2) isVisible = 1;
                    }

                    if (!isVisible) {
                        tempDisplayLine[col] = GLYPH_FILLED_BLOCK; // Inimigo invisível vira fundo escuro
                    }
                }
            }

            // Desenha o farol apenas na linha logo acima do jogador (TRACK_HEIGHT - 2)
            if(row == TRACK_HEIGHT - 2) {
                if(tempDisplayLine[playerCarPosition] == GLYPH_FILLED_BLOCK || tempDisplayLine[playerCarPosition] == GLYPH_EMPTY_SPACE) {
                    tempDisplayLine[playerCarPosition] = GLYPH_HEADLIGHT; // Coloca o glifo do farol na posição do jogador
                }
            }
        } else { // Fora do modo túnel
            // Remove qualquer glifo de farol (garante que não apareça durante o dia)
            for(col = 0; col < TRACK_WIDTH; col++) {
                if(tempDisplayLine[col] == GLYPH_HEADLIGHT) {
                    tempDisplayLine[col] = GLYPH_EMPTY_SPACE; // FAROL vira ESPAÇO
                }
            }
        }

        // Envia os comandos para desenhar a linha processada no GLCD
        Glcd_SendCommand(0x40);         // Define a coluna de início (sempre 0)
        Glcd_SendCommand(0xB8 + row);   // Define a página (linha visual) no GLCD
        Glcd_RenderTrackLine(tempDisplayLine, TRACK_WIDTH); // Desenha os glifos da linha
    }

    // --- Processamento e Desenho da Linha do Jogador (última linha da tela) ---
    // Prepara a última linha para exibir o carro do jogador
    for(col = 0; col < TRACK_WIDTH; col++) { // Declarado no início da função
        tempDisplayLine[col] = track[TRACK_HEIGHT - 1][col]; // Copia o estado original da última linha

        if(tunnelModeActive) {
            // Escurece o fundo na linha do jogador também
            if(tempDisplayLine[col] == GLYPH_EMPTY_SPACE) tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
            // Garante que inimigos na linha do jogador estejam escondidos (não devem aparecer aqui)
            if(tempDisplayLine[col] == GLYPH_ENEMY_CAR) tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
        }
    }

    // Insere o glifo do veículo do jogador na sua posição atual
    tempDisplayLine[playerCarPosition] = tunnelModeActive ? GLYPH_PLAYER_CAR_NIGHT : GLYPH_PLAYER_CAR_DAY;
    // Garante que as paredes sejam desenhadas corretamente na linha do jogador,
    // caso o movimento aleatório as tenha removido ou se o jogador estiver muito perto.
    tempDisplayLine[0] = GLYPH_ROAD_SIDE_DAY; // Parede esquerda na coluna 0
    tempDisplayLine[TRACK_WIDTH - 1] = GLYPH_ROAD_SIDE_DAY; // Parede direita na última coluna

    // Envia os comandos para desenhar a linha final do jogador no GLCD
    Glcd_SendCommand(0x40);             // Define a coluna de início
    Glcd_SendCommand(0xBF);             // Define a página (última linha visual)
    Glcd_RenderTrackLine(tempDisplayLine, TRACK_WIDTH); // Desenha a linha do jogador

    displayScore(); // Atualiza e exibe a pontuação
}

// --- Verificação de Colisão ---
// Determina se o veículo do jogador colidiu com uma parede ou um inimigo.
bit checkGameCollision() {
    // Colisão com a parede na linha do jogador
    if (track[TRACK_HEIGHT - 1][playerCarPosition] == GLYPH_ROAD_SIDE_DAY) {
        collisionType = COLLISION_WALL; // Tipo de colisão: parede
        return 1; // Houve colisão
    }
    // Colisão com outro veículo na linha do jogador
    if (track[TRACK_HEIGHT - 1][playerCarPosition] == GLYPH_ENEMY_CAR) {
        collisionType = COLLISION_ENEMY; // Tipo de colisão: inimigo
        return 1; // Houve colisão
    }
    return 0; // Nenhuma colisão detectada
}

// --- Tela de Vitória ---
// Exibe uma mensagem de vitória na tela quando o objetivo é alcançado.
void showVictoryScreen() {
    // Mensagem "VC VENCEU!" usando os índices do array LETRAS
    code unsigned char victoryMessage[10] = {4,5,11,4,3,1,5,3,12,10}; // V, C, ESPAÇO, V, E, N, C, E, U, !
    int i; // Declarado no início da função
    
    Glcd_SelectRightPage(); // Usa a página direita para a mensagem
    Glcd_SendCommand(0x40); // Define a coluna inicial
    Glcd_SendCommand(0xBC); // Define a linha para a mensagem de vitória

    // Desenha cada caractere da mensagem
    for(i = 0; i < 10; i++) {
        Glcd_DrawGlyph(LETRAS[victoryMessage[i]]);
    }

    while(1); // Mantém a tela de vitória indefinidamente
}

// --- Tela de Game Over ---
// Exibe uma mensagem de fim de jogo e o motivo da colisão.
void showGameOverScreen() {
    int i; 
    // MENSAGEM AJUSTADA: "VAIDENOVO!" (10 caracteres)
    // V=4, A=9, I=7, D=2, E=3, N=1, O=0, V=4, O=0, !=10
    code unsigned char wallCollisionMsg[10] = {4, 9, 7, 2, 3, 1, 0, 4, 0, 10}; // "VAIDENOVO!" (colisão com parede)
    code unsigned char enemyCollisionMsg[10] = {9,5,7,2,3,1,6,3,10,10}; // "ACIDENTE!!" (colisão com inimigo)

    Glcd_SelectRightPage(); // Usa a página direita para a mensagem
    Glcd_SendCommand(0x40); // Define a coluna inicial
    Glcd_SendCommand(0xBC); // Define a linha para a mensagem de game over

    // O loop desenha 10 caracteres, o tamanho exato dos arrays de mensagem
    for(i = 0; i < 10; i++) { 
        if (collisionType == COLLISION_WALL) { // Se foi colisão com a parede
            Glcd_DrawGlyph(LETRAS[wallCollisionMsg[i]]);
        } else { // Se foi colisão com um veículo inimigo
            Glcd_DrawGlyph(LETRAS[enemyCollisionMsg[i]]);
        }
    }

    while(1); // Mantém a tela de game over indefinidamente
}

// --- Função Principal do Programa (Main) ---
void main() {
    unsigned char newGeneratedLine[TRACK_WIDTH]; // Buffer para a nova linha da pista
    int k; // Declarado no início da função

    // Inicialização da pista: Preenche as primeiras linhas para que o jogo comece visível.
    for (k = 0; k < TRACK_HEIGHT; k++) {
        generateNextTrackLine(newGeneratedLine);    // Cria o conteúdo de uma nova linha
        scrollTrackDownAndAddTopLine(newGeneratedLine); // Adiciona a linha na pista e move as outras
    }
    renderTrackOnGlcd(); // Renderiza a pista inicial no GLCD

    // --- Loop Principal do Jogo ---
    // Este loop executa continuamente as etapas do jogo.
    while(1) {
        customDelay(20);             // Controla a velocidade geral do jogo
        handlePlayerInput();         // Processa a entrada do jogador para mover o veículo
        generateNextTrackLine(newGeneratedLine); // Cria o próximo segmento da pista

        // Verifica se o jogador alcançou a distância de vitória
        if(distanceTraveled >= VICTORY_DISTANCE) {
            showVictoryScreen(); // Encerra o jogo com a tela de vitória
        }

        scrollTrackDownAndAddTopLine(newGeneratedLine); // Atualiza a "rolagem" da pista

        // Verifica se ocorreu alguma colisão na linha do jogador
        if (checkGameCollision()) {
            showGameOverScreen(); // Exibe a tela de Game Over e encerra
        }

        renderTrackOnGlcd(); // Renderiza o estado atual do jogo no display
    }
}