//
// Created by Fay Chan on 2021/1/29.
//
#include <vector>
#include "def.h"
using namespace std;

double dijkstra(vector<vector<double> > G, int s, int d, vector<int> &shortestPath){//正确的
    vector<bool> vis(n);
    vector<double> dis(n);
    vector<int> pre(n);
    int i,j;
    //初始化
    fill(vis.begin(),vis.end(),false);
    fill(dis.begin(),dis.end(),INF);
    dis[s] = 0;
    for (i=0; i<n; i++)
        pre[i] = -1;

    //n次循环确定dis[n]数组
    for (i=0; i<n; i++)
    {
        int u = -1;
        double MIN = INF;
        //找到距离s距离最近的u和其最短距离dis[u]
        for (j=0; j<n; j++)
        {
            if(!vis[j] && dis[j] < MIN)
            {
                u = j;
                MIN = dis[j];
            }
        }
        if (u == -1)
            return INF;//剩余节点与s均不连通
        vis[u] = true;
        for(j=0; j<n; j++)
        {
            //遍历所有节点，如果j未被访问&&可以到达j&&以u为中节点使得dis[j]更小
            if(!vis[j] && G[u][j]!=INF && dis[u]+G[u][j]<dis[j])
            {
                dis[j] = dis[u] + G[u][j];
                pre[j] = u;
            }
        }
    }
    if (pre[d] != -1)
    {
        int t = d;
        shortestPath.push_back(d);
        int p;
        while (t != s)
        {
            p = pre[t];
            shortestPath.insert(shortestPath.begin(),p);
            t = p;
        }
    }
    return dis[d];
}

