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
int myAI[15][15]={0},otherAI[15][15]={0}; //方便計分
int x,y;

bool hasNeighbor(int x,int y) {
    for(int k=-2;k<=2;k++) {
        for(int l=-2;l<=2;l++) {
            if(k==0 && l==0) continue;
            if(x+k>=0 && x+k<15 && y+l>=0 && y+l<15)
                if(board[x+k][y+l]!=EMPTY) return true;
        }
    }
    return false;
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if(board[i][j]==player) myAI[i][j]=1;
            else if(board[i][j]==(player==1?2:1)) otherAI[i][j]=1;
        }
    }
}

int evaluate() {
    int my_value=0,other_value=0;
    for(int i=0;i<15;i++) {
        int my_row=myAI[i][0]*16+myAI[i][1]*8+myAI[i][2]*4+myAI[i][3]*2+myAI[i][4];
        int other_row=otherAI[i][0]*16+otherAI[i][1]*8+otherAI[i][2]*4+otherAI[i][3]*2+otherAI[i][4];
        int my_col=myAI[0][i]*16+myAI[1][i]*8+myAI[2][i]*4+myAI[3][i]*2+myAI[4][i];
        int other_col=otherAI[0][i]*16+otherAI[1][i]*8+otherAI[2][i]*4+otherAI[3][i]*2+otherAI[4][i];

        my_value=hash5[my_row]+hash5[my_col];
        other_value=hash5[other_row]+hash5[other_col];

        for(int j=5;j<15;j++) {
            my_row=((my_row-myAI[i][j-5]*16)*2+myAI[i][j]); //橫
            other_row=((other_row-otherAI[i][j-5]*16)*2+otherAI[i][j]);
            my_col=((my_col-myAI[j-5][i]*16)*2+myAI[j][i]); //豎
            other_col=((other_col-otherAI[j-5][i]*16)*2+myAI[j][i]);

            my_value+=hash5[my_row]+hash5[my_col];
            other_value+=hash5[other_row]+hash5[other_col];
        }
    }
    for(int i=-10;i<=10;i++) {
        for(int k=0;k<10;k++) {
            int my_dal;
            for(int j=0;j<5;j++) {
                my_dal+=myAI[k+j][k+i]*(1<<(4-j));
            }
            my_value+=hash5[my_dal];
        }        
    }
}
int alphabeta(std::array<std::array<int, SIZE>, SIZE> board,int depth,int alpha,int beta,bool maximizingPlayer) {
    if(depth==0 || piece+depth>225) return evaluate();
    if(maximizingPlayer) {
        int value=-INF;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {
                if(board[i][j]!=EMPTY || !hasNeighbor(i,j)) continue;
                board[i][j]=player;
                myAI[i][j]=1;
                int val=alphabeta(board,depth-1,alpha,beta,0);
                board[i][j]=EMPTY;
                myAI[i][j]=0;
                if(val>value) {
                    value=val;
                    x=i,y=j;
                }
                alpha=max(alpha,value);
                if(alpha>=beta) break;
            }
        }
        return value;
    } else {
        int value=INF;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {
                if(board[i][j]!=EMPTY || !hasNeighbor(i,j)) continue;
                board[i][j]=(player==1?2:1);
                otherAI[i][j]=1;
                int val=min(value,alphabeta(board,depth-1,alpha,beta,1));
                board[i][j]=EMPTY;
                otherAI[i][j]=0;
                if(val<value) {
                    value=val;
                    x=i,y=j;
                }
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
                    alphabeta(board,6,-INF,INF,1);
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
