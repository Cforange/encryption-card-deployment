//
// Created by Fay Chan on 2021/1/29.
//

//
// Created by Fay Chan on 2020/11/22.
//注意注意！！！！！！
//如果vector被重复调用，需要确保所有元素已经被clear！
//
#include <iostream>
#include <vector>
#include "def.h"
using namespace std;

typedef vector<vector<int>> vec;

void Psorting(vec P[n2]){//还没有验证，实现对二维数组第一列升序，第二列升序排序
    int i, m, n, row;
    for (i=0; i<n2; i++){
        vec p = P[i];
        row = p.size();
        for (m=0; m<row-1; m++)
            for (n=m+1; n<row; n++){
                if (p[m][0] > p[n][0])
                    swap(p[m],p[n]);
                else if (p[m][0] == p[n][0]){
                    if (p[m][1] > p[n][1])
                        swap(p[m],p[n]);
                }
            }
        P[i] = p;
    }
}

int Capacitydecision(int minimumcard, int num40, int num100, int num400){
    int availablecard;
    if (minimumcard == 40)
        if (num40 != 0)
            availablecard = 40;
        else if (num100 != 0)
            availablecard = 100;
        else if (num400 != 0)
            availablecard = 400;
        else
            availablecard = INF;
    else if (minimumcard == 100)
        if (num100 != 0)
            availablecard = 100;
        else if (num400 != 0)
            availablecard = 400;
        else
            availablecard = INF;
    else{
        if (num400 != 0)
            availablecard = 400;
        else
            availablecard = INF;
    }
    return availablecard;
}

int costencryptioncard(int ECbr){
    int costEC;
    if (ECbr == 40)
        costEC = 4;
    else if (ECbr == 100)
        costEC = 8;
    else if (ECbr == 400)
        costEC = 16;
    else
        costEC = INF;
    return costEC;
}

int costlinecard(int LCbr){
    int costLC;
    if (LCbr == 40)
        costLC = 2;
    else if (LCbr == 100)
        costLC = 4;
    else if (LCbr == 400)
        costLC = 8;
    else
        costLC = INF;
    return costLC;
}

void ECnumupdate(int *num40, int *num100, int *num400, int ECbr){
    //实际调用的时候输入ECnumupdate(&num40,...)
    if (ECbr == 40)
        *num40 -= 1;
    else if (ECbr == 100)
        *num100 -= 1;
    else
        *num400 -= 1;
}

int ECchoose(int ECbr, int v, int EC_use[e]){
    int i;
    for (i=0; i<e; i++)
    {
        if (ECbr == Ebitrate[i] && EC_use[i] == 1)
        {
            EC_use[i] = 0;
            break;
        }
    }
    return i;
}

void LCnumupdate(int *num40, int *num100, int *num400, int LCbr){
    //实际调用的时候输入ECnumupdate(&num40,...)
    if (LCbr == 40)
        *num40 -= 1;
    else if (LCbr == 100)
        *num100 -= 1;
    else
        *num400 -= 1;
}

int LCchoose(int LCbr, int v, int LC_use[l]){
    int i;
    for (i=0; i<l; i++)
    {
        if (LCbr == Lbitrate[i] && LC_use[i] == 1)
        {
            LC_use[i] = 0;
            break;
        }
    }
    return i;
}

