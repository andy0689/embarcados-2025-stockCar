#include <REGX52.H>

// --- Defini��es de Hardware para o GLCD ---
#define GlcdDataBus P1    // Porta de dados do display GLCD
sbit RS  = P2^0;          // Register Select (Comando/Dados)
sbit RW  = P2^1;          // Read/Write (Escrita no display)
sbit EN  = P2^2;          // Enable (Pulso para opera��o)
sbit CS1 = P2^3;          // Chip Select 1 (Sele��o da metade esquerda do GLCD)
sbit CS2 = P2^4;          // Chip Select 2 (Sele��o da metade direita do GLCD)

// --- Entradas de Controle do Jogador ---
sbit BTN_LEFT  = P3^2;    // Bot�o para mover o ve�culo para a esquerda
sbit BTN_RIGHT = P3^3;    // Bot�o para mover o ve�culo para a direita

// --- Enumera��es para maior clareza e legibilidade ---
// Define tipos de glifos para o jogo
enum GlyphType {
    GLYPH_EMPTY_SPACE = 0,
    GLYPH_FILLED_BLOCK = 1, // Usado para o fundo escuro do t�nel
    GLYPH_ROAD_SIDE_DAY = 2, // Lado da pista (dia)
    GLYPH_PLAYER_CAR_DAY = 3, // Ve�culo do jogador (dia)
    GLYPH_ENEMY_CAR = 4, // Ve�culo inimigo
    GLYPH_PLAYER_CAR_NIGHT = 5, // Ve�culo do jogador (noite)
    GLYPH_ROAD_SIDE_NIGHT = 6, // Lado da pista (noite) - atualmente n�o usado, mas pode ser para varia��es
    GLYPH_HEADLIGHT = 7 // Desenho do farol do ve�culo
};

// Define tipos de colis�o para o game over
enum CollisionType {
    COLLISION_WALL = 0,
    COLLISION_ENEMY = 1
};

// --- Desenhos Gr�ficos Personalizados (Glifos) ---
// Cada glifo � uma matriz de 6 bytes, representando 6 colunas de 8 pixels de altura.
// O bit 0 � o pixel superior, o bit 7 � o pixel inferior.
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
// Cada glifo � uma matriz de 6 bytes. Coment�rios agora refletem o glifo visual.
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

