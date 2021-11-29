//
// Created by Fay Chan on 2021/1/30.
//
#include <def.h>

void createCol(int M_bitrate[flow], int x_lc[flow][n2][l], int y_ec[flow][n2][e],
               int t_lcec[flow][n2][LE], GRBModel& m, GRBColumn column[flow], double col[len]){
    int i, j, k, u, v, a, z;
    int un, unv, ae;
//    double col[len];
    for (i=0; i<flow; i++){
        memset(col, 0, len*sizeof(double)/sizeof(char));
        k = 0;
        //con1：flow
        for (j=0; j<flow; j++){
            if (j == i){
                col[k] = 1;
            }
            k++;
        }
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
        //con5:n lt
        k += nlt;
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
        //con7:n et
        k += net;
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
        column[i].clear();
        column[i].addTerms(col, m.getConstrs(), len);
    }
//    free(col);
//    col = NULL;
}