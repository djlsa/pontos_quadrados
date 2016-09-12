/* Jogo dos Pontos e Quadrados


*/

#define TAMANHO_MAXIMO 10

void Quadricula(int tamanho,
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO])
{
    int i,j,ncasas;
    // colocar a identificação das colunas
    printf("\n  ");
    for(i=0;i<tamanho+1;i++)
        printf("%c   ",i+'0');
    printf("\n");
    for(i=0;i<tamanho+1;i++) {
        // colocar a identificação das linhas
        printf("%c ",i+'A');
        for(j=0;j<tamanho;j++)
            if(paredesHorizontais[i][j])
                printf("+---");
            else
                printf("+   ");
        printf("+\n");
        if(i<tamanho) {
            printf("  ");
            for(j=0;j<tamanho+1;j++) {
                if(paredesVerticais[i][j])
                    printf("| ");
                else
                    printf("  ");
                if(j<tamanho) {
                    if(casas[i][j]>0) 
                        printf("X ");
                    else if(casas[i][j]<0)
                        printf("O ");
                    else
                        printf("  ");
                }
            }
            printf("\n");
        }
    }
    // calcular número de casas controladas por cada jogador
    ncasas=0;
    for(i=0;i<tamanho;i++)
        for(j=0;j<tamanho;j++)
            if(casas[i][j]>0)
                ncasas++;
    printf("Casas X: %d, ", ncasas);
    ncasas=0;
    for(i=0;i<tamanho;i++)
        for(j=0;j<tamanho;j++)
            if(casas[i][j]<0)
                ncasas++;
    printf("casas O: %d\n", ncasas);
}

void Jogada(int tamanho,
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int jogador)
{
    char str[80];
    // letra da linha seguida do dígito da coluna
    while(1) {
        printf("\nJogada de %c (B3 traco horizontal da linha B e coluna 3, 3B traco vertical): ",
            (jogador>0?'X':'O'));
        do {
            gets(str);
        } while(strlen(str)==0);

        if(strlen(str)==2 && str[0]>='A' && str[0]<'A'+tamanho+1 && str[1]>='0' && str[1]<'0'+tamanho) {
            // traço horizontal, linha seguida de coluna
            if(paredesHorizontais[str[0]-'A'][str[1]-'0']==0) {
                paredesHorizontais[str[0]-'A'][str[1]-'0']=1;
                return;
            } else
                printf("Parede ja colocada.\n");

        }

        if(strlen(str)==2 && str[1]>='A' && str[1]<'A'+tamanho && str[0]>='0' && str[0]<'0'+tamanho+1) {
            // traço vertical, coluna seguida de linha
            if(paredesVerticais[str[1]-'A'][str[0]-'0']==0) {
                paredesVerticais[str[1]-'A'][str[0]-'0']=1;
                return;
            } else
                printf("Parede ja colocada.\n");
        }
    } 
}

int NumeroParedes(int i, int j, 
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO])
{
    int paredes=0;
    if(paredesVerticais[i][j]!=0)
        paredes++;
    if(paredesVerticais[i][j+1]!=0)
        paredes++;
    if(paredesHorizontais[i][j]!=0)
        paredes++;
    if(paredesHorizontais[i+1][j]!=0) 
        paredes++;
    return paredes;
}


void JogadorArtificial(int tamanho,
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO])
{
    int i,j,direcao,nparedes;
    int paredes[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
    int numeroParedes[5]; // total de casas com i paredes
    int ordemParedes[4]={3,0,1,2};


    for(i=0;i<5;i++)
        numeroParedes[i]=0;

    // calcular o número de paredes em cada casa
    for(i=0;i<tamanho;i++)
        for(j=0;j<tamanho;j++) {
            paredes[i][j]=NumeroParedes(i,j,paredesVerticais,paredesHorizontais);
            numeroParedes[paredes[i][j]]++;
        }

    // colocar paredes em casas por ordem de número de paredes, 
    // primeiro casas com 3 paredes, de seguida casas com 0 paredes, e assim sucessivamente
    for(nparedes=0;nparedes<4;nparedes++)
        if(numeroParedes[ordemParedes[nparedes]]>0) {
            do {
                do {
                    i=rand()%tamanho;
                    j=rand()%tamanho;
                    direcao=rand()%4;
                } while(paredes[i][j]!=ordemParedes[nparedes]);
                if(direcao==0 && paredesVerticais[i][j]==0) {
                    printf("\nJogada: %c%c\n",'0'+j,'A'+i);
                    paredesVerticais[i][j]=1;
                } else if(direcao==1 && paredesVerticais[i][j+1]==0) {
                    printf("\nJogada: %c%c\n",'0'+j+1,'A'+i);
                    paredesVerticais[i][j+1]=1;
                } else if(direcao==2 && paredesHorizontais[i][j]==0) {
                    printf("\nJogada: %c%c\n",'A'+i,'0'+j);
                    paredesHorizontais[i][j]=1;
                } else if(direcao==3 && paredesHorizontais[i+1][j]==0) {
                    printf("\nJogada: %c%c\n",'A'+i+1,'0'+j);
                    paredesHorizontais[i+1][j]=1;
                } else 
                    direcao=-1;
            } while(direcao<0);
            break;
        }
}


int VerificaCasas(int tamanho,
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO],
    int jogador)
{
    int resultado=0,i,j;
    for(i=0;i<tamanho;i++)
        for(j=0;j<tamanho;j++)
            if(casas[i][j]==0 &&
                paredesVerticais[i][j]!=0 && paredesVerticais[i][j+1]!=0 &&
                paredesHorizontais[i][j]!=0 && paredesHorizontais[i+1][j]!=0) {
                casas[i][j]=jogador;
                resultado++;
            }
    return resultado;
}

int CasasVazias(int tamanho,
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO])
{
    int resultado=0,i,j;
    for(i=0;i<tamanho;i++)
        for(j=0;j<tamanho;j++)
            if(casas[i][j]==0)
                resultado++;
    return resultado;
}

void main()
{
    int paredesVerticais[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
    int paredesHorizontais[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
    int casas[TAMANHO_MAXIMO][TAMANHO_MAXIMO];
    int N,i,j,jogador;

    // leitura de dados iniciais
    printf("\nJogo dos pontos e quadrados\nIndique dimensao [2-%d]: ", TAMANHO_MAXIMO-1);
    scanf("%d",&N);
    if(N<2)
        N=2;
    else if(N>TAMANHO_MAXIMO-1)
        N=TAMANHO_MAXIMO-1;

    // inicialização
    for(i=0;i<=N;i++)
        for(j=0;j<=N;j++)
            casas[i][j]=paredesVerticais[i][j]=paredesHorizontais[i][j]=0;
    srand(1);

    // desenhar o mapa de jogo
    Quadricula(N,paredesVerticais,paredesHorizontais,casas);

    //  efetuar jogadas
    jogador=1;
    while(CasasVazias(N,casas)>0)
    {
        if(jogador>0) // jogador humano
            Jogada(N,paredesVerticais,paredesHorizontais,casas,jogador);
        else
            JogadorArtificial(N,paredesVerticais,paredesHorizontais,casas);
        // contabilizar as casas, e no caso de não serem tomadas casas, trocar o jogador
        if(VerificaCasas(N,paredesVerticais,paredesHorizontais,casas,jogador)==0)
            jogador*=-1;
        // mostra o tabuleiro apenas se for o jogador humano a jogar
        Quadricula(N,paredesVerticais,paredesHorizontais,casas);
    }
}