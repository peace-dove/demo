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
MATCH p = (acc:Account {id:4616471093031481453})-[e1:transfer *1..3]->(other:Account)<-
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

MATCH p = (acc:Account {id:8725999155939212})-[e1:transfer *1..3]->(other:Account)
RETURN other.id;

检查什么时候返回
stack path path_unique同步

my: 0.005307197570800781
new: 0.0105

my: 1.5668585300445557 0.0166

<!-- use this -->
MATCH p = (acc:Account {id:8725999155939212})-[e1:transfer *2..2]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

MATCH (acc:Account {id:8725999155939212})-[e1:transfer]->(mid:Account {id:171700010671408982})-[e2:transfer]->(other:Account)
WHERE e1.timestamp < e2.timestamp
return e1.timestamp, e2.timestamp, other.id;

+----+-----------------------------------------------------------+
| id | path                                                      |
+----+-----------------------------------------------------------+
| 0  | [8725999155939212,255861028707897460,202099308156163676]  |
| 1  | [8725999155939212,255861028707897460,4894570019289042920] |
| 2  | [8725999155939212,255861028707897460,226306156153278388]  |
| 3  | [8725999155939212,255861028707897460,4770158079582933872] |
| 4  | [8725999155939212,255861028707897460,4838837973900333835] |
| 5  | [8725999155939212,171700010671408982,270497727496850066]  |
| 6  | [8725999155939212,171700010671408982,4865015421612329968] |
| 7  | [8725999155939212,171700010671408982,4871489071198774281] |
| 8  | [8725999155939212,171700010671408982,234187455501180702]  |
| 9  | [8725999155939212,171700010671408982,230247080705145324]  |
+----+-----------------------------------------------------------+


### cr2
MATCH (p:Person {id:19395})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
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

  "operationResult" : [ {
    "otherId" : 4872895071692797812,
    "sumLoanAmount" : 9.4103294E7,
    "sumLoanBalance" : 9.4103294E7
  }, {
    "otherId" : 4810688826961824290,
    "sumLoanAmount" : 4.0275519E7,
    "sumLoanBalance" : 4.0275519E7
  } ]

### cr5
MATCH (person:Person {id:39324})-[e1:own]->(src:Account)
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

<!-- use this -->
MATCH p=(src:Account {id:4743416582405895609})-[e2:transfer*2..2]->(dst:Account {id:4687403336918373745})
WHERE last(getMemberProp(e2, 'timestamp')) > 1659331936801
WITH relationships(p) as path, length(p) as len
ORDER BY len DESC 
RETURN path;

MATCH p=(src:Account {id:4743416582405895609})-[e2:transfer*2..2]->(dst:Account {id:4687403336918373745})
WITH relationships(p) as path, length(p) as len
ORDER BY len DESC 
RETURN path;

match (src:Account {id:4743416582405895609})-[e1:transfer]->(:Account)-[e2:transfer]->(dst:Account {id:4687403336918373745})
where e1.timestamp > 1662123596189
RETURN e1.timestamp, e2.timestamp;

match (src:Account {id:4743416582405895609})-[e1:transfer]->(:Account)-[e2:transfer]->(dst:Account {id:4687403336918373745})
RETURN e1.timestamp, e2.timestamp;

+----+---------------+---------------+
| id | e1.timestamp  | e2.timestamp  |
+----+---------------+---------------+
| 0  | 1664764621398 | 1659331936802 |
| 1  | 1662123596189 | 1659331936802 |
+----+---------------+---------------+

python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph

<!-- two hops -->
[4743416582405895609,4865576722298321633,4687403336918373745]
[4743416582405895609,174795960537326727,258112553643682103]

<!-- where not exec -->
match p=(src:Account {id:4743416582405895609})-[e:transfer*3..3]->(dst:Account)
WHERE isAsc(getMemberProp(e, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC
WHERE hasDuplicates(path)=false
RETURN path;

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

<!-- use this -->
match p=(src:Account {id:4743416582405895609})-[e1:transfer]->(:Account)-[e2:transfer]->(dst:Account {id:4687403336918373745})
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e1:transfer]->(:Account)-[e2:transfer]->(dst:Account {id:4687403336918373745})
where e1.timestamp < e2.timestamp
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e1:transfer]->(mid:Account {id:4865576722298321633})-[e2:transfer]->(dst:Account {id:4687403336918373745}) 
RETURN src.id, e1.timestamp, mid.id, e2.timestamp, dst.id;

match p=(src:Account {id:4743416582405895609})-[e1:transfer]->(mid:Account {id:174795960537326727})-[e2:transfer]->(dst:Account {id:258112553643682103}) 
RETURN src.id, e1.timestamp, mid.id, e2.timestamp, dst.id;

  "operationResult" : [ {
    "path" : [ 4743416582405895609, 4865576722298321633, 4802526327515135661 ]
  }, {
    "path" : [ 4743416582405895609, 4865576722298321633, 4878806046203723425 ]
  }, {
    "path" : [ 4743416582405895609, 174795960537326727 ]
  }, {
    "path" : [ 4743416582405895609, 4865576722298321633 ]
  }, {
    "path" : [ 4743416582405895609, 174795960537326727, 297237575406462040 ]
  } ]


### cr11

id: 31073 58403

MATCH (p1:Person {id:58403})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
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

  "operationResult" : [ {
    "sumLoanAmount" : 3.8567769E7,
    "numLoans" : 1
  } ]

( ( {isasc(false,getMemberProp(false,e1,timestamp)) = true} && {head(false,getMemberProp(false,e1,timestamp)) > 90} ) && {last(false,getMemberProp(false,e1,timestamp)) < 120})



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
