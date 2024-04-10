### shell start
```bash
python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph
```

```
throw lgraph::CypherException("OK");
throw lgraph::CypherException(temp);
throw lgraph::CypherException(std::to_string((int)(op_filter->filter_->Type())));
```

### cr1
MATCH p = (acc:Account {id:4615345742880441418})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
RETURN DISTINCT other.id as otherId,
length(p)-1 as accountDistance,
medium.id as mediumId,
medium.type as mediumType
ORDER BY accountDistance, otherId, mediumId;

MATCH p = (acc:Account {id:8725999155939212})-[e1:transfer *1..3]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;


检查什么时候返回
stack path path_unique同步

<!-- check asc, use this -->
MATCH p = (acc:Account {id:8725999155939212})-[e1:transfer *2..2]->(other:Account)
WHERE isDesc(getMemberProp(e1, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

### cr2
MATCH (p:Person {id:58203})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
(other:Account)
WHERE isDesc(getMemberProp(e2, 'timestamp'))=true AND
head(getMemberProp(e2, 'timestamp')) < 1669690342640 AND
last(getMemberProp(e2, 'timestamp')) > 1627020616747
WITH DISTINCT other
MATCH (other)<-[e3:deposit]-(loan:Loan)
WHERE e3.timestamp > 1627020616747 AND
e3.timestamp < 1669690342640
WITH DISTINCT other.id AS otherId, loan.loanAmount AS loanAmount, loan.balance
AS loanBalance
WITH otherId AS otherId, sum(loanAmount) as sumLoanAmount, sum(loanBalance) as
sumLoanBalance
RETURN otherId, round(sumLoanAmount * 1000) / 1000 as sumLoanAmount,
round(sumLoanBalance * 1000) / 1000 as sumLoanBalance
ORDER BY sumLoanAmount DESC, otherId ASC;

MATCH (p:Person {id:19395})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
(other:Account)
WHERE head(getMemberProp(e2, 'timestamp')) < 1669690342640 AND
last(getMemberProp(e2, 'timestamp')) > 1627020616747
RETURN other;

MATCH (p:Person {id:19395})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
(other:Account)
WHERE last(getMemberProp(e2, 'timestamp')) > 1627020616747
RETURN other;

MATCH (p:Person {id:19395})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
(other:Account)
RETURN other;


### cr5
check id:32025

MATCH (person:Person {id:8210})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*3..3]->(dst:Account)
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true AND
head(getMemberProp(e2, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e2, 'timestamp')) < 1669690342640
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

MATCH (person:Person {id:32025})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*1..3]->(dst:Account)
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true AND
head(getMemberProp(e2, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e2, 'timestamp')) < 1669690342640
WITH DISTINCT nodes(p) as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

MATCH (person:Person {id:32025})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*1..3]->(dst:Account)
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true
WITH DISTINCT nodes(p) as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

#### test head
[4743416582405895609,174795960537326727,297237575406462040]

MATCH p=(src:Account {id:4743416582405895609})-[e2:transfer*2..2]->(dst:Account)
WHERE head(getMemberProp(e2, 'timestamp')) > 1627020616747
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

<!-- last, use this -->
MATCH p=(src:Account {id:4743416582405895609})-[e2:transfer*2..2]->(dst:Account {id:4687403336918373745})
WHERE last(getMemberProp(e2, 'timestamp')) > 1659331936801
WITH relationships(p) as path, length(p) as len
ORDER BY len DESC 
RETURN path;


python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph

<!-- two hops -->
[4743416582405895609,4865576722298321633,4687403336918373745]
[4743416582405895609,174795960537326727,258112553643682103]

<!-- where not exec -->

match p=(src:Account {id:4743416582405895609})-[e:transfer*2..2]->(dst:Account {id:4687403336918373745})
WHERE isAsc(getMemberProp(e, 'timestamp'))=true
WITH DISTINCT relationships(p) as path, length(p) as len
ORDER BY len DESC
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e:transfer*1..1]->(dst:Account {id:4865576722298321633})
WITH DISTINCT relationships(p) as path, length(p) as len
ORDER BY len DESC
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e:transfer*2..2]->(dst:Account)
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC
WHERE hasDuplicates(path)=false
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e:transfer*2..2]->(dst:Account {id:4687403336918373745})
WHERE isAsc(getMemberProp(e, 'timestamp'))=true
WITH DISTINCT nodes(p) as path, length(p) as len
ORDER BY len DESC
WHERE hasDuplicates(path)=false
RETURN path;


### cr11

id: 31073 58403

MATCH (p1:Person {id:8155})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
WITH DISTINCT loan
WITH sum(loan.loanAmount) as sumLoanAmount, count(distinct loan) as numLoans
RETURN round(sumLoanAmount * 1000) / 1000 as sumLoanAmount, numLoans;

MATCH p=(p1:Person {id:58403})-[edge:guarantee*1..5]->(pN:Person)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
WITH DISTINCT getMemberProp(nodes(p), 'id') as path
return path;

MATCH p=(p1:Person {id:58403})-[edge:guarantee*1..5]->(pN:Person)
WITH DISTINCT getMemberProp(nodes(p), 'id') as path
return path;

MATCH (p1:Person {id:76694})-[edge:guarantee*1..5]->(pN:Person)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747
RETURN p1;


MATCH p=(p1:Person {id:76694})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WITH DISTINCT getMemberProp(nodes(p), 'id') as path
RETURN path;



1. 调整程序结构，realconsume
2. dfsstate 加上count，用于记录一个点的邻居数量
3. 如何取timestamp，参考GetMemberProp实现

### test
match (n:Account {id:4832081474947659562})<-[:transfer]-(:Account)<-[:transfer]-(n2:Account) return n.id, count(n2);

match (n:Account {id:4832081474947659562})<-[e:transfer *1..2]-(n2:Account) return n.id,count(n2);

match (n:Account)<-[e:transfer *1..2]-(n2:Account) return n.id,count(n2);


### test
mid: 4891754170010318768

match (n:Account {id:4832081474947659562})<-[:transfer]-(mid:Account)<-[:transfer]-(n2:Account {id:130323738850760202}) return n.id, mid.id, n2.id;


match (n:Account {id:4832081474947659562})<-[e:transfer]-(mid:Account {id:4891754170010318768})
return e;


match (n:Account {id:4832081474947659562})<-[e:transfer *2..2]-(n2:Account {id:130323738850760202}) return n.id, n2.id;

match (n:Account {id:130323738850760202})-[e:transfer *2..2]->(n2:Account {id:4832081474947659562}) return n.id, n2.id;


match (n:Account {id:4832081474947659562})<-[e1:transfer]-(mid:Account)<-[e2:transfer]-(n2:Account {id:130323738850760202}) return n.id, e1, mid.id, e2, n2.id;


3.28 总结
从边迭代器取到的边，tid非零。Path在append是没有处理这个数据，导致后续该项为0.

common.h Path中加入tids_

Path中tids_改回来，在contains使用的时候强行置为0

iterator.h 有两处修改

测试修改的文件
scheduler.cpp
state_machine.cpp
runtime_context.h