void h_solve(int flow, int important_flow, int x_lc[flow][n2][l], int y_ec[flow][n2][e],
             int t_lcec[flow][n2][l*e], double h_c[flow], int M_s[flow], int M_d[flow], int M_bitrate[flow],
             int hop[2][n][n], int danger[2][n][n], vector<vector<double>> G){
    double cost = 0;
    double costH = 0;
    double costLE = 0;
    int EC_use[n][e];
    int LC_use[n][l];
    int i, j, k, q, s, d, bitrate, x, y;
    int numE40[n], numE100[n], numE400[n],numL40[n], numL100[n], numL400[n];
    int minimumcard, availablecard, ECbr, LCbr, costEC, costLC;

    //LC_use, EC_use 初始化为1
    for (i=0; i<n; i++){
        for (j=0; j<l; j++){
            LC_use[i][j] = 1;
        }
        for (j=0; j<e; j++){
            EC_use[i][j] = 1;
        }
    }
    //the number of all types of EC,LCs of each node
    for (i=0; i<n; i++){
        numE40[i] = ue[0];
        numE100[i] = ue[1];
        numE400[i] = ue[2];
        numL40[i] = ul[0];
        numL100[i] = ul[1];
        numL400[i] = ul[2];
    }
    //初始化matrix
    vec P[n2];//P的元素是可变的二维数组
    for (i=0; i<n2; i++)
        P[i].push_back(vector<int>(6,0));//lc剩余容量，ec剩余容量，lc总容量，ec总容量，lc标号，ec标号
    //vector<vector<int>> P[i] vector<int> P[i][0]

    for (q=0; q<flow; q++){
        s = M_s[q];
        d = M_d[q];
        bitrate = M_bitrate[q];
        Psorting(P);

        int LCf, ECf;
        if (bitrate <= Lt[0])
            LCf = Lt[0];
        else if (bitrate <= Lt[1])
            LCf = Lt[1];
        else
            LCf = Lt[2];

        if (bitrate <= Et[0])
            ECf = Et[0];
        else if (bitrate <= Et[1])
            ECf = Et[1];
        else
            ECf = Et[2];

        // create lightpath cost matrix
        double LM[2][n][n];
        for (i=0; i<n; i++)//初始化
            for (k=0; k<2; k++){
                for (j=0; j<n; j++)
                    LM[k][i][j] = 0;
                LM[k][i][i] = INF;
            }

        if (q < important_flow){
            for (i=0; i<n; i++)
                for (j=0; j<n; j++){
                    int index = i * n + j;
                    for (k=0; k<2; k++){
                        x = 0;
                        while ((P[index][x][0]<bitrate || P[index][x][1]<bitrate) && x<P[index].size()-1)
                            x += 1;
                        if (P[index][x][0]<bitrate || P[index][x][1]<bitrate)
                            x += 1;
                        //to find the maximum lc capacity
                        if (x <= P[index].size()-1)//找到剩余容量最大的线卡
                            LM[k][i][j] = hop[k][i][j] * bitrate /100.0;
                        else{
                            if (danger[k][i][j] > 0){
                                minimumcard = ECf;
                                availablecard = Capacitydecision(minimumcard, numE40[j], numE100[j],
                                                                 numE400[j]);
                                ECbr = availablecard;
                                costEC = costencryptioncard(ECbr);
                                if (ECbr == INF)
                                    LM[k][i][j] = INF;
                                else{
                                    // add new ec
                                    y = 0;
                                    while (P[index][y][0]<ECbr && y<P[index].size()-1)
                                        y += 1;
                                    if (P[index][y][0]<ECbr)
                                        y += 1;
                                    if (y <= P[index].size()-1)
                                        LM[k][i][j] = hop[k][i][j] * bitrate /100.0 + costEC;
                                    else{
                                        //ad new lc
                                        LCbr = ECbr;
                                        minimumcard = LCbr;
                                        availablecard = Capacitydecision(minimumcard, numL40[j], numL100[j],
                                                                         numL400[j]);
                                        LCbr = availablecard;
                                        costLC = costlinecard(LCbr);
                                        if (LCbr == INF)
                                            LM[k][i][j] = INF;
                                        else
                                            LM[k][i][j] = hop[k][i][j] * bitrate /100.0 + costEC + costLC;
                                    }
                                }
                            }
                            else{ //path not dangerous
                                y = 0;
                                while (P[index][y][0]<bitrate && y<P[index].size()-1)
                                    y += 1;
                                if (P[index][y][0]<bitrate)
                                    y += 1;
                                if (y <= P[index].size()-1)
                                    LM[k][i][j] = hop[k][i][j] * bitrate /100.0;
                                else {
                                    //ad new lc
                                    LCbr = ECbr;
                                    minimumcard = LCbr;
                                    availablecard = Capacitydecision(minimumcard, numL40[j], numL100[j],
                                                                     numL400[j]);
                                    LCbr = availablecard;
                                    costLC = costlinecard(LCbr);
                                    if (LCbr == INF)
                                        LM[k][i][j] = INF;
                                    else
                                        LM[k][i][j] = hop[k][i][j] * bitrate / 100.0 + costLC;
                                }
                            }
                        }
                    }
                }
        }
        else{//non-important flows do not need ec
            for (i=0; i<n; i++)
                for (j=0; j<n; j++) {
                    int index = i * n + j;
                    for (k = 0; k < 2; k++) {
                        x = 0;
                        while (P[index][x][0]<bitrate && x<P[index].size()-1)
                            x += 1;
                        if (P[index][x][0]<bitrate)
                            x += 1;
                        if (x <= P[index].size()-1)
                            LM[k][i][j] = hop[k][i][j] * bitrate /100.0;
                        else{
                            LCbr = LCf;
                            minimumcard = LCbr;
                            availablecard = Capacitydecision(minimumcard, numL40[j], numL100[j],
                                                             numL400[j]);
                            LCbr = availablecard;
                            costLC = costlinecard(LCbr);
                            if (LCbr == INF)
                                LM[k][i][j] = INF;
                            else
                                LM[k][i][j] = hop[k][i][j] * bitrate / 100.0 + costLC;
                        }
                    }
                }
        }
        int k_matrix[n][n];
        vector<vector<double>> lightM = G;
        for (i=0; i<n; i++)
            for (j=0; j<n; j++)
            {
                k_matrix[i][j] = 0;//初始化
                if (i == j){
                    lightM[i][j] = INF;
                    continue;
                }
                if (LM[0][i][j] <= LM[1][i][j])
                    lightM[i][j] = LM[0][i][j];
                else{
                    lightM[i][j] = LM[1][i][j];
                    k_matrix[i][j] = 1;
                }
            }
        vector<int> shortestPath;
        double totalcost = 0;
        int u, v, t, index, g, w, ec_index, m, new_row;
        totalcost = dijkstra(lightM, s, d, shortestPath);
        if (shortestPath.size() == 0)
            cout << "there is no path for " << q << "-th flow, maybe the LC or EC is not enough." << endl;
        if (q < important_flow){
            for (j=0; j<shortestPath.size()-1; j++){
                u = shortestPath[j];
                v = shortestPath[j+1];
                t = k_matrix[u][v];
                index = u * n + v;
                if (danger[t][u][v] > 0){
                    k = 0;
                    while ((P[index][k][0]<bitrate || P[index][k][1]<bitrate) && k<P[index].size()-1)
                        k += 1;
                    if (P[index][k][0]<bitrate || P[index][k][1]<bitrate)
                        k += 1;
                    if (k <= P[index].size()-1){//choose the k-th row's lc and ec
                        g = P[index][k][4];
                        for (w=0; w<P[index].size(); w++){
                            if (P[index][w][4] == g)
                                P[index][w][0] -= bitrate;//he rest of ec capacity
                        }
                        P[index][k][1] -= bitrate;//the rest of ec capacity
                        ec_index = P[index][k][5];
                        x_lc[q][index][g] = 1;
                        y_ec[q][index][ec_index] = 1;
                        t_lcec[q][index][g*e + ec_index] = 1;
                        h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                        costH += (hop[t][u][v] * bitrate / 100.0);
                        cost += (hop[t][u][v] * bitrate / 100.0);
                    }
                    else{//add new lc or ec
                        ECbr = ECf;
                        minimumcard = ECbr;
                        availablecard = Capacitydecision(minimumcard, numE40[v], numE100[v], numE400[v]);
                        ECbr = availablecard;
                        costEC = costencryptioncard(ECbr);
                        ECnumupdate(&numE40[v], &numE100[v], &numE400[v], ECbr);

                        m = 0;
                        while (P[index][m][0]<ECbr && m<P[index].size()-1)
                            m += 1;
                        if (P[index][m][0]<ECbr)
                            m += 1;
                        if (m <= P[index].size()-1){//available lc can be used
                            g = P[index][m][4];
                            for (w=0; w<P[index].size(); w++){
                                if (P[index][w][4] == g)
                                    P[index][w][0] -= bitrate;//he rest of lc capacity
                            }
                            P[index].push_back(vector<int>(6,0));
                            new_row = P[index].size()-1;
                            P[index][new_row][0] = P[index][m][0];
                            P[index][new_row][1] = ECbr - bitrate;
                            P[index][new_row][2] = P[index][m][2];
                            P[index][new_row][3] = ECbr;
                            P[index][new_row][4] = g;
                            int F = ECchoose(ECbr, v, EC_use[v]);
                            P[index][new_row][5] = F;

                            x_lc[q][index][g] = 1;
                            y_ec[q][index][F] = 1;
                            t_lcec[q][index][g*e + F] = 1;
                            h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                            costLE += costEC;
                            costH += (hop[t][u][v] * bitrate / 100.0);
                            cost += (costEC + hop[t][u][v] * bitrate / 100.0);
                        }
                        else{//add new lc and ec
                            LCbr = ECbr;
                            minimumcard = LCbr;
                            availablecard = Capacitydecision(minimumcard, numL40[v], numL100[v], numL400[v]);
                            LCbr = availablecard;
                            costLC = costlinecard(LCbr);
                            LCnumupdate(&numL40[v], &numL100[v], &numL400[v], LCbr);

                            P[index].push_back(vector<int>(6,0));
                            new_row = P[index].size()-1;
                            P[index][new_row][0] = LCbr - bitrate;
                            P[index][new_row][1] = ECbr - bitrate;
                            P[index][new_row][2] = LCbr;
                            P[index][new_row][3] = ECbr;
                            int gg = LCchoose(LCbr, v, LC_use[v]);
                            P[index][new_row][4] = gg;
                            int F = ECchoose(ECbr, v, EC_use[v]);
                            P[index][new_row][5] = F;

                            x_lc[q][index][gg] = 1;
                            y_ec[q][index][F] = 1;
                            t_lcec[q][index][gg*e + F] = 1;
                            h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                            costLE += (costLC + costEC);
                            costH += (hop[t][u][v] * bitrate / 100.0);
                            cost += (costLC + costEC + hop[t][u][v] * bitrate / 100.0);
                        }
                    }
                }
                else{// link is safe
                    k = 0;
                    while ((P[index][k][0]<bitrate) && k<P[index].size()-1)
                        k += 1;
                    if (P[index][k][0]<bitrate)
                        k += 1;
                    if (k <= P[index].size()-1) {//choose the k-th row's lc
                        g = P[index][k][4];
                        for (w = 0; w < P[index].size(); w++) {
                            if (P[index][w][4] == g)
                                P[index][w][0] -= bitrate;//he rest of lc capacity
                        }

                        x_lc[q][index][g] = 1;
                        h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                        costH += (hop[t][u][v] * bitrate / 100.0);
                        cost += (hop[t][u][v] * bitrate / 100.0);
                    }
                    else{//add new lc
                        LCbr = LCf;
                        minimumcard = LCbr;
                        availablecard = Capacitydecision(minimumcard, numL40[v], numL100[v], numL400[v]);
                        LCbr = availablecard;
                        costLC = costlinecard(LCbr);
                        LCnumupdate(&numL40[v], &numL100[v], &numL400[v], LCbr);

                        P[index].push_back(vector<int>(6,0));
                        new_row = P[index].size()-1;
                        P[index][new_row][0] = LCbr - bitrate;
                        P[index][new_row][1] = 0;
                        P[index][new_row][2] = LCbr;
                        P[index][new_row][3] = 0;
                        int gg = LCchoose(LCbr, v, LC_use[v]);
                        P[index][new_row][4] = gg;
                        P[index][new_row][5] = 0;

                        x_lc[q][index][gg] = 1;
                        h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                        costLE += costLC;
                        costH += (hop[t][u][v] * bitrate / 100.0);
                        cost += (costLC + hop[t][u][v] * bitrate / 100.0);
                    }
                }
            }
        }
        else{//for non-important flow
            for (j=0; j<shortestPath.size()-1; j++) {
                u = shortestPath[j];
                v = shortestPath[j + 1];
                t = k_matrix[u][v];
                index = u * n + v;
                k = 0;
                while ((P[index][k][0]<bitrate) && k<P[index].size()-1)
                    k += 1;
                if (P[index][k][0]<bitrate)
                    k += 1;
                if (k <= P[index].size()-1) {//choose the k-th row's lc
                    g = P[index][k][4];
                    for (w = 0; w < P[index].size(); w++) {
                        if (P[index][w][4] == g)
                            P[index][w][0] -= bitrate;//he rest of lc capacity
                    }

                    x_lc[q][index][g] = 1;
                    h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                    costH += (hop[t][u][v] * bitrate / 100.0);
                    cost += (hop[t][u][v] * bitrate / 100.0);
                }
                else {//add new lc
                    LCbr = LCf;
                    minimumcard = LCbr;
                    availablecard = Capacitydecision(minimumcard, numL40[v], numL100[v], numL400[v]);
                    LCbr = availablecard;
                    costLC = costlinecard(LCbr);
                    LCnumupdate(&numL40[v], &numL100[v], &numL400[v], LCbr);

                    P[index].push_back(vector<int>(6, 0));
                    new_row = P[index].size() - 1;
                    P[index][new_row][0] = LCbr - bitrate;
                    P[index][new_row][1] = 0;
                    P[index][new_row][2] = LCbr;
                    P[index][new_row][3] = 0;
                    int gg = LCchoose(LCbr, v, LC_use[v]);
                    P[index][new_row][4] = gg;
                    P[index][new_row][5] = 0;

                    x_lc[q][index][gg] = 1;
                    h_c[q] += (hop[t][u][v] * bitrate / 100.0);

                    costLE += costLC;
                    costH += (hop[t][u][v] * bitrate / 100.0);
                    cost += (costLC + hop[t][u][v] * bitrate / 100.0);
                }
            }
        }
        shortestPath.clear();
    }
    cout << "cost:" << cost << endl;
    cout << "costH:" << costH << endl;
    cout << "costLE:" << costLE << endl;
}