// --- Conjunto de Caracteres para N�meros (NUMEROS) ---
code unsigned char NUMEROS[13][6] = {
    {0x3E, 0x45, 0x49, 0x51, 0x3E, 0x00}, // 0: D�gito '0'
    {0x00, 0x21, 0x7F, 0x01, 0x00, 0x00}, // 1: D�gito '1'
    {0x23, 0x45, 0x49, 0x51, 0x21, 0x00}, // 2: D�gito '2'
    {0x42, 0x41, 0x51, 0x69, 0x46, 0x00}, // 3: D�gito '3'
    {0x0C, 0x14, 0x24, 0x7F, 0x04, 0x00}, // 4: D�gito '4'
    {0x72, 0x51, 0x51, 0x51, 0x4E, 0x00}, // 5: D�gito '5'
    {0x1E, 0x29, 0x49, 0x49, 0x06, 0x00}, // 6: D�gito '6'
    {0x40, 0x47, 0x48, 0x50, 0x60, 0x00}, // 7: D�gito '7'
    {0x36, 0x49, 0x49, 0x49, 0x36, 0x00}, // 8: D�gito '8'
    {0x30, 0x49, 0x49, 0x4A, 0x3C, 0x00}, // 9: D�gito '9'
    {0x00, 0x00, 0x06, 0x06, 0x00, 0x00}, // 10: Caractere '.' (ponto decimal)
    {0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // 11: Caractere 'k'
    {0xFE, 0x60, 0x18, 0x60, 0xFE, 0x00}, // 12: Caractere 'M'
};

// --- Dados da Pista (Representa��o em Linhas) ---
// Agora usamos uma matriz 2D para as linhas da pista, o que simplifica o acesso e a manipula��o.
#define TRACK_HEIGHT 8 // N�mero de linhas vis�veis da pista
#define TRACK_WIDTH 10 // N�mero de colunas da pista
idata unsigned char track[TRACK_HEIGHT][TRACK_WIDTH];

// --- Posi��o do Ve�culo do Jogador ---
unsigned char playerCarPosition = 4; // Coluna inicial do ve�culo (centro da pista)

// --- Limites Din�micos da Pista (Paredes) ---
unsigned char leftBoundary = 0;  // Coluna da parede esquerda
unsigned char rightBoundary = 9; // Coluna da parede direita

// --- Vari�veis de Controle do Jogo ---
unsigned char enemySpawnCounter = 0; // Contador para espa�ar a apari��o de inimigos
unsigned char randomSeed = 3;        // Semente para gera��o pseudo-aleat�ria
enum CollisionType collisionType = COLLISION_WALL; // Indica o tipo de colis�o
unsigned int distanceTraveled = 0;   // Dist�ncia percorrida em d�cimos de quil�metro (ex: 12 = 1.2km)

// --- Estado do Ambiente de Jogo ---
bit tunnelModeActive = 0; // Sinaliza se o jogo est� no modo t�nel (noite)

// --- Constantes de Jogo ---
#define TUNNEL_MODE_DISTANCE 150 // Dist�ncia para ativar o modo t�nel (15.0 km)
#define VICTORY_DISTANCE 201     // Dist�ncia para a vit�ria (20.1 km)

// --- Rotina de Atraso de Tempo ---
// Cria um atraso simples, �til para controlar a velocidade das opera��es do GLCD e do jogo.
void customDelay(int milliseconds) {
    int i, j; // Declarado no in�cio da fun��o
    for(i = 0; i < milliseconds; i++)
        for(j = 0; j < 10; j++); // Loop de calibra��o para o atraso
}

// --- Fun��es de Controle do GLCD ---
// Seleciona o chip de controle do GLCD para a p�gina esquerda (primeira metade).
void Glcd_SelectLeftPage() { CS1 = 0; CS2 = 1; }
// Seleciona o chip de controle do GLCD para a p�gina direita (segunda metade).
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

// Desenha um caractere/s�mbolo personalizado no GLCD.
void Glcd_DrawGlyph(unsigned char *symbolData) {
    int i; // Declarado no in�cio da fun��o
    for(i = 0; i < 6; i++) // Itera pelas 6 colunas do glifo
        Glcd_SendData(symbolData[i]);
}

// Renderiza uma linha completa da pista no GLCD.
void Glcd_RenderTrackLine(unsigned char *lineData, int lineLength) {
    int i; // Declarado no in�cio da fun��o
    for(i = 0; i < lineLength; i++) // Percorre os elementos da linha
        Glcd_DrawGlyph(GLYPHS[lineData[i]]); // Desenha o glifo correspondente
}

// Inverte os bits de um byte (�til para adaptar caracteres ao GLCD se necess�rio).
// Renomeada para 'invertByteBits' para clareza.
unsigned char invertByteBits(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// Exibe a pontua��o (dist�ncia percorrida) na parte direita do GLCD.
void displayScore() {
    unsigned char scoreDigits[6]; // Buffer para os caracteres da pontua��o "12.3km"
    unsigned int tempDistance = distanceTraveled; // Usa uma c�pia para manipula��o
    int i, j; // Declarado no in�cio da fun��o

    // Converte a dist�ncia em d�gitos individuais
    scoreDigits[0] = tempDistance / 100;
    scoreDigits[1] = (tempDistance / 10) % 10;
    scoreDigits[2] = 10; // �ndice para o '.' nos NUMEROS
    scoreDigits[3] = tempDistance % 10;
    scoreDigits[4] = 11; // �ndice para 'k' nos NUMEROS
    scoreDigits[5] = 12; // �ndice para 'm' nos NUMEROS

    Glcd_SelectRightPage(); // Seleciona a p�gina direita do GLCD
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
    int i, j; // Declarado no in�cio da fun��o
    // Desloca as linhas existentes para baixo
    for(i = TRACK_HEIGHT - 1; i > 0; i--) {
        for(j = 0; j < TRACK_WIDTH; j++) {
            track[i][j] = track[i-1][j];
        }
    }
    // Adiciona a nova linha no topo (�ndice 0)
    for(j = 0; j < TRACK_WIDTH; j++) {
        track[0][j] = newTrackLine[j];
    }
}

// Atualiza a posi��o lateral do ve�culo do jogador com base nos bot�es.
void handlePlayerInput() {
    static bit prevBtnLeftState = 1;  // Estado anterior do bot�o esquerdo
    static bit prevBtnRightState = 1; // Estado anterior do bot�o direito

    // Detecta borda de descida no bot�o esquerdo (pressionado)
    if (BTN_LEFT == 0 && prevBtnLeftState == 1) {
        if (playerCarPosition > 0) { // Garante que n�o saia da tela
            playerCarPosition--; // Move o ve�culo para a esquerda
        }
    }
    prevBtnLeftState = BTN_LEFT;

    // Detecta borda de descida no bot�o direito (pressionado)
    if (BTN_RIGHT == 0 && prevBtnRightState == 1) {
        if (playerCarPosition < TRACK_WIDTH - 1) { // Garante que n�o saia da tela
            playerCarPosition++; // Move o ve�culo para a direita
        }
    }
    prevBtnRightState = BTN_RIGHT;
}

// Gera o conte�do de uma nova linha da pista, incluindo paredes e poss�veis inimigos.
void generateNextTrackLine(unsigned char *generatedLine) {
    int i; // Declarado no in�cio da fun��o
    unsigned char objectPosition; // Posi��o para objetos na nova linha

    // Inicializa a nova linha com espa�os vazios
    for(i = 0; i < TRACK_WIDTH; i++) generatedLine[i] = GLYPH_EMPTY_SPACE;

    // --- Gera��o Pseudo-Aleat�ria Aprimorada ---
    // Usa uma combina��o de fatores para uma semente mais variada.
    // O operador XOR (^) e adi��o com um n�mero primo podem aumentar a aleatoriedade.
    randomSeed = (randomSeed * 17 + playerCarPosition * 31 + enemySpawnCounter * 23 + track[1][4] + 13) % 251;
    enemySpawnCounter++; // Incrementa o contador de frames para controle de inimigos

    // --- L�gica de Movimento das Paredes da Pista ---
    // Faz a parede esquerda mover-se entre as colunas 0 e 1
    if ((randomSeed % 5) < 2 && leftBoundary < 1) leftBoundary++;
    else if ((randomSeed % 5) > 3 && leftBoundary > 0) leftBoundary--;

    // Faz a parede direita mover-se entre as colunas 8 e 9
    if ((randomSeed % 7) < 2 && rightBoundary < 9) rightBoundary++;
    else if ((randomSeed % 7) > 4 && rightBoundary > 8) rightBoundary--;

    // Coloca os glifos de parede na nova linha
    generatedLine[leftBoundary]  = GLYPH_ROAD_SIDE_DAY; // Ou GLYPH_ROAD_SIDE_NIGHT se aplic�vel
    generatedLine[rightBoundary] = GLYPH_ROAD_SIDE_DAY;

    // --- Gera��o de Ve�culos Inimigos ---
    // A cada poucos frames (controlado por enemySpawnCounter e randomSeed), gera um inimigo
    if(enemySpawnCounter >= (randomSeed % 3) + 5) {
        enemySpawnCounter = 0; // Reseta o contador
        // Posi��o aleat�ria para o inimigo (evita paredes e a posi��o do jogador na pr�xima linha)
        // Garante que objectPosition esteja sempre dentro de [1, TRACK_WIDTH - 2]
        // O limite de (TRACK_WIDTH - 2) para o m�dulo garante que n�o apare�a nas paredes externas.
        objectPosition = 1 + (randomSeed % (TRACK_WIDTH - 2)); 
        
        generatedLine[objectPosition] = GLYPH_ENEMY_CAR;  // Coloca o glifo de ve�culo inimigo
    }

    distanceTraveled++; // Incrementa a dist�ncia percorrida (0.1km por linha)

    // Ativa o modo t�nel ap�s uma certa dist�ncia
    if(distanceTraveled >= TUNNEL_MODE_DISTANCE) {
        tunnelModeActive = 1;
    }
}

// --- Renderiza��o da Pista no GLCD ---
// Desenha todas as linhas da pista, aplicando os efeitos de dia/noite e farol.
// Agora usa a matriz 2D 'track' para renderiza��o.
void renderTrackOnGlcd() {
    int row, col; // Declarado no in�cio da fun��o
    unsigned char tempDisplayLine[TRACK_WIDTH]; // Buffer tempor�rio para a linha a ser exibida
    int distanceToPlayerLine; // Declarado no in�cio da fun��o
    unsigned char safePlayerPosOffset; // Vari�vel para armazenar o offset seguro

    Glcd_SelectLeftPage(); // As linhas da pista s�o desenhadas na p�gina esquerda

    // Processa e desenha as linhas da pista (da linha 0 � TRACK_HEIGHT-1)
    for(row = 0; row < TRACK_HEIGHT; row++) {
        // Copia a linha atual para um buffer tempor�rio para modifica��o
        for(col = 0; col < TRACK_WIDTH; col++) {
            tempDisplayLine[col] = track[row][col];
        }

        // --- Efeitos Visuais do Modo T�nel ---
        if (tunnelModeActive) {
            // Transforma espa�os vazios em pixels preenchidos (escurece o fundo)
            for(col = 0; col < TRACK_WIDTH; col++) {
                if (tempDisplayLine[col] == GLYPH_EMPTY_SPACE) {
                    tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
                }
            }

            // Esconde ve�culos inimigos fora do alcance do farol
            // A linha do jogador � a �ltima (TRACK_HEIGHT - 1)
            // Dist�ncia � calculada a partir da linha do jogador para cima.
            distanceToPlayerLine = (TRACK_HEIGHT - 1) - row;

            for(col = 0; col < TRACK_WIDTH; col++) {
                if (track[row][col] == GLYPH_ENEMY_CAR) { // Verifica o glifo original da pista
                    bit isVisible = 0;
                    // L�gica de visibilidade do farol (cone gradual)
                    if (distanceToPlayerLine == 1 && col == playerCarPosition) {
                        isVisible = 1;
                    } else if (distanceToPlayerLine == 2) {
                        // Prote��o para playerCarPosition - 1
                        safePlayerPosOffset = (playerCarPosition > 0) ? (playerCarPosition - 1) : 0;
                        if (col >= safePlayerPosOffset && col <= playerCarPosition + 1) isVisible = 1;
                    } else if (distanceToPlayerLine == 3) {
                        // Prote��o para playerCarPosition - 2
                        safePlayerPosOffset = (playerCarPosition > 1) ? (playerCarPosition - 2) : 0;
                        if (col >= safePlayerPosOffset && col <= playerCarPosition + 2) isVisible = 1;
                    }

                    if (!isVisible) {
                        tempDisplayLine[col] = GLYPH_FILLED_BLOCK; // Inimigo invis�vel vira fundo escuro
                    }
                }
            }

            // Desenha o farol apenas na linha logo acima do jogador (TRACK_HEIGHT - 2)
            if(row == TRACK_HEIGHT - 2) {
                if(tempDisplayLine[playerCarPosition] == GLYPH_FILLED_BLOCK || tempDisplayLine[playerCarPosition] == GLYPH_EMPTY_SPACE) {
                    tempDisplayLine[playerCarPosition] = GLYPH_HEADLIGHT; // Coloca o glifo do farol na posi��o do jogador
                }
            }
        } else { // Fora do modo t�nel
            // Remove qualquer glifo de farol (garante que n�o apare�a durante o dia)
            for(col = 0; col < TRACK_WIDTH; col++) {
                if(tempDisplayLine[col] == GLYPH_HEADLIGHT) {
                    tempDisplayLine[col] = GLYPH_EMPTY_SPACE; // FAROL vira ESPA�O
                }
            }
        }

        // Envia os comandos para desenhar a linha processada no GLCD
        Glcd_SendCommand(0x40);         // Define a coluna de in�cio (sempre 0)
        Glcd_SendCommand(0xB8 + row);   // Define a p�gina (linha visual) no GLCD
        Glcd_RenderTrackLine(tempDisplayLine, TRACK_WIDTH); // Desenha os glifos da linha
    }

    // --- Processamento e Desenho da Linha do Jogador (�ltima linha da tela) ---
    // Prepara a �ltima linha para exibir o carro do jogador
    for(col = 0; col < TRACK_WIDTH; col++) { // Declarado no in�cio da fun��o
        tempDisplayLine[col] = track[TRACK_HEIGHT - 1][col]; // Copia o estado original da �ltima linha

        if(tunnelModeActive) {
            // Escurece o fundo na linha do jogador tamb�m
            if(tempDisplayLine[col] == GLYPH_EMPTY_SPACE) tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
            // Garante que inimigos na linha do jogador estejam escondidos (n�o devem aparecer aqui)
            if(tempDisplayLine[col] == GLYPH_ENEMY_CAR) tempDisplayLine[col] = GLYPH_FILLED_BLOCK;
        }
    }

    // Insere o glifo do ve�culo do jogador na sua posi��o atual
    tempDisplayLine[playerCarPosition] = tunnelModeActive ? GLYPH_PLAYER_CAR_NIGHT : GLYPH_PLAYER_CAR_DAY;
    // Garante que as paredes sejam desenhadas corretamente na linha do jogador,
    // caso o movimento aleat�rio as tenha removido ou se o jogador estiver muito perto.
    tempDisplayLine[0] = GLYPH_ROAD_SIDE_DAY; // Parede esquerda na coluna 0
    tempDisplayLine[TRACK_WIDTH - 1] = GLYPH_ROAD_SIDE_DAY; // Parede direita na �ltima coluna

    // Envia os comandos para desenhar a linha final do jogador no GLCD
    Glcd_SendCommand(0x40);             // Define a coluna de in�cio
    Glcd_SendCommand(0xBF);             // Define a p�gina (�ltima linha visual)
    Glcd_RenderTrackLine(tempDisplayLine, TRACK_WIDTH); // Desenha a linha do jogador

    displayScore(); // Atualiza e exibe a pontua��o
}

// --- Verifica��o de Colis�o ---
// Determina se o ve�culo do jogador colidiu com uma parede ou um inimigo.
bit checkGameCollision() {
    // Colis�o com a parede na linha do jogador
    if (track[TRACK_HEIGHT - 1][playerCarPosition] == GLYPH_ROAD_SIDE_DAY) {
        collisionType = COLLISION_WALL; // Tipo de colis�o: parede
        return 1; // Houve colis�o
    }
    // Colis�o com outro ve�culo na linha do jogador
    if (track[TRACK_HEIGHT - 1][playerCarPosition] == GLYPH_ENEMY_CAR) {
        collisionType = COLLISION_ENEMY; // Tipo de colis�o: inimigo
        return 1; // Houve colis�o
    }
    return 0; // Nenhuma colis�o detectada
}

// --- Tela de Vit�ria ---
// Exibe uma mensagem de vit�ria na tela quando o objetivo � alcan�ado.
void showVictoryScreen() {
    // Mensagem "VC VENCEU!" usando os �ndices do array LETRAS
    code unsigned char victoryMessage[10] = {4,5,11,4,3,1,5,3,12,10}; // V, C, ESPA�O, V, E, N, C, E, U, !
    int i; // Declarado no in�cio da fun��o
    
    Glcd_SelectRightPage(); // Usa a p�gina direita para a mensagem
    Glcd_SendCommand(0x40); // Define a coluna inicial
    Glcd_SendCommand(0xBC); // Define a linha para a mensagem de vit�ria

    // Desenha cada caractere da mensagem
    for(i = 0; i < 10; i++) {
        Glcd_DrawGlyph(LETRAS[victoryMessage[i]]);
    }

    while(1); // Mant�m a tela de vit�ria indefinidamente
}

// --- Tela de Game Over ---
// Exibe uma mensagem de fim de jogo e o motivo da colis�o.
void showGameOverScreen() {
    int i; 
    // MENSAGEM AJUSTADA: "VAIDENOVO!" (10 caracteres)
    // V=4, A=9, I=7, D=2, E=3, N=1, O=0, V=4, O=0, !=10
    code unsigned char wallCollisionMsg[10] = {4, 9, 7, 2, 3, 1, 0, 4, 0, 10}; // "VAIDENOVO!" (colis�o com parede)
    code unsigned char enemyCollisionMsg[10] = {9,5,7,2,3,1,6,3,10,10}; // "ACIDENTE!!" (colis�o com inimigo)

    Glcd_SelectRightPage(); // Usa a p�gina direita para a mensagem
    Glcd_SendCommand(0x40); // Define a coluna inicial
    Glcd_SendCommand(0xBC); // Define a linha para a mensagem de game over

    // O loop desenha 10 caracteres, o tamanho exato dos arrays de mensagem
    for(i = 0; i < 10; i++) { 
        if (collisionType == COLLISION_WALL) { // Se foi colis�o com a parede
            Glcd_DrawGlyph(LETRAS[wallCollisionMsg[i]]);
        } else { // Se foi colis�o com um ve�culo inimigo
            Glcd_DrawGlyph(LETRAS[enemyCollisionMsg[i]]);
        }
    }

    while(1); // Mant�m a tela de game over indefinidamente
}

// --- Fun��o Principal do Programa (Main) ---
void main() {
    unsigned char newGeneratedLine[TRACK_WIDTH]; // Buffer para a nova linha da pista
    int k; // Declarado no in�cio da fun��o

    // Inicializa��o da pista: Preenche as primeiras linhas para que o jogo comece vis�vel.
    for (k = 0; k < TRACK_HEIGHT; k++) {
        generateNextTrackLine(newGeneratedLine);    // Cria o conte�do de uma nova linha
        scrollTrackDownAndAddTopLine(newGeneratedLine); // Adiciona a linha na pista e move as outras
    }
    renderTrackOnGlcd(); // Renderiza a pista inicial no GLCD

    // --- Loop Principal do Jogo ---
    // Este loop executa continuamente as etapas do jogo.
    while(1) {
        customDelay(20);             // Controla a velocidade geral do jogo
        handlePlayerInput();         // Processa a entrada do jogador para mover o ve�culo
        generateNextTrackLine(newGeneratedLine); // Cria o pr�ximo segmento da pista

        // Verifica se o jogador alcan�ou a dist�ncia de vit�ria
        if(distanceTraveled >= VICTORY_DISTANCE) {
            showVictoryScreen(); // Encerra o jogo com a tela de vit�ria
        }

        scrollTrackDownAndAddTopLine(newGeneratedLine); // Atualiza a "rolagem" da pista

        // Verifica se ocorreu alguma colis�o na linha do jogador
        if (checkGameCollision()) {
            showGameOverScreen(); // Exibe a tela de Game Over e encerra
        }

        renderTrackOnGlcd(); // Renderiza o estado atual do jogo no display
    }
}