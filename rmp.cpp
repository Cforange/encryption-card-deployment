//
// Created by Fay Chan on 2021/1/30.
//

/* Copyright 2020, Gurobi Optimization, LLC */

/* Solve the classic diet model, showing how to add constraints
   to an existing model. */

#include "gurobi_c++.h"
#include <def.h>

using namespace std;

//void master_problem_lp(double h_c[flow], int x_lc[flow][n2][l], int y_ec[flow][n2][e],
//                           int t_lcec[flow][n2][l * e], int M_bitarate[flow], GRBModel& m,
//                           GRBVar *lambda_ic, GRBVar *f_lc, GRBVar *g_ec, GRBVar *Q_lcec,
//                           GRBConstr *con1, GRBConstr *con2, GRBConstr *con3, GRBConstr *con4,
//                           GRBConstr *con5, GRBConstr *con6, GRBConstr *con7, GRBConstr *con8,
//                           GRBConstr *con9, GRBConstr *con10, GRBConstr *con11){
//    try{
//        //const double lb[] = {0};
//        const double ub[] = {1};
//        //const double obj[] = {0};
//        int i = 0;
//        int j = 0;
//        int k = 0;
//        int u = 0;
//        int v = 0;
//        int a = 0;
//        int z = 0;
//        GRBLinExpr temp = 0;
//
//        //column 初始为1，（i，c）in {(0,0),(1,0),...,(flow-1,0)},因此所有可以取消对所有column的求和
//        //create variables, and set the objective
//        lambda_ic = m.addVars(0, ub, h_c, &(C), NULL, (flow));
//        f_lc = m.addVars(0, ub, Lcost, &(C), NULL, (n*n*l));
//        g_ec = m.addVars(0, ub, Ecost, &(C), NULL, (n*n*e));
//        Q_lcec = m.addVars(0, ub, 0, &(C), NULL, (n * n * l * e));
//
//        //set objective
//        m.set(GRB_IntAttr_ModelSense, 1);
//
//        //add constraints to the model
//        //con1
//        for (i=0; i<flow; i++)
//        {
//            con1[i] = m.addConstr(lambda_ic[i] == 1);
//        }
//        //con2
//        for (j=0; j<n*n; j++)
//            for (k=0; k<l; k++)
//            {
//                temp = 0;
//                for (i=0; i<flow; i++)
//                {
//                    temp += (x_lc[i][j][k] * M_bitarate[i] * lambda_ic[i]);
//                }
//                con2[j * l + k] = m.addConstr(temp <= Lbitrate[k]);
//            }
//        //con3
//        for (j=0; j<n*n; j++)
//            for (k=0; k<e; k++)
//            {
//                for (i=0; i<flow; i++)
//                {
//                    temp += (y_ec[i][j][k] * M_bitarate[i] * lambda_ic[i]);
//                }
//                con3[j * l + k] = m.addConstr(temp <= Ebitrate[k]);
//            }
//        //con4
//        for (i=0; i<flow; i++)
//            for (j=0; j<n*n; j++)
//                for (k=0; k<l; k++)
//                {
//                    con4[i*n*n*l + j*l + k] = m.addConstr(x_lc[i][j][k] * lambda_ic[i] - f_lc[j*l + k] <= 0);
//                }
//        //con5
//        for (v=0; v<n; v++)
//            for (a=0; a<lt; a++)
//            {
//                temp = 0;
//                for (u=0; u<n; u++)
//                    for (z=0; z<l; z++)
//                    {
//                        if (Lbitrate[z] == Lt[a])
//                            temp += f_lc[u*n*l + v*l + z];
//                    }
//                con5[v*lt + a] = m.addConstr(temp <= ul[a]);
//            }
//        //con6
//        for (i=0; i<flow; i++)
//            for (j=0; j<n*n; j++)
//                for (k=0; k<e; k++)
//                {
//                    con6[i*n*n*e + j*e + k] = m.addConstr(y_ec[i][j][k] * lambda_ic[i] - g_ec[j*e + k] <= 0);
//                }
//        //con7
//        for (v=0; v<n; v++)
//            for (a=0; a<et; a++)
//            {
//                temp = 0;
//                for (u=0; u<n; u++)
//                    for (z=0; z<e; z++)
//                    {
//                        if (Ebitrate[z] == Et[a])
//                            temp += g_ec[u*n*e + v*e + z];
//                    }
//                con7[v*et + a] = m.addConstr(temp <= ue[a]);
//            }
//        //con8
//        for (k=0; k<n*n; k++)
//            for(j=0; j<l*e; j++)
//            {
//                temp = 0;
//                for (i=0; i<flow; i++)
//                    temp += (t_lcec[i][k][j] * lambda_ic[i]);
//                con8[k*l*e + j] = m.addConstr(temp - Q_lcec[k*l*e + j] >= 0);
//            }
//        //con9
//        for (i=0; i<flow; i++)
//            for (k=0; k<n*n; k++)
//                for(j=0; j<l*e; j++)
//                {
//                    con9[i*n*n*l*e + k*l*e + j] = m.addConstr(t_lcec[i][k][j] * lambda_ic[i] - Q_lcec[k*l*e + j]
//                                                              <= 0);
//                }
//        //con10
//        for (k=0; k<n*n; k++)
//            for (z=0; z<e; z++)
//            {
//                temp = 0;
//                for (a=0; a<l; a++)
//                    temp += Q_lcec[k*l*e + a*e + z];
//                con10[k*l*e + a*e + z] = m.addConstr(temp <= 1);
//            }
//        //con11
//        for (k=0; k<n*n; k++)
//            for (a=0; a<l; a++)
//            {
//                temp = 0;
//                for (z=0; z<e; z++)
//                    temp += (Q_lcec[k*l*e + a*e + z] * Ebitrate[z]);
//                con11[k*l*e + a*e + z] = m.addConstr(temp <= Lbitrate[a]);
//            }
//    }
//    catch (GRBException e) {
//        cout << "Error code = " << e.getErrorCode() << endl;
//        cout << e.getMessage() << endl;
//    }
//    catch (...) {
//        cout << "Exception during optimization" << endl;
//    }
//
//}