void kshortestPaths(vector<vector<double> > G, int s, int d, vector<int> &shortestPath,
                    vector<vector<int> > &shortestPaths, double totalcost[2], int k_paths){
    int i=0;int j=0;int path_number=0;int size_X=0;int current_P = 0;
    int k = 1;

    vector<vector<int> > Pall;//Pall holds all the paths found so far
    vector<double> Pall_cost;
    vector<vector<int> > X;//X is a subset of Pall(used by Yen's algorithm below)
    vector<double> X_cost;
    vector<int> X_num;
    vector<int> S;

    double cost = dijkstra(G, s, d, shortestPath);
    if (shortestPath.empty())//判断向量是否为空
    {
        for (i=0; i<k_paths; i++)
            totalcost[i] = INF;
        return ;
    }
    else{
        path_number = 0;//注意，以前是1
        Pall.push_back(shortestPath);
        Pall_cost.push_back(cost);
        current_P = path_number;//注意，以前是1
        size_X = 1;
        X.push_back(shortestPath);
        X_cost.push_back(cost);
        X_num.push_back(path_number);
        S.push_back(shortestPath[0]);//偏移顶点是初始的第一个节点

        shortestPaths.push_back(shortestPath);
        totalcost[0] = cost;
        while (k<k_paths && size_X!=0)
        {
            //remove P from X
            for(i=0; i<X.size(); i++)
            {
                if (X_num[i] == current_P)
                {
                    size_X -= 1;
                    X.erase(X.begin()+i);
                    X_cost.erase(X_cost.begin()+i);
                    X_num.erase(X_num.begin()+i);
                }
            }

            vector<int> P_ = Pall[current_P];//P_ is current P,just to make it easier for the notations
            //find w in (P_,w) in set S,w was the dev vertex used to find P_
            int w = S[current_P];
            int w_index_in_path;
            for (i=0; i<P_.size(); i++)
            {
                if (w == P_[i])
                    w_index_in_path = i;
            }

            vector<vector<double> > tempG;
            //index_dev_vertex is index in P_ of deviation vertex
            for (int index_dev_vertex=w_index_in_path; index_dev_vertex<P_.size()-1; index_dev_vertex++)
            {
                tempG = G;
                //Remove vertices in P before index_dev_vertex and there incident edges
                for (i=0; i<index_dev_vertex-1; i++)
                {
                    int v = P_[i];
                    for (j=0; j<n; j++)
                    {
                        tempG[v][j] = INF;
                        tempG[j][v] = INF;
                    }
                }
                //remove incident edge of v if v is in shortestPaths (K) U P_ with similar sub_path to P_
                vector<vector<int> > SP_sameSubPath;
                int index = 0;
                SP_sameSubPath.push_back(P_);
                for (i=0; i<shortestPaths.size(); i++)
                {
                    if (shortestPaths[i].size() >= index_dev_vertex)
                    {
                        for (j=0; j<index_dev_vertex; j++)
                        {
                            if (P_[j] != shortestPaths[i][j])
                                break;
                        }
                        if (j == index_dev_vertex)
                        {
                            index += 1;
                            SP_sameSubPath.push_back(shortestPaths[i]);
                        }
                    }
                }
                int v_ = P_[index_dev_vertex];
                int nex;
                for (j=0; j<SP_sameSubPath.size(); j++)
                {
                    nex = SP_sameSubPath[j][index_dev_vertex+1];
                    tempG[v_][nex] = INF;
                }

                //get the cost of the sub path before deviation vertex v
                vector<int> sub_P;
                int cost_sub_P;
                sub_P.assign(P_.begin(),P_.begin()+index_dev_vertex);
                if (index_dev_vertex != 0)
                    cost_sub_P = G[P_[index_dev_vertex-1]][P_[index_dev_vertex]];
                else{
                    cost_sub_P = 0;
                }
                if (!sub_P.empty()){
                    for (i=0; i<sub_P.size()-1; i++)
                    {
                        cost_sub_P += G[sub_P[i]][sub_P[i+1]];
                    }
                }
                //call dijkstra between deviation vertex to destination node
                vector<int> dev_p;
                dev_p.clear();//清空所有元素
                double c = dijkstra(tempG, s, d, dev_p);
                if (!dev_p.empty())
                {
                    path_number += 1;
                    Pall_cost.push_back(c + cost_sub_P);
                    dev_p.insert(dev_p.begin(),sub_P.begin(),sub_P.end());
                    Pall.push_back(dev_p);
                    S.push_back(P_[index_dev_vertex]);
                    size_X += 1;
                    X.push_back(dev_p);
                    X_num.push_back(path_number);
                    X_cost.push_back(c + cost_sub_P);
                }
            }
            //step necessary otherwise if k is bigger than number of possible paths
            //the last results will get repeated
            if (size_X > 0)
            {
                int shortestXcost = X_cost[0];// cost of path
                int shortestX = X_num[0];// ref number of path
                for (i=1; i<size_X; i++)
                {
                    if (X_cost[i] < shortestXcost)
                    {
                        shortestX = X_num[i];
                        shortestXcost = X_cost[i];
                    }
                }
                current_P = shortestX;
                k += 1;
                shortestPaths.push_back(Pall[current_P]);
                totalcost[k-1] = Pall_cost[current_P];
            }
        }
        return ;
    }
}

void dangerPath(int d[2][n][n], int h[2][n][n], vector<vector<double> > G, int row, int dl[][2],
                vector<int> &shortestPath, vector<vector<int> > &shortestPaths){
    //创建d时初始化：memset(d, 0, sizeof(d));
    int i=0;int j=0;int k=0;int u=0;int v=0;
    double totalcost[2];
    for (u=0; u<n; u++)
        for (v=0; v<n; v++)
        {
            kshortestPaths(G, u, v, shortestPath, shortestPaths, totalcost, 2);
            if (!shortestPaths.empty())
            {
                for (k=0; k<2; k++)
                {
                    vector<int> a = shortestPaths[k];
                    for (i=0; i<row; i++)
                        for (j=0; j<a.size()-1; j++)
                            if (a[j] == dl[i][0] && a[j+1] == dl[i][1])
                                d[k][u][v] = 1;

                    h[k][u][v] = totalcost[k];
                }
            }
            shortestPath.clear();
            shortestPaths.clear();
        }
}
