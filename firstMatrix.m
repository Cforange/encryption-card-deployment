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
        ECf=b3;%���������ܿ����߿����������b3�Ǵ���ÿ������Ĵ����,���м���b1<b2<b3
    end
end

if bitrate<=a1
    LCf=a1;
else
    if bitrate<=a2
        LCf=a2;
    else
        LCf=a3;%���������ܿ����߿�����������Ǵ���ÿ������Ĵ����
    end
end

for s=1:node
   for d=1:node
        dangercost=0;
        [shortestPath, totalCost] = dijkstra(netCostMatrix, s, d);%���·
        %�������·���µ�CAG
        for i = 1:(length(shortestPath)-1)
            danbian = phydangernetcostmatrix(shortestPath(1,i),shortestPath(1,i+1));
            dangercost = dangercost + danbian;%�ж�Դ�ڵ㵽Ŀ�Ľڵ�����·���Ƿ�ȫ
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