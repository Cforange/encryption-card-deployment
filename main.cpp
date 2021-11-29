//    cout << sizeof(M_s) << endl;//返回的是内存
//    cout << sizeof(M_s)/sizeof(M_s[0]) << endl;//返回数组长度

#include <stdio.h>
#include <vector>
#include <iostream>
#include <ctime>
#include "def.h"
#include <math.h>
#include "ThreadPool.h"

#include <gurobi_c++.h>
using namespace std;

double col[len];

int x_lc[flow][n2][l];
int y_ec[flow][n2][e];
int t_lcec[flow][n2][LE];
double h_c[flow];

GRBConstr con1[flow];
GRBConstr con2[n3];
GRBConstr con3[n4];
GRBConstr con4[fnl];
GRBConstr con5[nlt];
GRBConstr con6[fne];
GRBConstr con7[net];
GRBConstr con8[n5];
GRBConstr con9[fnle];
GRBConstr con10[n4];
GRBConstr con11[n3];

double param_1[flow][n][n][l];
double param_2[flow][n][n][e];
double param_3[flow][n][n][l][e];
double param_4[flow];

void pr1(double param_1[flow][n][n][l], int M_bitrate[flow], GRBConstr con2[], GRBConstr con4[]){
    int i, u, v, a;
    for (i=0; i<flow; i++) {
        int innl = i * n3;
        for (u = 0; u < n; u++){
            int unl = u * n * l;
            for (v = 0; v < n; v++) {
                int vl = v * l;
                for (a = 0; a < l; a++)
                    param_1[i][u][v][a] = (- M_bitrate[i] * con2[unl + vl + a].get(GRB_DoubleAttr_Pi) - con4[innl + unl + vl + a].get(GRB_DoubleAttr_Pi));
            }
        }
    }
}

void pr2(double param_2[flow][n][n][e], int M_bitrate[flow], GRBConstr con3[], GRBConstr con6[]){
    int i, u, v, a;
    for (i=0; i<flow; i++) {
        int inne = i * n4;
        for (u = 0; u < n; u++){
            int une = u * n * e;
            for (v = 0; v < n; v++) {
                int ve = v * e;
                for (a = 0; a < e; a++)
                    param_2[i][u][v][a] = (- M_bitrate[i] * con3[une + ve + a].get(GRB_DoubleAttr_Pi) - con6[inne + une + ve + a].get(GRB_DoubleAttr_Pi));
            }
        }
    }
}

void pr3(double param_3[flow][n][n][l][e], GRBConstr con8[], GRBConstr con9[]){
    int i, u, v, a, z;
    for (i=0; i<flow; i++) {
        int innle = i * n5;
        for (u = 0; u < n; u++){
            int unle = u * n * l * e;
            for (v = 0; v < n; v++) {
                int vle = v * l * e;
                for (a = 0; a < l; a++){
                    int ae = a * e;
                    for (z = 0; z < e; z++)
                        param_3[i][u][v][a][z] = (- con9[innle + unle + vle + ae + z].get(GRB_DoubleAttr_Pi) + con8[unle + vle + ae + z].get(GRB_DoubleAttr_Pi));
                }

            }
        }
    }
}

void pr4(double param_4[flow], GRBConstr con1[]){
    int i;
    for (i=0; i<flow; i++) {
        param_4[i] = con1[i].get(GRB_DoubleAttr_Pi);
    }
}

void fun1(int M_bitrate[flow], int x_lc[flow][n2][l], double col[len], int i, int k){//k是开始的索引值
    int u, v, a, un, unv;
    //con2: n n l
    for (u=0; u<n; u++){
        un = u*n;
        for (v=0; v<n; v++){
            unv = un+v;
            for (a=0; a<l; a++){
                col[k] = x_lc[i][unv][a] * M_bitrate[i];
                k++;
            }
        }
    }
}

void fun2(int M_bitrate[flow], int y_ec[flow][n2][e], double col[len], int i, int k){
    int u, v, a, un, unv;
    //con3:n n e
    for (u=0; u<n; u++){
        un = u*n;
        for (v=0; v<n; v++){
            unv = un+v;
            for (a=0; a<e; a++){
                col[k] = y_ec[i][unv][a] * M_bitrate[i];
                k++;
            }
        }
    }
}

