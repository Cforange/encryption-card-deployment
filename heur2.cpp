//
// Created by Fay Chan on 2021/1/30.
//


#include <vector>
#include <def.h>
using namespace std;


double dijkstra(vector<vector<double>> G, int s, int d, vector<int> &shortestPath);

double find_min1(int pos[3], double temp1[2][l][e]){//double*** temp1,
    int min = INF;
    for(int k=0; k<2; k++)
        for(int u=0; u<l; u++)
            for(int v=0; v<e; v++)
            {
                if (temp1[k][u][v] < min)
                {
                    min = temp1[k][u][v];
                    pos[0] = k;
                    pos[1] = u;
                    pos[2] = v;
                }
            }
    return min;
}

double find_min2(int pos[2], double temp2[][l]){
    int min = INF;
    for(int k=0; k<2; k++)
        for(int u=0; u<l; u++)
            if (temp2[k][u] < min)
            {
                min = temp2[k][u];
                pos[0] = k;
                pos[1] = u;
            }
    return min;
}

void dijkstra_sub(int M_s[flow], int M_d[flow],
                  double param_1[flow][n][n][l], double param_2[flow][n][n][e],
                  double param_3[flow][n][n][l][e], double param_4[flow], int x_lc[flow][n2][l],
                  int y_ec[flow][n2][e], int t_lcec[flow][n2][LE], double h_c[flow],
                  int danger[2][n][n], int h[2][n][n], int M_bitrate[flow], vector<vector<double>> G,
                  double reducedcost[flow]){
    int i, j, u, v, k, a, z, s, d, ind;
    double min1, min2;
    int kk[n][n], xx[n][n], yy[n][n];//memset
    for (i=0; i<n; i++){//初始化
        for (j=0; j<n; j++){
            kk[i][j] = -1;
            xx[i][j] = -1;
            yy[i][j] = -1;
        }
    }

    double temp1[2][l][e];
    double temp2[2][l];

    vector<vector<double>> P = G;
    vector<int> shortestPath;
    for (i=0; i<flow; i++){
        int bitrate = M_bitrate[i];
        s = M_s[i];
        d = M_d[i];
        h_c[i] = 0;
        if (i < important_flow){
            for(u=0; u<n; u++) {
                for (v = 0; v < n; v++) {
                    // save the weight of each lc-ec

                    for (k=0; k<2; k++) {
                        for (a = 0; a < l; a++) {
                            //初始化temp1,temp2
                            temp2[k][a] = INF;
                            for (z = 0; z < e; z++) {
                                temp1[k][a][z] = INF;
                            }
                        }
                    }
                    for (k=0; k<2; k++) {
                        if (danger[k][u][v]>0) {
                            for (a = 0; a < l; a++) {
                                for (z = 0; z < e; z++) {
                                    if (Lbitrate[a] >= bitrate && Ebitrate[z] >= bitrate)
                                        temp1[k][a][z] = (param_1[i][u][v][a] + param_2[i][u][v][z] + param_3[i][u][v][a][z] + bitrate * h[k][u][v] / 100.0);

                                }
                            }
                        }
                        else{
                            for (a = 0; a < l; a++) {
                                if (Lbitrate[a]>=bitrate)
                                    temp2[k][a] = param_1[i][u][v][a] + bitrate * h[k][u][v] / 100.0;
                            }
                        }
                    }

                    //compare temp1 with temp2 to find the minimum
                    int pos1[3] = {-1, -1, -1};
                    int pos2[2] = {-1, -1};
                    min1 = find_min1(pos1, temp1);
                    min2 = find_min2(pos2, temp2);
                    if (min1 <= min2)
                    {
                        P[u][v] = min1;
                        kk[u][v] = pos1[0];
                        xx[u][v] = pos1[1];
                        yy[u][v] = pos1[2];
                    }
                    else{
                        P[u][v] = min2;
                        kk[u][v] = pos2[0];
                        xx[u][v] = pos2[1];
                    }
                }

            }
            shortestPath.clear();
            double totalcost = dijkstra(P, s, d, shortestPath);
            for (int m=0; m<shortestPath.size()-1; m++){
                u = shortestPath[m];
                v = shortestPath[m+1];
                a = xx[u][v];
                ind = u * n + v;
                x_lc[i][ind][a] = 1;
                //cout << "x_lc" << i << "," << ind/n << "," << ind%n << "," << a << endl;
                z = yy[u][v];
                if (z != -1){
                    y_ec[i][ind][z] = 1;
                    t_lcec[i][ind][a*e + z] = 1;
                    //cout << "y_ec" << i << "," << ind/n << "," << ind%n << "," << z << endl;
                }
                j = kk[u][v];
                h_c[i] += (bitrate * h[j][u][v] / 100.0);
            }
            reducedcost[i] = totalcost - param_4[i];
        }
        else{
            for(u=0; u<n; u++) {
                for (v = 0; v < n; v++) {

                    for (k = 0; k < 2; k++) {
                        for (a = 0; a < l; a++) {
                            //初始化temp2
                            temp2[k][a] = INF;
                            if (Lbitrate[a] >= bitrate)
                                temp2[k][a] = param_1[i][u][v][a] + bitrate * h[k][u][v] / 100.0;
                        }
                    }
                    //compare temp1 with temp2 to find the minimum
                    int pos2[2] = {-1, -1};
                    min2 = find_min2(pos2, temp2);
                    P[u][v] = min2;
                    kk[u][v] = pos2[0];
                    xx[u][v] = pos2[1];
                }
            }
            shortestPath.clear();
            double totalcost = dijkstra(P, s, d, shortestPath);
            for (int m=0; m<shortestPath.size()-1; m++){
                u = shortestPath[m];
                v = shortestPath[m+1];
                a = xx[u][v];
                ind = u * n + v;
                x_lc[i][ind][a] = 1;
                //cout << "x_lc" << i << "," << ind/n << "," << ind%n << "," << a << endl;
                j = kk[u][v];
                h_c[i] += (bitrate * h[j][u][v] / 100.0);
            }
            reducedcost[i] = totalcost - param_4[i];
        }
        //cout << "reducedcost"<< i << ":" << reducedcost[i] << endl;
    }
}