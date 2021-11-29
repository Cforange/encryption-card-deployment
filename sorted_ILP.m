tic;
%LC和EC cost只计算了单向光路的，而我的ILP模型是双向建立LC和EC的。
filename = 'M.mat';
requests = load(filename);

M_s = requests.M.s;
M_s1 = requests.M.s1;
M_d = requests.M.d;
M_d1 = requests.M.d1;
M_bitrate = requests.M.bitrate;

node = 6;
a1=40;
a2=100;
a3=400;%输入的线卡速率
b1=40;
b2=100;
b3=400;%输入的加密卡的速率
A1=1;
A2=2;
A3=4;%和线卡速率相对应的成本
B1=4;
B2=8;
B3=16;%和加密卡速率相对应的成本
P1=cell(node, node);

cost=0;%总资源
cost0=0;%带宽资源
costA=0;%线卡的成本
costA400=0;
costA100=0;
costA40=0;
costB=0;%加密卡的成本
costB400=0;
costB100=0;
costB40=0;
costl=0;
coste=0;

numEC40=2;
numEC100=2;
numEC400=2;
numEC = numEC40 + numEC100 + numEC400;
EC_inf = [40,40,100,100,400,400];
 for i=1: node
     EC_use{1,i} = [1,1,1,1,1,1];
 end

numLC40=2; 
numLC100=2;
numLC400=2;
numLC = numLC40 + numLC100 + numLC400;
LC_inf = [40,40,100,100,400,400];% 40,40,40,40,40,100,100,100,400,400,400,400
 for i=1: node
     LC_use{1,i} = [1,1,1,1,1,1];%1,1,1,1,1,1,1,1,1,1,1,1
 end



for i = 1:node
    
        numE40(i)=numEC40;
        numE100(i)=numEC100;
        numE400(i)=numEC400;
        numL40(i)=numLC40;
        numL100(i)=numLC100;
        numL400(i)=numLC400;
    
end

F=0;G=0;

%初始化x(u,v,i,j,n),y(u,v,i,j,n),h(i),bl(u,v,i,j,n),be(u,v,i,j,n),t(u,v,i,j,n,j1,n1)
xlc(length(M_s),node,node,numLC) = 0;
yec(length(M_s),node,node,numEC) = 0;
%lcbit(node,node,length(M_s),numLC) = 0;
%ecbit(node,node,length(M_s),numEC) = 0;
hops(length(M_s)) = 0;
tlc_ec(length(M_s),node,node,numLC,numEC) = 0;


for i=1: node
    for j=1: node
        P1{i,j}=0;%存储最短路径上的线卡加密卡的信息    返回0的6X6矩阵
    end
end

for q = 1:length(M_s)
    s=M_s(q); 
    d=M_d(q);
    bitrate=M_bitrate(q);
    P=P1;
    [P] =Psorting(P,node); 
    
netCostMatrix=[0 1 inf 1 inf inf;
               1 0 1 1 inf inf;
               inf 1 0 inf 1 1;
               1 1 inf 0 1 inf;
               inf inf 1 1 0 1;
               inf inf 1 inf 1 0];
           
phydangernetcostmatrix = [0,0,0,1,0,0;
                          0,0,0,0,0,0;
                          0,0,0,0,0,1;
                          1,0,0,0,0,0;
                          0,0,0,0,0,0;
                          0,0,1,0,0,0];
           
