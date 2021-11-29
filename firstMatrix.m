function [lightpathCostMatrix,lightpathhopsMatrix,dangerouslightpathMatrix,ECf,LCf] = firstMatrix(node,netCostMatrix,phydangernetcostmatrix,bitrate,b1,b2,b3,a1,a2,a3)
%node = 6

lightpathCostMatrix=cell( node, node);
lightpathhopsMatrix=cell( node, node);
dangerouslightpathMatrix=cell( node,node);

shortestPath = [];

if bitrate<=b1
    ECf=b1;
else
    if bitrate<=b2
        ECf=b2;
    else
        ECf=b3;%这里假设加密卡或线卡的最大速率b3是大于每个请求的带宽的,其中假设b1<b2<b3
    end
end

if bitrate<=a1
    LCf=a1;
else
    if bitrate<=a2
        LCf=a2;
    else
        LCf=a3;%这里假设加密卡或线卡的最大速率是大于每个请求的带宽的
    end
end

for s=1:node
   for d=1:node
        dangercost=0;
        [shortestPath, totalCost] = dijkstra(netCostMatrix, s, d);%最短路
        %计算最短路径下的CAG
        for i = 1:(length(shortestPath)-1)
            danbian = phydangernetcostmatrix(shortestPath(1,i),shortestPath(1,i+1));
            dangercost = dangercost + danbian;%判断源节点到目的节点的最短路径是否安全
        end
        if dangercost == 0
            opticalpathcost = 0;
            dangerouslightpathMatrix{s,d} = 0;
        else
             opticalpathcost = 0;
             dangerouslightpathMatrix{s,d} = 1;
        end
        if s == d
            lightpathCostMatrix{s,d} = inf;
            lightpathhopsMatrix{s,d} = 0;
        else
            lightpathCostMatrix{s,d} = opticalpathcost;
            lightpathhopsMatrix{s,d} = length(shortestPath)-1;
        end
   end
end