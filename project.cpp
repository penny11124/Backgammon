#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <map>
#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)
const int INF=1000000000;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
int piece;
int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
int simboard[15][15]={0};
int x=0,y=0,row,col;
int hash[255555],hash2[255555];
int f(int l,int r){
    if(r>14)
        return 0;
    int v=0;
    for(int i=l;i<=r;i++)
        v=v*10+simboard[row][i];
    if(r-l+1==5)
        return hash2[v];
    return hash[v];
}
int f2(int l,int r){
    if(r>14)
        return 0;
    int v=0,i;
    for(i=l;i<=r;i++)
        v=v*10+simboard[i][col];
    if(r-l+1==5)
        return hash2[v];
    return hash[v];
}
int f3(int l,int r,int len){//(l,r)->(l+len-1,r+len-1)
    if(max(l,r)+len-1>14||min(l,r)<0)  
        return 0;
    int i,v=0;
    for(i=0;i<len;i++)
        v=v*10+simboard[l+i][r+i];
    if(len==5)
        return hash2[v];
    return hash[v];
}
int f4(int l,int r,int len){//(l,r)->(l-len,r+len)
    if(l-len<0||r<0||r+len>14||l>14)
        return 0;
    int i,v=0;
    for(i=0;i<len;i++)
        v=v*10+simboard[l-i][r+i];
    if(len==5)
        return hash2[v];
    return hash[v];
}
bool hasNeighbor(int x,int y) {
    for(int k=-1;k<=1;k++) {
        for(int l=-1;l<=1;l++) {
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
            if(board[i][j]==player) simboard[i][j]=1;
            else if(board[i][j]==(player==1?2:1)) simboard[i][j]=2;
        }
    }
}