[lightpathCostMatrix, lightpathhopsMatrix,dangerouslightpathMatrix,ECf, LCf] = firstMatrix( node,netCostMatrix,phydangernetcostmatrix,bitrate,b1,b2,b3,a1,a2,a3);
 H = lightpathhopsMatrix;
 D = dangerouslightpathMatrix;
 LM = lightpathCostMatrix;
 for i = 1:node
     for j = 1:node
         x = 1;
         while ((P1{i,j}(x,1)<bitrate)||(P1{i,j}(x,2)<bitrate))&&(x<size(P1{i,j},1)) %返回行数
             x = x + 1;
         end
         if (P1{i,j}(x,1)<bitrate)||(P1{i,j}(x,2)<bitrate)
             x=x+1;
         else
         end
         if x <= size(P1{i,j},1)  %现有拓扑中的线卡和加密卡可以继续使用
             LM{i,j}(1,1) = H{i,j}(1,1) * bitrate/100;%对于（i，j)光路权重为H(i,j)
         else%说明对于该请求在（i，j)上面没有可以同时使用的加密卡和线卡
             if D{i,j}(1,1) > 0 %链路危险，需要新建加密卡
                 minimumcard = ECf;
                 num40 = min(numE40(j));
                 num100 = min(numE100(j));
                 num400 = min(numE400(j));
                 [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                 ECbr = availablecard;
                 [costEC] = costencryptioncard(ECbr);%输出当下速率加密卡的成本
                 if ECbr == inf %说明加密卡资源不够
                     LM{i,j}(1,1)=inf;%这条路加密卡资源不够不可走，需要绕路
                 else%说明加密卡资源够
                     y = 1;
                     while ((P1{i,j}(y,1))<ECbr) && (y<size(P1{i,j},1))
                         y = y + 1;
                     end
                     if P1{i,j}(y,1) < ECbr
                         y=y+1;
                     else
                     end
                     if y <= size(P1{i,j},1)%说明现有拓扑中有可供新的加密卡使用的线卡
                         LM{i,j}(1,1)=H{i,j}(1,1)* bitrate/100 + costEC;%新建加密卡和带宽成本
                     else%说明现有拓扑中没有可用的线卡，需要新建线卡
                         LCbr=ECbr;%新建线卡的最小容量要至少等于新建的加密卡的容量
                         minimumcard=LCbr;
                         num40 = min(numL40(j));
                         num100 = min(numL100(j));
                         num400 = min(numL400(j));
                         [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                         LCbr=availablecard;
                         [costLC] = costlinecard(LCbr);
                         if LCbr==inf%线卡资源不够
                             LM{i,j}(1,1)=inf;
                         else%线卡资源够
                             LM{i,j}(1,1)=H{i,j}(1,1)* bitrate/100 + costEC + costLC;
                         end
                     end
                 end
             else%说明链路不危险不需要新建加密卡
                 y=1;
                 while ((P1{i,j}(y,1))<bitrate)&&(y<size(P1{i,j},1))
                     y=y+1;
                 end
                 if P1{i,j}(y,1)<bitrate
                     y=y+1;
                 else
                 end
                 if y<=size(P1{i,j},1)%说明现有拓扑中使用的线卡
                     LM{i,j}(1,1)=H{i,j}(1,1)* bitrate/100;
                 else %说明需要新建线卡
                     LCbr=LCf;%最初请求适用的线卡容量
                     minimumcard=LCbr;
                     num40 = min(numL40(j));
                     num100 = min(numL100(j));
                     num400 = min(numL400(j));
                     [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                     LCbr = availablecard;
                     [costLC] = costlinecard(LCbr);
                     if LCbr==inf%线卡资源不够
                         LM{i,j}(1,1)=inf;
                     else%线卡资源够
                         LM{i,j}(1,1)=H{i,j}(1,1)* bitrate/100+ costLC;
                     end
                 end
             end
         end
     end
 end
         
     netCostMatrix=cell2mat(LM);
     [shortestPath,totalCost] = dijkstra(netCostMatrix, s, d);%更新辅助图，找到最短路径
     if q <= length(M_s1)%对于c重要的流
         k = 1;
         while ((P1{s,d}(k,1)<bitrate)|| (P1{s,d}(k,2)<bitrate))&&(k<size(P1{s,d},1))
             k=k+1;
         end
         if (P1{s,d}(k,1)<bitrate)|| (P1{s,d}(k,2)<bitrate)
             k=k+1;
         end
         if k<=size(P1{s,d},1)%有加密卡和线卡同时可以使用
             g= P1{s,d}(k,5);
             for w=1:size(P1{s,d},1)
                 if P1{s,d}(w,5) == g
                     P1{s,d}(w,1) = P1{s,d}(w,1)-bitrate;%所用线卡的速率相应减去请求带宽 因为每个线卡可能对应多个加密卡
                 end
             end
             P1{s,d}(k,2)= P1{s,d}(k,2)-bitrate;%所用加密卡的速率相应减去请求带宽
             
             xlc(q,s,d,g) = 1;
             yec(q,s,d,P1{s,d}(k,6)) = 1;
             %lcbit(s,d,q,g) = bitrate;
             %ecbit(s,d,q,P1{s,d}(k,6)) = bitrate;
             hops(q) = H{s,d}(1,1) * bitrate;
             tlc_ec(q,s,d,g,P1{s,d}(k,6)) = 1;
             
             cost0=cost0+(H{s,d}(1,1)*bitrate);%更新带宽成本
             cost=cost+(H{s,d}(1,1)*bitrate/100);%总cost 更新
         else
             %%%%%%%没有加密卡和线卡同时被使用，需要新建加密卡
             ECbr=ECf;
             minimumcard=ECbr;
             num40 = min(numE40(d));
             num100 = min(numE100(d));
             num400 = min(numE400(d));
             [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
             ECbr=availablecard;
             [costEC] = costencryptioncard(ECbr);
             [numE40(d),numE100(d),numE400(d)] = ECnumupdate(numE40(d),numE100(d),numE400(d),ECbr);%更新加密卡的数量
             %%%%%%%%%%%记录所需容量的加密卡对应的成本
             cost1 = costEC;
             %%%%%%%%%%%%%%
             m=1;
             while( P1{s,d}(m,1)< ECbr)&&(m<size(P1{s,d},1))
                 m=m+1;
             end
             if P1{s,d}(m,1)< ECbr
                 m=m+1;
             end
             if m<=size(P1{s,d},1)%说明有线卡可以被新建的加密卡所使用
                 g1= P1{s,d}(m,5);
                 for w=1:size(P1{s,d},1)
                     if P1{s,d}(w,5)==g1
                         P1{s,d}(w,1)= P1{s,d}(w,1)-bitrate;%所用线卡的速率相应减去请求带宽
                     end
                 end
                 f=size(P1{s,d},1);
                 costB=costB+cost1;%更新加密卡的成本
                 cost0=cost0+(H{s,d}(1,1)*bitrate);%更新带宽成本
                 cost=cost+cost1+(H{s,d}(1,1)*bitrate/100);%更新总成本
                 %%%%%%%%%更新线卡和加密卡的信息
                 P1{s,d}(f+1,1)= P1{s,d}(m,1);%更新线卡剩余带宽
                 P1{s,d}(f+1,2)= ECbr-bitrate;%更新加密卡剩余带宽
                 P1{s,d}(f+1,3)=P1{s,d}(m,3);%记录所用线卡总容量
                 P1{s,d}(f+1,4)= ECbr;%记录加密卡总容量
                 P1{s,d}(f+1,5)=P1{s,d}(m,5);%记录使用的是几号线卡
                 [F] = ECchoose(ECbr,EC_inf,EC_use{1,d},numEC);
                 EC_use{1,d}(F) = 0;
                 P1{s,d}(f+1,6) = F;%记录使用的是几号加密卡
                 
                 xlc(s,d,q,g1) = 1;
                 yec(s,d,q,F) = 1;
                 hops(q) = H{s,d}(1,1) * bitrate;
                 tlc_ec(s,d,q,g1,F) = 1;
                 
             else%现有网络拓扑中的线卡容量不够新加的加密卡所使用，即需要再新建线卡
                 LCbr=ECbr;%新建线卡的最小容量要至少等于新建的加密卡的容量
                 minimumcard=LCbr;
                 num40 = min(numL40(d));
                 num100 = min(numL100(d));
                 num400 = min(numL400(d));
                 [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                 LCbr=availablecard;
                 [costLC] = costlinecard(LCbr);
                 [numL40(d),numL100(d),numL400(d)] = LCnumupdate(numL40(d),numL100(d),numL400(d),LCbr);%更新线卡的数量
                 %%%%%%%%%%%记录线卡对应容量的成本
                 cost2=costLC;
                 %%%%%%%%%%%%%%%%%%
                 f=size(P1{s,d},1);
                 costB=costB+cost1;%更新加密卡的总成本
                 costA=costA+cost2;%更新线卡的总成本
                 cost0=cost0+(H{s,d}(1,1)*bitrate);%更新带宽的总成本
                 cost=cost+cost1+cost2+(H{s,d}(1,1)*bitrate/100);%更新总成本  
                 P1{s,d}(f+1,1)= LCbr-bitrate;
                 P1{s,d}(f+1,2)= ECbr-bitrate;
                 P1{s,d}(f+1,3)=LCbr;
                 P1{s,d}(f+1,4)= ECbr;
                 [G] = LCchoose(LCbr,LC_inf,LC_use{1,d},numLC);
                 LC_use{1,d}(G) = 0;

                 P1{s,d}(f+1,5)=G;
                 [F] = ECchoose(ECbr,EC_inf,EC_use{1,d},numEC);
                 EC_use{1,d}(F) = 0;
                 P1{s,d}(f+1,6)=F;
                 
                 xlc(q,s,d,G) = 1;
                 yec(q,s,d,F) = 1;
                 %lcbit(s,d,q,G) = bitrate;
                 %ecbit(s,d,q,F) = bitrate;
                 hops(q) = H{s,d}(1,1) * bitrate;
                 tlc_ec(q,s,d,G,F) = 1;
                 
             end
         end
     else
         for j=1:(length(shortestPath)-1)
             if D{shortestPath(j),shortestPath(j+1)}(1,1)>0
                  k=1;
                  while ((P1{shortestPath(j),shortestPath(j+1)}(k,1)<bitrate)|| (P1{shortestPath(j),shortestPath(j+1)}(k,2)<bitrate))&&(k<size(P1{shortestPath(j),shortestPath(j+1)},1))
                      k=k+1;
                  end
                  if (P1{shortestPath(j),shortestPath(j+1)}(k,1)<bitrate)|| (P1{shortestPath(j),shortestPath(j+1)}(k,2)<bitrate)
                      k=k+1;
                  end
                  if k<=size(P1{shortestPath(j),shortestPath(j+1)},1)%有加密卡和线卡同时可以使用
                      g= P1{shortestPath(j),shortestPath(j+1)}(k,5);
                      for w=1:size(P1{shortestPath(j),shortestPath(j+1)},1)
                          if P1{shortestPath(j),shortestPath(j+1)}(w,5) == g
                              P1{shortestPath(j),shortestPath(j+1)}(w,1) = P1{shortestPath(j),shortestPath(j+1)}(w,1)-bitrate;%所用线卡的速率相应减去请求带宽 因为每个线卡可能对应多个加密卡
                          end
                      end
                      P1{shortestPath(j),shortestPath(j+1)}(k,2)= P1{shortestPath(j),shortestPath(j+1)}(k,2)-bitrate;%所用加密卡的速率相应减去请求带宽
                      cost0=cost0+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate);%更新带宽成本
                      cost=cost+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate/100);%总cost 更新
                      
                      u = shortestPath(j);
                      v = shortestPath(j+1);
                      
                      xlc(q,u,v,g) = 1;
                      yec(q,u,v,P1{u,v}(k,6)) = 1;
                      %lcbit(u,v,q,g) = bitrate;
                      %ecbit(u,v,q,P1{u,v}(k,6)) = bitrate;
                      hops(q) = hops(q) + H{u,v}(1,1) * bitrate;
                      tlc_ec(q,u,v,g,P1{u,v}(k,6)) = 1;
                      
                  else
                      %%%%%%%没有加密卡和线卡同时被使用，需要新建加密卡
                      ECbr=ECf;
                      minimumcard=ECbr;
                      u = shortestPath(j);
                      v = shortestPath(j+1);
                      num40 = min(numE40(v));
                      num100 = min(numE100(v));
                      num400 = min(numE400(v));
                      [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                      ECbr=availablecard;
                      [costEC] = costencryptioncard(ECbr);
                      [numE40(v),numE100(v),numE400(v)] = ECnumupdate(numE40(v),numE100(v),numE400(v),ECbr);%更新加密卡的数量

                      %%%%%%%%%%%记录所需容量的加密卡对应的成本
                      cost1=costEC;
                      %%%%%%%%%%%%%%
                      m=1;
                      while( P1{shortestPath(j),shortestPath(j+1)}(m,1)< ECbr)&&(m<size(P1{shortestPath(j),shortestPath(j+1)},1))
                          m=m+1;
                      end
                      if  P1{shortestPath(j),shortestPath(j+1)}(m,1)< ECbr
                           m=m+1;
                      end
                      if m<=size(P1{shortestPath(j),shortestPath(j+1)},1)%说明有线卡可以被新建的加密卡所使用
                          g1= P1{shortestPath(j),shortestPath(j+1)}(m,5);
                          for w=1:size(P1{shortestPath(j),shortestPath(j+1)},1)
                              if P1{shortestPath(j),shortestPath(j+1)}(w,5)==g1
                                  P1{shortestPath(j),shortestPath(j+1)}(w,1)= P1{shortestPath(j),shortestPath(j+1)}(w,1)-bitrate;%所用线卡的速率相应减去请求带宽
                              end
                          end
                          f=size(P1{shortestPath(j),shortestPath(j+1)},1);
                          costB=costB+cost1;%更新加密卡的成本
                          cost0=cost0+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate);%更新带宽成本
                          cost=cost+cost1+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate/100);%更新总成本
                          %%%%%%%%%更新线卡和加密卡的信息
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,1)= P1{shortestPath(j),shortestPath(j+1)}(m,1);%更新线卡剩余带宽
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,2)= ECbr-bitrate;%更新加密卡剩余带宽
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,3)=P1{shortestPath(j),shortestPath(j+1)}(m,3);%记录所用线卡总容量
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,4)= ECbr;%记录加密卡总容量
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,5)=P1{shortestPath(j),shortestPath(j+1)}(m,5);%记录使用的是几号线卡
                          [F] = ECchoose(ECbr,EC_inf,EC_use{1,v},numEC);
                          EC_use{1,v}(F) = 0;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,6) = F;
                          
                          xlc(q,u,v,g1) = 1;
                          yec(q,u,v,F) = 1;
                          %lcbit(u,v,q,g1) = bitrate;
                          %ecbit(u,v,q,F) = bitrate;
                          hops(q) = hops(q) + H{u,v}(1,1) * bitrate;
                          tlc_ec(q,u,v,g1,F) = 1;
                          
                      else%现有网络拓扑中的线卡容量不够新加的加密卡所使用，即需要再新建线卡
                          LCbr=ECbr;%新建线卡的最小容量要至少等于新建的加密卡的容量
                          minimumcard=LCbr;
                          num40 = min(numL40(v));
                          num100 = min(numL100(v));
                          num400 = min(numL400(v));
                          [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                          LCbr=availablecard;
                          [costLC] = costlinecard(LCbr);
                          [numL40(v),numL100(v),numL400(v)] = LCnumupdate(numL40(v),numL100(v),numL400(v),LCbr);%更新线卡的数量
                          %%%%%%%%%%%记录线卡对应容量的成本
                          cost2=costLC;
                          %%%%%%%%%%%%%%%%%%
                          f=size(P1{shortestPath(j),shortestPath(j+1)},1);
                          costB=costB+cost1;%更新加密卡的总成本
                          costA=costA+cost2;%更新线卡的总成本
                          cost0=cost0+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate);%更新带宽的总成本
                          cost=cost+cost1+cost2+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate/100);%更新总成本  
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,1)= LCbr-bitrate;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,2)= ECbr-bitrate;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,3)=LCbr;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,4)= ECbr;
                          [G] = LCchoose(LCbr,LC_inf,LC_use{1,v},numLC);
                          LC_use{1,v}(G) = 0;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,5)=G;
                          [F] = ECchoose(ECbr,EC_inf,EC_use{1,v},numEC);
                          EC_use{1,v}(F) = 0;
                          P1{shortestPath(j),shortestPath(j+1)}(f+1,6)=F;
                          
                          xlc(q,u,v,G) = 1;
                          yec(q,u,v,F) = 1;
                          %lcbit(u,v,q,G) = bitrate;
                          %ecbit(u,v,q,F) = bitrate;
                          hops(q) = hops(q) + H{u,v}(1,1) * bitrate;
                          tlc_ec(q,u,v,G,F) = 1;
                          
                      end
                  end
             else%链路不危险，不需要加密卡
                 f=size(P1{shortestPath(j),shortestPath(j+1)},1);
                 n=1;
                 while(P1{shortestPath(j),shortestPath(j+1)}(n,1)<bitrate)&&(n<size(P1{shortestPath(j),shortestPath(j+1)},1))
                     n=n+1;
                 end
                 if P1{shortestPath(j),shortestPath(j+1)}(n,1)<bitrate
                     n=n+1;
                 else
                 end
                 if n<=size(P1{shortestPath(j),shortestPath(j+1)},1)%说明有线卡可以使用
                     g2= P1{shortestPath(j),shortestPath(j+1)}(n,5);
                     for w=1:size(P1{shortestPath(j),shortestPath(j+1)},1)
                         if P1{shortestPath(j),shortestPath(j+1)}(w,5)==g2
                             P1{shortestPath(j),shortestPath(j+1)}(w,1)= P1{shortestPath(j),shortestPath(j+1)}(w,1)-bitrate;%所用线卡的速率相应减去请求带宽
                         end
                     end
                     cost0=cost0+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate);
                     cost=cost+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate/100);
                     
                     u = shortestPath(j);
                     v = shortestPath(j+1);
                     
                     xlc(q,u,v,g2) = 1;
                     %lcbit(u,v,q,g2) = bitrate;
                     hops(q) = hops(q) + H{u,v}(1,1) * bitrate;
                     
                 else%说明没有线卡可以使用，需要新建线卡
                     LCbr=LCf;
                     minimumcard=LCbr;
                     u = shortestPath(j);
                     v = shortestPath(j+1);
                     num40 = min(numL40(v));
                     num100 = min(numL100(v));
                     num400 = min(numL400(v));
                     [availablecard] = Capacitydecision( minimumcard,num40,num100,num400);%判断在已有的线卡/加密卡资源下可以新建多大的线卡/加密卡
                     LCbr = availablecard;
                     [costLC] = costlinecard(LCbr);
                     [numL40(v),numL100(v),numL400(v)] = LCnumupdate(numL40(v),numL100(v),numL400(v),LCbr);%更新线卡的数量
                     cost2=costLC;
                     u = shortestPath(j);
                     v = shortestPath(j+1);
                     
                     costA=costA+cost2;
                     cost0=cost0+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate);
                     cost=cost+cost2+(H{shortestPath(j),shortestPath(j+1)}(1,1)*bitrate/100);
                     f=size(P1{shortestPath(j),shortestPath(j+1)},1);
                     [G] = LCchoose(LCbr,LC_inf,LC_use{1,v},numLC);
                     LC_use{1,v}(G) = 0;
                     P1{shortestPath(j),shortestPath(j+1)}(f+1,1)= LCbr-bitrate;
                     P1{shortestPath(j),shortestPath(j+1)}(f+1,3)=LCbr;
                     P1{shortestPath(j),shortestPath(j+1)}(f+1,5)=G;
                     P1{shortestPath(j),shortestPath(j+1)}(f+1,6)=0;
                     
                     xlc(q,u,v,G) = 1;
                     %lcbit(u,v,q,G) = bitrate;
                     hops(q) = hops(q) + H{u,v}(1,1) * bitrate;
                     
                 end
             end
         end
     end
