//
// Created by Fay Chan on 2021/1/29.
//

#ifndef CG_NEW_DEF_H
#define CG_NEW_DEF_H
#include <vector>
#include <gurobi_c++.h>
using namespace std;
//定义常数
//static const int l = 6;
//static const int e = 6;
//static const int Lbitrate[] = {40, 40, 100, 100, 400, 400};
//static const int Ebitrate[] = {40, 40, 100, 100, 400, 400};
//static const double Lcost[] = {1, 1, 2, 2, 4, 4};
//static const double Ecost[] = {2, 2, 4, 4, 8, 8};
static const int l = 10;
static const int e = 6;
static const int Lbitrate[] = {40, 40, 100, 100, 400, 400, 400, 400, 400, 400};
static const int Ebitrate[] = {40, 40, 100, 100, 400, 400};
static const double Lcost[] = {1, 1, 2, 2, 4, 4, 4, 4, 4, 4};
static const double Ecost[] = {2, 2, 4, 4, 8, 8};
static const int Lt[] = {40, 100, 400};
static const int Et[] = {40, 100, 400};
static const int ul[] = {2, 2, 6};
static const int ue[] = {2, 2, 2};
static const double INF = 65536;

static const int flow = 50;
static const int important_flow = 10;

static const int n = 14;
static const int n2 = 196;
static const int n3 = n2 * l;
static const int n4 = n2 * e;
static const int n5 = n2 * l * e;
static const int LE = l * e;
static const int fnle = flow * n5;
static const int fnl = flow * n3;
static const int fne = flow * n4;

static const int lt = 3;
static const int et = 3;
static const int nlt = n * lt;
static const int net = n * et;
static const int len = flow + n3 + n4 + fnl + n * lt + fne + n * et + n5 + fnle + n4 + n3;

double dijkstra(vector<vector<double> > G, int s, int d, vector<int> &shortestPath);
void kshortestPaths(vector<vector<double> > G, int s, int d, vector<int> &shortestPath,
                    vector<vector<int> > &shortestPaths, double totalcost[2], int k_paths);
void dangerPath(int d[2][n][n], int h[2][n][n], vector<vector<double> > G, int row, int dl[][2],
                vector<int> &shortestPath, vector<vector<int> > &shortestPaths);
void h_solve(int flow, int important_flow, int x_lc[flow][n2][l], int y_ec[flow][n2][e],
             int t_lcec[flow][n2][l*e], double h_c[flow], int M_s[flow], int M_d[flow], int M_bitrate[flow],
             int hop[2][n][n], int danger[2][n][n], vector<vector<double>> G);
//void master_problem_lp(double h_c[flow], int x_lc[flow][n2][l], int y_ec[flow][n2][e],
//                           int t_lcec[flow][n2][l * e], int M_bitarate[flow], GRBModel& m,
//                           GRBVar *lambda_ic, GRBVar *f_lc, GRBVar *g_ec, GRBVar *Q_lcec,
//                           GRBConstr *con1, GRBConstr *con2, GRBConstr *con3, GRBConstr *con4,
//                           GRBConstr *con5, GRBConstr *con6, GRBConstr *con7, GRBConstr *con8,
//                           GRBConstr *con9, GRBConstr *con10, GRBConstr *con11);
void getParam(GRBConstr con1[], GRBConstr con2[], GRBConstr con3[], GRBConstr con4[], GRBConstr con6[],
              GRBConstr con8[], GRBConstr con9[], int M_bitrate[flow], double param_1[flow][n][n][l],
              double param_2[flow][n][n][e], double param_3[flow][n][n][l][e], double param_4[flow]);
void dijkstra_sub(int M_s[flow], int M_d[flow],
                  double param_1[flow][n][n][l],double param_2[flow][n][n][e],
                  double param_3[flow][n][n][l][e], double param_4[flow], int x_lc[flow][n2][l],
                  int y_ec[flow][n2][e], int t_lcec[flow][n2][l*e], double h_c[flow],
                  int danger[2][n][n], int h[2][n][n], int M_bitrate[flow], vector<vector<double>> G,
                  double reducedcost[flow]);
void createCol(int M_bitrate[flow], int x_lc[flow][n2][l], int y_ec[flow][n2][e],
               int t_lcec[flow][n2][l * e], GRBModel& m, GRBColumn column[flow], double col[len]);

#endif //CG_NEW_DEF_H