//void getParam(GRBConstr con1[], GRBConstr con2[], GRBConstr con3[], GRBConstr con4[], GRBConstr con6[],
//              GRBConstr con8[], GRBConstr con9[], int M_bitrate[flow], double param_1[flow][n][n][l],
//              double param_2[flow][n][n][e], double param_3[flow][n][n][l][e], double param_4[flow]){
//    //get the dual value
//    double dual_1[flow];
//    double dual_2[n3];
//    double dual_3[n4];
//    double dual_4[fnl];
//    double dual_5[fne];
//    double dual_6[n5];
//    double dual_7[fnle];
//    int i=0,k=0,u=0,v=0,a=0,z = 0;
//
//    for (i=0; i<flow; i++)
//    {
//        dual_1[i] = con1[i].get(GRB_DoubleAttr_Pi);
//    }
//
//    for (i=0; i<n3; i++){
//        dual_2[i] = con2[i].get(GRB_DoubleAttr_Pi);
//    }
//    for (i=0; i<n4; i++){
//        dual_3[i] = con3[i].get(GRB_DoubleAttr_Pi);
//    }
////    for (i=0; i<n2; i++)
////    {
////        for (a=0; a<l; a++)
////            dual_2[i*l + a] = con2[i*l + a].get(GRB_DoubleAttr_Pi);
////        for (a=0; a<e; a++)
////            dual_3[i*e + a] = con3[i*e + a].get(GRB_DoubleAttr_Pi);
////    }
//
//    for (i=0; i<fnl; i++){
//        dual_4[i] = con4[i].get(GRB_DoubleAttr_Pi);
//    }
//    for (i=0; i<fne; i++){
//        dual_5[i] = con6[i].get(GRB_DoubleAttr_Pi);
//    }
////    for (i=0; i<flow*n2; i++)
////    {
////        for (a=0; a<l; a++)
////            dual_4[i*l + a] = con4[i*l + a].get(GRB_DoubleAttr_Pi);
////        for (a=0; a<e; a++)
////            dual_5[i*e + a] = con6[i*e + a].get(GRB_DoubleAttr_Pi);
////    }
//    for (i=0; i<n5; i++){
//        dual_6[i] = con8[i].get(GRB_DoubleAttr_Pi);
//    }
//    for (i=0; i<fnle; i++){
//        dual_7[i] = con9[i].get(GRB_DoubleAttr_Pi);
//    }
//
////    for (i=0; i<n5; i++)
////        dual_6[i] = con8[i].get(GRB_DoubleAttr_Pi);
////
////    for (i=0; i<fnle; i++)
////        dual_7[i] = con9[i].get(GRB_DoubleAttr_Pi);
//
//    //transform the dual solution to the parameters of subproblem
//
//    for (i=0; i<flow; i++) {
//        int innl = i * n3;
//        int inne = i * n4;
//        int innle = i * n5;
//        for (u = 0; u < n; u++){
//            int unl = u * n * l;
//            int une = u * n * e;
//            int unle = u * n * l * e;
//            for (v = 0; v < n; v++) {
//                int vl = v * l;
//                int ve = v * e;
//                int vle = v * l * e;
//                for (a = 0; a < l; a++)
//                    param_1[i][u][v][a] = (- M_bitrate[i] * con2[unl + vl + a].get(GRB_DoubleAttr_Pi) - con4[innl + unl + vl + a].get(GRB_DoubleAttr_Pi));
//
//                for (a = 0; a < e; a++)
//                    param_2[i][u][v][a] = (- M_bitrate[i] * con3[une + ve + a].get(GRB_DoubleAttr_Pi) - con6[inne + une + ve + a].get(GRB_DoubleAttr_Pi));
//
//                for (a = 0; a < l; a++){
//                    int ae = a * e;
//                    for (z = 0; z < e; z++)
//                        param_3[i][u][v][a][z] = (- con9[innle + unle + vle + ae + z].get(GRB_DoubleAttr_Pi) + con8[unle + vle + ae + z].get(GRB_DoubleAttr_Pi));
//                }
//
//            }
//        }
//        param_4[i] = con1[i].get(GRB_DoubleAttr_Pi);
//    }
//}