end

for q = 1: length(M_s)
    for u = 1:node
        for v = 1:node
            for e = 1:numEC
                param.yec(q,u,v,e) = yec(q,u,v,e);
            end
            
            for l = 1:numLC
                param.xlc(q,u,v,l) = xlc(q,u,v,l);
                %param.lcbit(u,v,q,l) = M_bitrate(q);
            end 
            
            for l = 1:numLC
                for e = 1:numEC
                    param.tlc_ec(q,u,v,l,e) = tlc_ec(q,u,v,l,e);
                end
            end
            
        end
    end
end

param.gec(node,node,numEC)=0;
for q = 1: length(M_s)
    for u = 1:node
        for v = 1:node
            for e = 1:numEC
                if yec(q,u,v,e)~=0
                    param.gec(u,v,e)=1;
                end
            end
        end
    end
end

param.flc(node,node,numLC)=0;
for q = 1: length(M_s)
    for u = 1:node
        for v = 1:node
            for e = 1:numLC
                if xlc(q,u,v,e)~=0
                    param.flc(u,v,e)=1;
                end
            end
        end
    end
end
                
for q = 1: length(M_s)
    param.hops(q) = hops(q);
end

filename=['param','.mat'];
save(filename,'param');
cost0
costA
costB
cost = cost0/100 + 2*costA + 2*costB

toc;