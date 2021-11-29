#
# multilayer optical network planning with encrytion card deployment
# ILP modeling problem
#
# This finds the optimal solution for minimizing request routing and 
# resource distribution
#
param x_file, symbolic := "x.txt";

/* parameters */
/* 1.set of network nodes */
set N;

/* 2.set of all traffic flows */
set F;

/* 2.1.set of the highest important flows */
set F1 within F;

/* 2.2.set of the middle important flows */
set F2 within F;

/* 3.set of all LCs used when flowing into the node */
set L;

/* 4.set of all LCs with different types */
set Lt;

/* 5.set of all ECs used when flowing into the node */
set E;

/* 6.set of all ECs with different types */
set Et;

/* 6'.set of all shortest paths from source to destination  */
set K;
 
/* 7.set of k-th shortest light path */
param link{u in N,v in N};

/* 8.set of light path with dangerous links */
param d{k in K,u in N,v in N};

/* 9.set of hop count of the k-th shortest light path (u,v) */
param h{k in K,u in N,v in N};

/* 10.set of bitrate of LC */
param BL{l in L};

/* 11.set of bitrate of EC */
param BE{e in E};

/* 12.set of bitrate of Lt */
param BLt{lt in Lt};

/* 13.set of bitrate of Et */
param BEt{et in Et};

/* 14.set of cost of Lt */
param CLt{l in L};

/* 15.set of cost of Et */
param CEt{e in E};

/* 16.set of bitrate of the i-th flow */
param Bit{i in F};

/* 17.set of source node of the i-th flow */
param Source{i in F};

/* 18.set of destination node of the i-th flow */
param Des{i in F};

/* 19.set of upper limit of LCs at each node */
param UL{lt in Lt};

/* 20.set of upper limit of ECs at each node */
param UE{et in Et};


# variables

/* 1.equals to 1,if the i-th traffic flows through the link (u,v);equals to 0 otherwise */
var r{i in F,u in N,v in N},binary;

/* 2.equals to 1,if the i-th traffic flowing through the link (u,v) chooses the k-th shortest path */
var rr{k in K,i in F,u in N,v in N},binary;

/* 3.equals to 1,if the i-th traffic flowing through the link (u,v) use the l-th LC;equals to 0 otherwise */
var x{i in F,u in N,v in N,l in L},binary;

/* 4.equals to 1,if the i-th traffic flowing through the link (u,v) use the e-th EC;equals to 0 otherwise */
var y{i in F,u in N,v in N,e in E},binary;

/* 5.integer variable denoting the number of LCs with rate lt assigned to the v node */
/*var nl{v in N,lt in Lt}, integer , >=0;*/

/* 6.integer variable denoting the number of ECs with rate et assigned to the v node */
/*var ne{v in N,et in Et}, integer , >=0;*/

/* 7.equals to 1,if the i-th flow and j-th flow flowing through the link (u,v) use the l-th LC;equals to 0 otherwise */
/*var w{i in F,j in F,u in N,v in N,l in L:j <> i},binary;*/

/* 8.equals to 1,if the i-th flow and j-th flow flowing through the link (u,v) use the e-th EC;equals to 0 otherwise */ 
/*var z{i in F,j in F,u in N,v in N,e in E:j <> i},binary;*/

/* 9.equals to 1,if the link (u,v) use the l-th LC,equals to 0 otherwise */
var f{u in N,v in N,l in L},binary;

/* 10.equals to 1,if the link (u,v) use the e-th EC,equals to 0 otherwise */
var g{u in N,v in N,e in E},binary;

/* 11.equals to 1,if the i-th flow flowing through the link (u,v) uses the l-th LC and the e-th EC;equals to 0 otherwise */
var t{i in F,u in N,v in N,l in L,e in E},binary;

/* 12.equals to 1,if the flows flowing through the link (u,v) use the l-th LC and e-th EC;equals to 0 otherwise */
var Q{u in N,v in N,l in L,e in E},binary;


#objective

minimize cost :  sum{u in N,v in N,l in L}(2 * CLt[l] * f[u,v,l]) + sum{u in N,v in N,e in E}(2 * CEt[e] * g[u,v,e]) + sum{u in N,v in N,i in F,k in K}(h[k,u,v] * rr[k,i,u,v] * Bit[i]/100);


#constraints

s.t.se1{i in F, u in N,v in N}:r[i,u,v] <= link[u,v];

s.t.se2{i in F,u in N:(u == Source[i])}: sum{v in N}(r[i,u,v] - r[i,v,u]) = 1;

s.t.se3{i in F,u in N:(u == Des[i])}: sum{v in N}(r[i,u,v] - r[i,v,u]) = -1;

s.t.se4{i in F,u in N:((u <> Source[i]) and (u <> Des[i]))}: sum{v in N}(r[i,u,v] - r[i,v,u]) = 0;

s.t.se5{i in F,u in N,v in N}:sum{k in K}rr[k,i,u,v] = r[i,u,v];

s.t.se6{i in F,u in N,v in N}:sum{l in L}x[i,u,v,l] = r[i,u,v];

s.t.se7{u in N,v in N,l in L}:sum{i in F}(x[i,u,v,l] * Bit[i]) <= BL[l];

s.t.se8{i in F1,u in N,v in N}:sum{e in E}y[i,u,v,e] = sum{k in K}(rr[k,i,u,v] * d[k,u,v]);

s.t.se88{i in F2,u in N,v in N}:sum{e in E}y[i,u,v,e] = 0;

s.t.se9{u in N,v in N,e in E}:sum{i in F}(y[i,u,v,e] * Bit[i]) <= BE[e];

s.t.se10{v in N,lt in Lt}:sum{u in N,l in L:BL[l] = BLt[lt]}f[u,v,l] <= UL[lt];

s.t.se11{v in N,et in Et}:sum{u in N,e in E:BE[e] = BEt[et]}g[u,v,e] <= UE[et];

s.t.se12{u in N,v in N,l in L}:f[u,v,l] <= sum{i in F}x[i,u,v,l];

s.t.se13{i in F,u in N,v in N,l in L}:f[u,v,l] >= x[i,u,v,l];

s.t.se14{u in N,v in N,e in E}:g[u,v,e] <= sum{i in F}(y[i,u,v,e]);

s.t.se15{i in F,u in N,v in N,e in E}:g[u,v,e] >= y[i,u,v,e];

s.t.se16{i in F,u in N,v in N,l in L,e in E}:(x[i,u,v,l] + y[i,u,v,e] - 1) <= t[i,u,v,l,e];

s.t.se17{i in F,u in N,v in N,l in L,e in E}:t[i,u,v,l,e] <= y[i,u,v,e];

s.t.se18{u in N,v in N,l in L,e in E}:Q[u,v,l,e] <= sum{i in F}t[i,u,v,l,e];

s.t.se19{i in F,u in N ,v in N,l in L,e in E}:Q[u,v,l,e] >= t[i,u,v,l,e] ;

s.t.se20{u in N ,v in N,l in L}:sum{e in E}(Q[u,v,l,e] * BE[e]) <= BL[l];

s.t.se21{u in N ,v in N,e in E}:sum{l in L}Q[u,v,l,e] <= 1;

solve;

printf "%f\n",cost >> x_file;

end;