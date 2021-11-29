function [P] = Psorting(P,node)
for i1 = 1: node
    for j1 = 1: node
        p = P{i1,j1};
        j = 0;
        p = sortrows(p,1);%按第一列元素对p的每一行进行排序
        [row,column] = size(p);
        for i = 1: row
            if i == row
                break;
            end
            if p(i+1,1) ~= p(i,1)
                j = 0;
                continue;
            end
            if p(i+1,1) == p(i,1)
                j = j + 1;
                Q = p(i+1-j:i+1,2:column);
                S = sortrows(Q,1);
                p(i+1-j:i+1,2:column) = S;
            end
        end
        P{i1,j1} = p;
    end
end
                
        