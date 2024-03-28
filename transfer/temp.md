### shell start
```bash
python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph
```

### query
```sql
explain MATCH p = (acc:Account {id:2251799813685615})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 90 AND
last(getMemberProp(e1, 'timestamp')) < 120 AND
e2.timestamp > 90 AND
e2.timestamp < 120 AND
medium.isBlocked = true
RETURN DISTINCT other.id as otherId
ORDER BY otherId;
```

```
throw lgraph::CypherException("OK");
throw lgraph::CypherException(temp);
throw lgraph::CypherException(std::to_string((int)(op_filter->filter_->Type())));
```


make install -j48
lgraph_server -d restart

python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph


MATCH (p:Person {id:32025})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
(other:Account)
RETURN other;


### cr1
MATCH p = (acc:Account {id:2251799813685615})-[e1:transfer *1..3]->(other:Account)<-
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

  "operationResult" : [ {
    "otherId" : 152840912353886323,
    "accountDistance" : 2,
    "mediumId" : 43980465155202,
    "mediumType" : "NFC"
  }, {
    "otherId" : 1125899906850268,
    "accountDistance" : 3,
    "mediumId" : 13194139617169,
    "mediumType" : "IPv6"
  } ]


### cr2
MATCH (p:Person {id:32025})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
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
MATCH (person:Person {id:32025})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*1..3]->(dst:Account)
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
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;


<!-- two hops -->
[4743416582405895609,4865576722298321633,4687403336918373745]

<!-- where not exec -->
match p=(src:Account {id:4743416582405895609})-[e:transfer*2..2]->(dst:Account)
WHERE isAsc(getMemberProp(e, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC
WHERE hasDuplicates(path)=false
RETURN path;

match p=(src:Account {id:4743416582405895609})-[e:transfer*2..2]->(dst:Account)
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
MATCH (p1:Person {id:76694})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
WITH DISTINCT loan
WITH sum(loan.loanAmount) as sumLoanAmount, count(distinct loan) as numLoans
RETURN round(sumLoanAmount * 1000) / 1000 as sumLoanAmount, numLoans;


MATCH p=(p1:Person {id:76694})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WITH DISTINCT getMemberProp(nodes(p), 'id') as path
RETURN path;

  "operationResult" : [ {
    "sumLoanAmount" : 3.8567769E7,
    "numLoans" : 1
  } ]

( ( {isasc(false,getMemberProp(false,e1,timestamp)) = true} && {head(false,getMemberProp(false,e1,timestamp)) > 90} ) && {last(false,getMemberProp(false,e1,timestamp)) < 120})


lgraph_import -c /root/scripts/import.conf --overwrite 1 \
    --dir /root/lgraph_db_sf1 --delimiter "|" --v3 0


1. 调整程序结构，realconsume
2. dfsstate 加上count，用于记录一个点的邻居数量
3. 如何取timestamp，参考GetMemberProp实现



match (n:Account )<-[e:transfer *2..2]-(n2:Account) 
with n.id as id, count(distinct n2) as cnt1
match (n:Account )<-[:transfer]-(:Account)<-[:transfer]-(n3:Account)
with n.id as id, count(distinct n3) as cnt2, cnt1
where cnt1<>cnt2
return id, cnt1, cnt2 limit 10;

match (n:Account )<-[e:transfer *2..2]-(n2:Account) return n.id, count(n2) as cnt order by cnt desc limit 10;

match (n:Account {id:4832081474947659562})<-[:transfer]-(:Account)<-[:transfer]-(n2:Account) return n.id, count(n2) as cnt order by cnt desc limit 10;


### test
match (n:Account {id:4832081474947659562})<-[:transfer]-(:Account)<-[:transfer]-(n2:Account) return n.id, count(n2);

match (n:Account {id:4832081474947659562})<-[e:transfer *2..2]-(n2:Account) return n.id,count(n2);


### test
match (n:Account {id:4832081474947659562})<-[:transfer]-(mid:Account)<-[:transfer]-(n2:Account {id:130323738850760202}) return n.id, n2.id;

match (n:Account {id:4832081474947659562})<-[e:transfer *2..2]-(n2:Account {id:130323738850760202}) return n.id, n2.id;


match (n:Account {id:4832081474947659562})<-[e1:transfer]-(mid:Account)<-[e2:transfer]-(n2:Account {id:130323738850760202}) return n.id, e1, mid.id, e2, n2.id;