void fun3(int x_lc[flow][n2][l], double col[len], int i, int k){
    int j, u, v, a, un, unv;
    //con4:flow n n l
    for (j=0; j<flow; j++){
        if (j == i){
            for (u=0; u<n; u++){
                un = u*n;
                for (v=0; v<n; v++){
                    unv = un+v;
                    for (a=0; a<l; a++){
                        col[k] = x_lc[i][unv][a];
                        k++;
                    }
                }
            }
        }
        else{
            k += n3;
        }
    }
}

void fun4(int y_ec[flow][n2][e], double col[len], int i, int k){
    int j, u, v, a, un, unv;
    //con6:flow n n e
    for (j=0; j<flow; j++){
        if (j == i){
            for (u=0; u<n; u++){
                un = u*n;
                for (v=0; v<n; v++){
                    unv = un+v;
                    for (a=0; a<e; a++){
                        col[k] = y_ec[i][unv][a];
                        k++;
                    }
                }
            }
        }
        else{
            k += n4;
        }
    }
}

void fun5(int t_lcec[flow][n2][LE], double col[len], int i, int k){
    int u, v, a, z, un, unv;
    //con8:n n l e
    for (u=0; u<n; u++){
        un = u*n;
        for (v=0; v<n; v++){
            unv = un+v;
            for (a=0; a<l; a++){
                int ae = a*e;
                for (z=0; z<e; z++){
                    col[k] = t_lcec[i][unv][ae + z];
                    k++;
                }
            }
        }
    }
}

void fun6(int t_lcec[flow][n2][LE], double col[len], int i, int k){
    int j, u, v, a, z, un, unv, ae;
    //con9:flow n n l e
    for (j=0; j<flow; j++){
        if (j == i){
            for (u=0; u<n; u++){
                un = u*n;
                for (v=0; v<n; v++){
                    unv = un+v;
                    for (a=0; a<l; a++){
                        ae = a*e;
                        for (z=0; z<e; z++){
                            col[k] = t_lcec[i][unv][ae + z];
                            k++;
                        }
                    }
                }
            }
        }
        else{
            k += n5;

        }
    }
}