int evaluate() {
    int i,j,v=0;
    for(i=0;i<15;i++){
        row=col=i;
        for(j=0;j<15;j++)
            v+=f(j,j+4)+f(j,j+5)+f2(j,j+4)+f2(j,j+5);
    }
    for(i=-14;i<=14;i++)
        for(j=0;j<15;j++)
            v+=f3(j,i+j,5)+f3(j,i+j,6);
    for(i=0;i<=28;i++)
        for(j=0;j<15;j++)
            v+=f4(j,i-j,5)+f4(j,i-j,6);
    return v;
    // int my_value=0,other_value=0;
    // for(int i=0;i<15;i++) {
    //     int my_row=myAI[i][0]*16+myAI[i][1]*8+myAI[i][2]*4+myAI[i][3]*2+myAI[i][4];
    //     int other_row=otherAI[i][0]*16+otherAI[i][1]*8+otherAI[i][2]*4+otherAI[i][3]*2+otherAI[i][4];
    //     int my_col=myAI[0][i]*16+myAI[1][i]*8+myAI[2][i]*4+myAI[3][i]*2+myAI[4][i];
    //     int other_col=otherAI[0][i]*16+otherAI[1][i]*8+otherAI[2][i]*4+otherAI[3][i]*2+otherAI[4][i];

    //     my_value=hash5[my_row]+hash5[my_col];
    //     other_value=hash5[other_row]+hash5[other_col];

    //     for(int j=5;j<15;j++) {
    //         my_row=((my_row-myAI[i][j-5]*16)*2+myAI[i][j]); //橫
    //         other_row=((other_row-otherAI[i][j-5]*16)*2+otherAI[i][j]);
    //         my_col=((my_col-myAI[j-5][i]*16)*2+myAI[j][i]); //豎
    //         other_col=((other_col-otherAI[j-5][i]*16)*2+myAI[j][i]);

    //         my_value+=hash5[my_row]+hash5[my_col];
    //         other_value+=hash5[other_row]+hash5[other_col];
    //     }
    // }
    // for(int i=-10;i<=10;i++) {
    //     for(int k=0;k<10;k++) {
    //         int my_rdal,my_ldal,other_rdal,other_ldal;
    //         for(int j=0;j<5;j++) {
    //             my_rdal+=myAI[k+j][k+i]*(1<<(4-j));
    //             other_rdal+=otherAI[k+j][k+i]*(1<<(4-j));
    //         }
    //         my_value+=hash5[my_rdal];
    //         other_value+=hash5[other_rdal];
    //     }        
    // }
    // for(int i=4;i<=24;i++) {
    //     for(int k=0;k<10;k++) {
    //         int my_rdal,my_ldal,other_rdal,other_ldal;
    //         for(int j=0;j<5;j++) {
    //             my_rdal+=myAI[k+j][k+i]*(1<<(4-j));
    //             other_rdal+=otherAI[k+j][k+i]*(1<<(4-j));
    //         }
    //         my_value+=hash5[my_rdal];
    //         other_value+=hash5[other_rdal];
    //     }        
    // }

    // return my_value-other_value;
}
int alphabeta(int depth,int alpha,int beta,bool maximizingPlayer) {
    // for(int i=0;i<15;i++) {
    //     for(int j=0;j<15;j++) {
    //         printf("%d",board[i][j]);
    //     }
    //     printf("\n");
    // }
    //   printf("\n");
    //     printf("\n");
    if(depth==0 || piece+depth>225) return evaluate();
    if(maximizingPlayer) {
        int value=-INF;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {
                if(board[i][j]!=EMPTY || !hasNeighbor(i,j)) continue;
                board[i][j]=player;
                simboard[i][j]=1;
                int val=alphabeta(depth-1,alpha,beta,0);
                board[i][j]=EMPTY;
                simboard[i][j]=0;
                if(val>value) {
                    value=val;
                    if(depth==3) x=i,y=j;
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
                simboard[i][j]=2;
                int val=alphabeta(depth-1,alpha,beta,1);
                board[i][j]=EMPTY;
                simboard[i][j]=0;
                if(val<value) {
                    value=val;
                    if(depth==3) x=i,y=j;
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
    // Keep updating the output until getting killed.
    //while(true) {
        int cnt=0;
        for(int i=0;i<15;i++) {
            for(int j=0;j<15;j++) {//(i,j)
                if(board[i][j]!=EMPTY)
                    continue;
                bool used=0;
                for(int k=-1;k<=1;k++) 
                    for(int l=-1;l<=1;l++) 
                        if(i+k>=0 && i+k<15 && j+l>=0 && j+l<15)
                            used|=(board[i+k][j+l]!=EMPTY);
                cnt+=used;
                //if(used)
                    //alphabeta(2,-INF,INF,1);
            }
        }
        alphabeta(3,-INF,INF,1);
        if(!cnt){
            fout << 7 << " " << 7 << std::endl;     
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
        else if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;     
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
        piece++;
  //  }
}

int main(int, char** argv) {
    hash[211000]=hash[112]=15; //死二
    hash[122000]=hash[221]=-16;
    hash[210100]=hash[1012]=25; //死二
    hash[120200]=hash[2021]=-26;
    hash[210010]=hash[10012]=20; //死二
    hash[120020]=hash[20021]=-21;
    hash[11000]=hash[110]=650; //活二
    hash[22000]=hash[220]=-660;
    hash[1010]=hash[10100]=400; //活二
    hash[2020]=hash[20200]=-410; 
    hash[211100]=hash[1112]=500; //死三
    hash[122200]=hash[2221]=-510;
    hash[210110]=hash[11012]=800; //死三
    hash[120220]=hash[22021]=-810;
    hash2[10011]=hash2[10101]=600; //死三
    hash2[20022]=hash2[20202]=-610;
    hash[10110]=20000; //活三
    hash[20220]=-19900*2;
    hash[11100]=hash[1110]=30000; //活三
    hash[22200]=hash[2220]=-29900*2;
    hash[211110]=hash[11112]=25000; //死四
    hash[122220]=hash[22221]=-24900*2;
    hash2[11101]=30000; //死四
    hash2[22202]=-29900*2;
    hash2[11011]=26000; //死四
    hash2[22022]=-25900*2;
    hash[11110]=300000*2; //活四
    hash[22220]=-299990*4;
    hash2[11111]=30000000; //五連
    hash2[22222]=-299999990;
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}