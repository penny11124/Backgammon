#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <map>
const int INF=10000000000;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
int piece;
int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
int hash5[32];
int hash6[64];
int x,y;
bool AllList[15][15];

bool hasNeighbor(int x,int y) {
    for(int k=-2;k<=2;k++) 
        for(int l=-2;l<=2;l++) 
            if(x+k>=0 && x+k<15 && y+l>=0 && y+l<15)
                if(board[x+k][y+l]!=EMPTY) return true;
    return false;
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

int evaluate(std::array<std::array<int, SIZE>, SIZE> board) {

}
int alphabeta(std::array<std::array<int, SIZE>, SIZE> board,int depth,int alpha,int beta,bool maximizingPlayer) {
    if(depth==0 || piece+depth>225) return evaluate(board[][]);
    if(maximizingPlayer) {
        int value=-INF;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {
                value=max(value,alphabeta(board[i][j],depth-1,alpha,beta,0));
                alpha=max(alpha,value);
                if(alpha>=beta) break;
            }
        }
        return value;
    } else {
        int value=INF;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {
                value=min(value,alphabeta(board[i][j],depth-1,alpha,beta,1));
                beta=min(beta,value);
                if(beta<=alpha) break;
            }
        }
        return value;
    }

}
void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {//(i,j)
                if(board[i][j]!=EMPTY)
                    continue;
                bool used=0;
                for(int k=-1;k<=1;k++) 
                    for(int l=-1;l<=1;l++) 
                        if(i+k>=0 && i+k<15 && j+l>=0 && j+l<15)
                            used|=(board[i+k][j+l]!=EMPTY);
                if(used)
                    alphabeta(board[i][j],6,-INF,INF,1);
            }
        }
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;     
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
        piece++;
    }
}

int main(int, char** argv) {
    hash5[12]=70,hash5[10]=60,hash5[6]=70;
    hash5[26]=300,hash5[7]=500,hash5[28]=500;
    hash5[14]=5000,hash5[29]=4950,hash5[27]=4900;
    hash5[23]=4950,hash5[30]=5000,hash5[15]=5000;
    hash5[31]=99999999;
    hash6[18]=50,hash6[22]=4800,hash6[26]=4800,hash6[30]=500000;
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