int main(){
    int i, j, k = 0;
    double g[n][n]  =  {{INF, 1, 1, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
                        {1, INF, 1, INF, INF, INF, INF, 1, INF, INF, INF, INF, INF, INF},
                        {1, 1, INF, INF, INF, 1, INF, INF, INF, INF, INF, INF, INF, INF},
                        {1, INF, INF, INF, 1, INF, INF, INF, INF, 1, INF, INF, INF, INF},
                        {INF, INF, INF, 1, INF, 1, 1, INF, INF, INF, INF, INF, INF, INF},
                        {INF, INF, 1, INF, 1, INF, INF, INF, 1, INF, INF, INF, 1, INF},
                        {INF, INF, INF, INF, 1, INF, INF, 1, INF, INF, INF, INF, INF, INF},
                        {INF, 1, INF, INF, INF, INF, 1, INF, INF, INF, 1, INF, INF, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, INF, INF, 1, INF, INF, INF},
                        {INF, INF, INF, 1, INF, INF, INF, INF, INF, INF, INF, 1, INF, 1},
                        {INF, INF, INF, INF, INF, INF, INF, 1, 1, INF, INF, 1, INF, 1},
                        {INF, INF, INF, INF, INF, INF, INF, INF, INF, 1, 1, INF, 1, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, INF, INF, INF, 1, INF, 1},
                        {INF, INF, INF, INF, INF, INF, INF, INF, INF, 1, 1, INF, 1, INF}};
//    double g[n][n] = {{INF, 1, INF, 1, INF, INF},
//                      {1, INF, 1, 1, INF, INF},
//                      {INF, 1, INF, INF, 1, 1},
//                      {1, 1, INF, INF, 1, INF},
//                      {INF, INF, 1, 1, INF, 1},
//                      {INF, INF, 1, INF, 1, INF}};
    vector<vector<double> >  G(n, vector<double> (n));//初始化
    for (i=0;i<n; i++){
        for(j=0; j<n; j++){
            G[i][j] = g[i][j];
        }
    }
//    double test1[38000];

    const int row = 20;
    int dl[row][2] = {{1,7},
                      {2,5},
                      {3,9},
                      {4,5},
                      {4,6},
                      {5,2},
                      {5,4},
                      {5,8},
                      {5,12},
                      {6,4},
                      {6,7},
                      {7,1},
                      {7,6},
                      {7,10},
                      {8,5},
                      {8,10},
                      {9,3},
                      {10,7},
                      {10,8},
                      {12,5}};
//    const int row = 4;
//    int dl[][2] = {{1,2},
//                   {2,1},
//                   {3,4},
//                   {4,5}};
    int d[2][n][n];
    int h[2][n][n];
    //初始化为0
    memset(d, 0, sizeof(d));
    memset(h, 0, sizeof(h));
    vector<int> shortestPath;
    vector<vector<int> > shortestPaths;
//    double cost = dijkstra(G, 1, 0, shortestPath);
//    double totalcost[2];
//    kshortestPaths(G, 0, 13,shortestPath,shortestPaths, totalcost, 2);
//    cout << totalcost[0] << totalcost[1]<< endl;
    dangerPath(d, h, G, row, dl, shortestPath, shortestPaths);//基本没有问题了。
//    for (i=0;i<n; i++){
//        for(j=0; j<n; j++){
//            for(k=0; k<2; k++){
//                cout << d[k][i][j] << endl;
//            }
//        }
//    }
//    for (i=0;i<n; i++){
//        for(j=0; j<n; j++){
//            for(k=0; k<2; k++){
//                cout << h[k][i][j] << endl;
//            }
//        }
//    }
    //随机生成流<src, des, bits>
    int M_s[flow] = {0};
    int M_d[flow] = {0};
    int M_bitrate[flow] = {0};
    int bandwidth = 0;
    int LEN = 11;
    int choice[] = {50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
    srand((unsigned )time(NULL));//设置种子
    i=0;
    while(i < flow){
        int temp1 = 0, temp2 = 0, temp3, temp4;
        while(temp1 == temp2){
            temp1 = rand() % n;
            temp2 = rand() % n;
        }
        temp3 = choice[rand() % LEN];
        temp4 = choice[rand() % LEN];
        M_s[i] = temp1;
        M_d[i] = temp2;
        M_bitrate[i] = temp3;
        i++;
        if (i >= flow)
            break;
        M_s[i] = temp1;
        M_d[i] = temp2;
        M_bitrate[i] = temp4;
        i++;
        if (i >= flow)
            break;
        M_s[i] = temp1;
        M_d[i] = temp2;
        M_bitrate[i] = 400 - temp3 - temp4;
        i++;
        if (i >= flow)
            break;
        bandwidth += M_bitrate[i];
    }

    //启发式算法
//    int x_lc[flow][n2][l] = {0};
//    int y_ec[flow][n2][e] = {0};
//    int t_lcec[flow][n2][LE] = {0};
//    double h_c[flow];
    //初始化
    memset(x_lc, 0, sizeof(x_lc));
    memset(y_ec, 0, sizeof(y_ec));
    memset(t_lcec, 0, sizeof(t_lcec));
    memset(h_c, 0, sizeof(h_c));
    h_solve(flow, important_flow, x_lc, y_ec, t_lcec, h_c, M_s, M_d, M_bitrate, h, d, G);
    cout << "bitrate" << endl;
    for (i=0;i<flow; i++){
        cout << M_bitrate[i] << endl;
    }
    cout << "x_lc" << endl;
    for (i=0;i<flow; i++){
        for(j=0; j<n2; j++){
            for(k=0; k<l; k++){
                if (x_lc[i][j][k] != 0)
                    printf("%d,%d,%d,%d\n",i,j/n,j%n,k);
            }
        }
    }
    cout << "y_ec" << endl;
    for (i=0;i<flow; i++){
        for(j=0; j<n2; j++){
            for(k=0; k<e; k++){
                if (y_ec[i][j][k] != 0)
                    printf("%d,%d,%d,%d\n",i,j/n,j%n,k);
            }
        }
    }
    //初始化变量和约束
    //create an environment
    GRBEnv* env = 0;
    GRBVar lambda_ic[flow];
    GRBVar f_lc[n3];
    GRBVar g_ec[n4];
    GRBVar Q_lcec[n5];
    int status;
    try{

        env = new GRBEnv();
        //create an empty model
        GRBModel m = GRBModel(*env);

//        GRBConstr con1[flow];
//        GRBConstr con2[n3];
//        GRBConstr con3[n4];
//        GRBConstr con4[fnl];
//        GRBConstr con5[nlt];
//        GRBConstr con6[fne];
//        GRBConstr con7[net];
//        GRBConstr con8[n5];
//        GRBConstr con9[fnle];
//        GRBConstr con10[n4];
//        GRBConstr con11[n3];

//        master_problem_lp(h_c, x_lc, y_ec, t_lcec, M_bitrate, m, lambda_ic, f_lc, g_ec, Q_lcec, con1, con2, con3, con4, con5, con6, con7, con8,
//                          con9, con10, con11);
        //变量定义
        for (i=0; i<flow; i++){
            lambda_ic[i] = m.addVar(0, 1, h_c[i], GRB_CONTINUOUS);
        }
        for (i=0; i<n2; i++){
            for (j=0; j<l; j++){
                f_lc[i*l+j] = m.addVar(0, 1, 2*Lcost[j], GRB_CONTINUOUS);
            }
        }
        for (i=0; i<n2; i++){
            for (j=0; j<e; j++){
                g_ec[i*e+j] = m.addVar(0, 1, 2*Ecost[j], GRB_CONTINUOUS);
            }
        }
        for (i=0; i<n2; i++){
            for (j=0; j<LE; j++){
                Q_lcec[i*LE+j] = m.addVar(0, 1, 0, GRB_CONTINUOUS);
            }
        }
        //set objective
        m.set(GRB_IntAttr_ModelSense, 1);
        //add constraints to the model
        //con1:flow
        int a, u, v, z;
        int jl, je, kLE;
        GRBLinExpr temp = 0;
        for (i=0; i<flow; i++)
        {
            con1[i] = m.addConstr(lambda_ic[i] == 1);
        }
        //con2:n n l
        for (j=0; j<n2; j++){
            jl = j * l;
            for (k=0; k<l; k++)
            {
                temp = 0;
                for (i=0; i<flow; i++)
                {
                    temp += (x_lc[i][j][k] * M_bitrate[i] * lambda_ic[i]);
                }
                con2[jl + k] = m.addConstr(temp <= Lbitrate[k]);
            }
        }

        //con3:n n e
        for (j=0; j<n2; j++){
            je = j * e;
            for (k=0; k<e; k++)
            {
                temp = 0;
                for (i=0; i<flow; i++)
                {
                    temp += (y_ec[i][j][k] * M_bitrate[i] * lambda_ic[i]);
                }
                con3[je + k] = m.addConstr(temp <= Ebitrate[k]);
            }
        }

        //con4：flow n n l
        for (i=0; i<flow; i++){
            int in3 = i * n3;
            for (j=0; j<n2; j++){
                jl = j * l;
                for (k=0; k<l; k++){
                    con4[in3 + jl + k] = m.addConstr(x_lc[i][j][k] * lambda_ic[i] - f_lc[j*l + k] <= 0);
                }
            }

        }

        //con5:n lt
        for (v=0; v<n; v++){
            int vlt = v * lt;
            int vl = v * l;
            for (a=0; a<lt; a++){
                temp = 0;
                for (u=0; u<n; u++){
                    int unl = u * n * l;
                    for (z=0; z<l; z++)
                    {
                        if (Lbitrate[z] == Lt[a])
                            temp += f_lc[unl + vl + z];
                    }
                }
                con5[vlt + a] = m.addConstr(temp <= ul[a]);
            }
        }

        //con6:flow n n e
        for (i=0; i<flow; i++){
            int in4 = i * n4;
            for (j=0; j<n2; j++){
                je = j * e;
                for (k=0; k<e; k++){
                    con6[in4 + je + k] = m.addConstr(y_ec[i][j][k] * lambda_ic[i] - g_ec[je + k] <= 0);
                }
            }

        }

        //con7:n et
        for (v=0; v<n; v++){
            int ve = v * e;
            int vet = v * et;
            for (a=0; a<et; a++)
            {
                temp = 0;
                for (u=0; u<n; u++){
                    int une = u * n * e;
                    for (z=0; z<e; z++){
                        if (Ebitrate[z] == Et[a])
                            temp += g_ec[une + ve + z];
                    }
                }
                con7[vet + a] = m.addConstr(temp <= ue[a]);
            }
        }

        //con8:n n l e
        for (k=0; k<n2; k++){
            kLE = k * LE;
            for(j=0; j<LE; j++)
            {
                temp = 0;
                for (i=0; i<flow; i++){
                    temp += (t_lcec[i][k][j] * lambda_ic[i]);
                }
                con8[kLE + j] = m.addConstr(Q_lcec[kLE + j] - temp <= 0);
            }
        }

        //con9:flow n n l e
        for (i=0; i<flow; i++){
            int in5 = i * n5;
            for (k=0; k<n2; k++){
                kLE = k * LE;
                for(j=0; j<LE; j++){
                    con9[in5 + kLE + j] = m.addConstr(t_lcec[i][k][j] * lambda_ic[i] - Q_lcec[kLE + j] <= 0);
                }
            }

        }

        //con10:n n e
        for (k=0; k<n2; k++){
            kLE = k * LE;
            int ke = k * e;
            for (z=0; z<e; z++){
                temp = 0;
                for (a=0; a<l; a++)
                    temp += Q_lcec[kLE + a*e + z];
                con10[ke + z] = m.addConstr(temp <= 1);
            }
        }

        //con11: n n l
        for (k=0; k<n2; k++){
            kLE = k * LE;
            int kl = k * l;
            for (a=0; a<l; a++)
            {
                temp = 0;
                int ae = a * e;
                for (z=0; z<e; z++)
                    temp += (Q_lcec[kLE + ae + z] * Ebitrate[z]);
                con11[kl + a] = m.addConstr(temp <= Lbitrate[a]);
            }
        }
        clock_t start, end;
        start = clock();
        m.optimize();
        cout << "m result:" << m.get(GRB_DoubleAttr_ObjVal) << endl;

        //从主问题中求的对偶解
//        double param_1[flow][n][n][l] = {0};
//        double param_2[flow][n][n][e] = {0};
//        double param_3[flow][n][n][l][e] = {0};
//        double param_4[flow] = {0};

//        double dual_1[flow];
//        for (i=0; i<flow; i++)
//        {
//            dual_1[i] = con1[i].get(GRB_DoubleAttr_Pi);
//        }
//        cout << "f_lc" << endl;
//        for (i=0; i<n2; i++){
//            for (j=0; j<l; j++){
//                if (f_lc[i*l+j].get(GRB_DoubleAttr_X) != 0)
//                    cout << i/n << "," << i%n << "," << j << "," << f_lc[i*l+j].get(GRB_DoubleAttr_X) << endl;
//            }
//        }
//        cout << "g_ec" << endl;
//        for (i=0; i<n2; i++){
//            for (j=0; j<e; j++){
//                if (g_ec[i*l+j].get(GRB_DoubleAttr_X) != 0)
//                    cout << i/n << "," << i%n << "," << j << "," << g_ec[i*l+j].get(GRB_DoubleAttr_X) << endl;
//            }
//        }
        ThreadPool pool(4);
        clock_t ss, ee;
        pool.enqueue(pr1, param_1, M_bitrate, con2, con4);
        pool.enqueue(pr2, param_2, M_bitrate, con3, con6);
        pool.enqueue(pr3, param_3, con8, con9);
        pool.enqueue(pr4, param_4, con1);
//        ee = clock();
//        cout << "time_1 :" << (double) (ee - ss)/CLOCKS_PER_SEC << endl;
//        ss = clock();
//        for (i=0; i<flow; i++) {
//            int innl = i * n3;
//            int inne = i * n4;
//            int innle = i * n5;
//            for (u = 0; u < n; u++){
//                int unl = u * n * l;
//                int une = u * n * e;
//                int unle = u * n * l * e;
//                for (v = 0; v < n; v++) {
//                    int vl = v * l;
//                    int ve = v * e;
//                    int vle = v * l * e;
//                    for (a = 0; a < l; a++)
//                        param_1[i][u][v][a] = (- M_bitrate[i] * con2[unl + vl + a].get(GRB_DoubleAttr_Pi) - con4[innl + unl + vl + a].get(GRB_DoubleAttr_Pi));
//
//                    for (a = 0; a < e; a++)
//                        param_2[i][u][v][a] = (- M_bitrate[i] * con3[une + ve + a].get(GRB_DoubleAttr_Pi) - con6[inne + une + ve + a].get(GRB_DoubleAttr_Pi));
//
//                    for (a = 0; a < l; a++){
//                        int ae = a * e;
//                        for (z = 0; z < e; z++)
//                            param_3[i][u][v][a][z] = (- con9[innle + unle + vle + ae + z].get(GRB_DoubleAttr_Pi) + con8[unle + vle + ae + z].get(GRB_DoubleAttr_Pi));
//                    }
//
//                }
//            }
//            param_4[i] = con1[i].get(GRB_DoubleAttr_Pi);
//        }
//        ee = clock();
//        cout << "time_2 :" << (double) (ee - ss)/CLOCKS_PER_SEC << endl;
////        getParam(con1, con2, con3, con4, con6, con8, con9, M_bitrate, param_1, param_2, param_3, param_4);
//
//        cout << "param1" << endl;
//        for (i=0;i<flow; i++){
//            for(j=0; j<n; j++){
//                for(k=0; k<n; k++){
//                    for(a=0; a<l; a++){
//                        if (param_1[i][j][k][a] != 0)
//                            printf("%d,%d,%d,%d,%f\n",i,j,k,a,param_1[i][j][k][a]);
//                    }
//
//                }
//            }
//        }
//        cout << "param2" << endl;
//        for (i=0;i<flow; i++){
//            for(j=0; j<n; j++){
//                for(k=0; k<n; k++){
//                    for(a=0; a<e; a++){
//                        if (param_2[i][j][k][a] != 0)
//                            printf("%d,%d,%d,%d,%f\n",i,j,k,a,param_2[i][j][k][a]);
//                    }
//
//                }
//            }
//        }
//        cout << "param3" << endl;
//        for (i=0;i<flow; i++){
//            for(j=0; j<n; j++){
//                for(k=0; k<n; k++){
//                    for(a=0; a<l; a++){
//                        for(z=0; z<e; z++) {
//                            if (param_3[i][j][k][a][z] != 0)
//                                printf("%d,%d,%d,%d,%d,%f\n", i, j, k, a, z, param_3[i][j][k][a][z]);
//                        }
//                    }
//
//                }
//            }
//        }
//        cout << "param4" << endl;
//        for (i=0;i<flow; i++){
//            printf("%f\n",param_4[i]);
//        }
        //求解子问题
        double reducedcost[flow] = {0};
        memset(x_lc, 0, sizeof(x_lc));
        memset(y_ec, 0, sizeof(y_ec));
        memset(t_lcec, 0, sizeof(t_lcec));
        memset(h_c, 0, sizeof(h_c));
        dijkstra_sub(M_s, M_d, param_1, param_2, param_3, param_4, x_lc, y_ec, t_lcec,
                     h_c, d, h, M_bitrate, G, reducedcost);
        cout << "x_lc" << endl;
        for (i=0;i<flow; i++){
            for(j=0; j<n2; j++){
                for(k=0; k<l; k++){
                    if (x_lc[i][j][k] != 0)
                        printf("%d,%d,%d,%d\n",i,j/n,j%n,k);
                }
            }
        }
        cout << "y_ec" << endl;
        for (i=0;i<flow; i++){
            for(j=0; j<n2; j++){
                for(k=0; k<e; k++){
                    if (y_ec[i][j][k] != 0)
                        printf("%d,%d,%d,%d\n",i,j/n,j%n,k);
                }
            }
        }
        GRBColumn column[flow];
//        double col[len];
//        double* col = (double* )malloc(len * sizeof(double));
        clock_t s1, e1, s2, e2;
        s1 = clock();
//        createCol(M_bitrate, x_lc, y_ec, t_lcec, m, column, col);
//        e1 = clock();
//        cout << "the run time of createcol_1 :" << (double) (e1 - s1)/CLOCKS_PER_SEC << endl;
        //createCol with multithread
        //s1 = clock();
        for (i=0; i<flow; i++) {
            memset(col, 0, len * sizeof(double) / sizeof(char));
            k = 0;
            //con1：flow
            for (j=0; j<flow; j++){
                if (j == i){
                    col[k] = 1;
                }
                k++;
            }
            //con2: n n l
            pool.enqueue(fun1, M_bitrate, x_lc, col, i, k);
            //con3: n n e
            k += n3;
            pool.enqueue(fun2, M_bitrate, y_ec, col, i, k);
            //con4: flow n n l
            k += n4;
            pool.enqueue(fun3, x_lc, col, i, k);
            //con5: n lt 均为0
            //con6: flow n n e
            k += (fnl + nlt);
            pool.enqueue(fun4, y_ec, col, i, k);
            //con7: n et 均为0
            //con8: n n l e
            k += (fne + net);
            pool.enqueue(fun5, t_lcec, col, i, k);
            //con9: flow n n l e
            k += n5;
            pool.enqueue(fun6, t_lcec, col, i, k);
            column[i].clear();
            column[i].addTerms(col, m.getConstrs(), len);
        }
        e1 = clock();
        cout << "the run time of createcol :" << (double) (e1 - s1)/CLOCKS_PER_SEC << endl;
        double ub[flow];
        double lb[flow] = {0};
        char type[flow];
        for (i=0; i<flow; i++){
            ub[i] = 1;
            type[i] = GRB_CONTINUOUS;
        }
        //cout << "m result:" << m.get(GRB_DoubleAttr_ObjVal) <<endl;
        s2 = clock();
        m.addVars(lb, ub, h_c, type, NULL, column, flow);
        e2 = clock();
        cout << "the run time of addvar :" << (double) (e2 - s2)/CLOCKS_PER_SEC << endl;
        int iteration_time = 1;
        cout << "iteration_time:" << iteration_time << endl;

        //开始迭代过程
        while (iteration_time <= 50){
            //求reducedcost最小值
            double min = *min_element(reducedcost, reducedcost + flow);
            if (min <= -pow(10, -4)){
                m.optimize();
//                getParam(con1, con2, con3, con4, con6, con8, con9, M_bitrate, param_1, param_2, param_3, param_4);
                pool.enqueue(pr1, param_1, M_bitrate, con2, con4);
                pool.enqueue(pr2, param_2, M_bitrate, con3, con6);
                pool.enqueue(pr3, param_3, con8, con9);
                pool.enqueue(pr4, param_4, con1);

//                for (i=0; i<flow; i++) {
//                    int innl = i * n3;
//                    int inne = i * n4;
//                    int innle = i * n5;
//                    for (u = 0; u < n; u++){
//                        int unl = u * n * l;
//                        int une = u * n * e;
//                        int unle = u * n * l * e;
//                        for (v = 0; v < n; v++) {
//                            int vl = v * l;
//                            int ve = v * e;
//                            int vle = v * l * e;
//                            for (a = 0; a < l; a++)
//                                param_1[i][u][v][a] = (- M_bitrate[i] * con2[unl + vl + a].get(GRB_DoubleAttr_Pi) - con4[innl + unl + vl + a].get(GRB_DoubleAttr_Pi));
//
//                            for (a = 0; a < e; a++)
//                                param_2[i][u][v][a] = (- M_bitrate[i] * con3[une + ve + a].get(GRB_DoubleAttr_Pi) - con6[inne + une + ve + a].get(GRB_DoubleAttr_Pi));
//
//                            for (a = 0; a < l; a++){
//                                int ae = a * e;
//                                for (z = 0; z < e; z++)
//                                    param_3[i][u][v][a][z] = (- con9[innle + unle + vle + ae + z].get(GRB_DoubleAttr_Pi) + con8[unle + vle + ae + z].get(GRB_DoubleAttr_Pi));
//                            }
//
//                        }
//                    }
//                    param_4[i] = con1[i].get(GRB_DoubleAttr_Pi);
//                }

//                cout << "param1" << endl;
//                for (i=0;i<flow; i++){
//                    for(j=0; j<n; j++){
//                        for(k=0; k<n; k++){
//                            for(a=0; a<l; a++){
//                                if (param_1[i][j][k][a] != 0)
//                                    printf("%d,%d,%d,%d,%f\n",i,j,k,a,param_1[i][j][k][a]);
//                            }
//
//                        }
//                    }
//                }
//                cout << "param2" << endl;
//                for (i=0;i<flow; i++){
//                    for(j=0; j<n; j++){
//                        for(k=0; k<n; k++){
//                            for(a=0; a<e; a++){
//                                if (param_2[i][j][k][a] != 0)
//                                    printf("%d,%d,%d,%d,%f\n",i,j,k,a,param_2[i][j][k][a]);
//                            }
//
//                        }
//                    }
//                }
//                cout << "param3" << endl;
//                for (i=0;i<flow; i++){
//                    for(j=0; j<n; j++){
//                        for(k=0; k<n; k++){
//                            for(a=0; a<l; a++){
//                                for(z=0; z<e; z++) {
//                                    if (param_3[i][j][k][a][z] != 0)
//                                        printf("%d,%d,%d,%d,%d,%f\n", i, j, k, a, z, param_3[i][j][k][a][z]);
//                                }
//                            }
//
//                        }
//                    }
//                }
//                cout << "param4" << endl;
//                for (i=0;i<flow; i++){
//                    printf("%f\n",param_4[i]);
//                }

                memset(x_lc, 0, sizeof(x_lc));
                memset(y_ec, 0, sizeof(y_ec));
                memset(t_lcec, 0, sizeof(t_lcec));
                memset(h_c, 0, sizeof(h_c));
                dijkstra_sub(M_s, M_d, param_1, param_2, param_3, param_4, x_lc, y_ec, t_lcec,
                             h_c, d, h, M_bitrate, G, reducedcost);
                s1 = clock();
                for (i=0; i<flow; i++) {
                    memset(col, 0, len * sizeof(double) / sizeof(char));
                    k = 0;
                    //con1：flow
                    for (j=0; j<flow; j++){
                        if (j == i){
                            col[k] = 1;
                        }
                        k++;
                    }
                    //con2: n n l
                    pool.enqueue(fun1, M_bitrate, x_lc, col, i, k);
                    //con3: n n e
                    k += n3;
                    pool.enqueue(fun2, M_bitrate, y_ec, col, i, k);
                    //con4: flow n n l
                    k += n4;
                    pool.enqueue(fun3, x_lc, col, i, k);
                    //con5: n lt 均为0
                    //con6: flow n n e
                    k += (fnl + nlt);
                    pool.enqueue(fun4, y_ec, col, i, k);
                    //con7: n et 均为0
                    //con8: n n l e
                    k += (fne + net);
                    pool.enqueue(fun5, t_lcec, col, i, k);
                    //con9: flow n n l e
                    k += n5;
                    pool.enqueue(fun6, t_lcec, col, i, k);
                    column[i].clear();
                    column[i].addTerms(col, m.getConstrs(), len);
                }
//                createCol(M_bitrate, x_lc, y_ec, t_lcec, m, column, col);
                e1 = clock();
                cout << "the run time of createcol :" << (double) (e1 - s1)/CLOCKS_PER_SEC << endl;
                s2 = clock();
                m.addVars(lb, ub, h_c, type, NULL, column, flow);
                e2 = clock();
                cout << "the run time of addvar :" << (double) (e2 - s2)/CLOCKS_PER_SEC << endl;
                iteration_time += 1;
                cout << "iteration_time:" << iteration_time << endl;
            }
            else{
                break;
            }
        }
        end = clock();
        cout << "the run time of cg_lp :" << (double) (end - start)/CLOCKS_PER_SEC << endl;
        //求解ilp-mp
        int length = m.get(GRB_IntAttr_NumVars);
        char type2[length];
        for (i=0; i<length; i++){
            type2[i] = GRB_BINARY;
        }
        m.set(GRB_CharAttr_VType, m.getVars(), type2, length);
        m.optimize();
        end = clock();
        cout << "the run time of cg :" << (double) (end - start)/CLOCKS_PER_SEC << endl;
        cout << "m final result:" << m.get(GRB_DoubleAttr_ObjVal) <<endl;
    }
    catch (GRBException e){
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (bad_alloc){
        cout << "bad alloc " << endl;
    }
    catch (...)
    {
        cout << "Exception during optimization" << endl;
    }


    return 0;
}
