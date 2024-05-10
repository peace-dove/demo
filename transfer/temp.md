### shell start
```bash
python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph
```

```
throw lgraph::CypherException("OK");
throw lgraph::CypherException(temp);
throw lgraph::CypherException(std::to_string((int)(op_filter->filter_->Type())));
```

### Test Code
cypher 8 9 11 21

input id for cr1 use
2251799813685615
4614220667607332302
4614782243171217523


MATCH p = (acc:Account {id:2251799813685615})-[*1..2]->() return count(p);
MATCH p = (acc:Account {id:4614220667607332302})-[*1..2]->() return count(p);
MATCH p = (acc:Account {id:4614782243171217523})-[*1..2]->() return count(p);

MATCH p = (acc:Account {id:2251799813685615})-[*2..2]->() return count(p);
MATCH p = (acc:Account {id:4614220667607332302})-[*2..2]->() return count(p);
MATCH p = (acc:Account {id:4614220667607332302})-[*0..1]->()-[]->() return count(p);

MATCH p = (acc:Account {id:2251799813685615})-[]->()-[*0..1]->(other) return other;
MATCH p = (acc:Account {id:4614220667607332302})-[]->()-[*0..1]->(other) return other;

MATCH p = (acc{id:4614220667607332302})-[*0..1]->()-[]->() RETURN count(p);

personid: 32025
MATCH (n:Company)<-[:invest*0..3]-(m:Person{id:32025}) RETURN n,m;
MATCH (n:Account {id:4614220667607332302})-[*2]-(m:Account) RETURN DISTINCT m.email ORDER BY m.email;

8

#12 4924.1 Expected equality of these values:
#12 4924.1   ctx->result_->Size()
#12 4924.1     Which is: 6
#12 4924.1   check[i]
#12 4924.1     Which is: 7
#12 4924.1 test/test_cypher.cpp:125: Failure
#12 4924.1 Expected equality of these values:
#12 4924.1   ctx->result_->Size()
#12 4924.1     Which is: 9
#12 4924.1   check[i]
#12 4924.1     Which is: 13
#12 4924.1 test/test_cypher.cpp:125: Failure
#12 4924.1 Expected equality of these values:
#12 4924.1   ctx->result_->Size()
#12 4924.1     Which is: 5
#12 4924.1   check[i]
#12 4924.1     Which is: 13
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 5
#12 4924.2   check[i]
#12 4924.2     Which is: 6
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 4
#12 4924.2   check[i]
#12 4924.2     Which is: 15
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 4
#12 4924.2   check[i]
#12 4924.2     Which is: 7
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 4
#12 4924.2   check[i]
#12 4924.2     Which is: 6
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 1
#12 4924.2   check[i]
#12 4924.2     Which is: 3
#12 4924.2 test/test_cypher.cpp:125: Failure
#12 4924.2 Expected equality of these values:
#12 4924.2   ctx->result_->Size()
#12 4924.2     Which is: 1
#12 4924.2   check[i]
#12 4924.2     Which is: 12


MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..]->(n)-[:ACTED_IN]->(m) RETURN n,m
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*1..5]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*1..2]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*2..5]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*1..]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*2..]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..2]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..3]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*1]->(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*2]->(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[*..]->(n) RETURN DISTINCT n,n.name,n.title
MATCH (van:Person {name:'Vanessa Redgrave'})-[*3]->(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[*2..]->(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[]->()-[*0]->(m) RETURN m
MATCH (van:Person {name:'Vanessa Redgrave'})-[]->()-[*0..1]->(m) RETURN DISTINCT m
MATCH (mic:Person {name:'Michael Redgrave'})-[]->()-[*0..1]->(m) RETURN DISTINCT m
MATCH (jem:Person {name:'Jemma Redgrave'})<-[:HAS_CHILD*..]-(a) RETURN a
MATCH (jem:Person {name:'Jemma Redgrave'})<-[:HAS_CHILD*..]-(a)-[:ACTED_IN*..]->(m) RETURN a,m
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD|MARRIED*..]->(n) RETURN DISTINCT n.name
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD|MARRIED*1..2]->(n) RETURN DISTINCT n.name
MATCH (liam:Person {name:'Liam Neeson'})<-[:HAS_CHILD|MARRIED*1..3]-(a) RETURN DISTINCT a.name
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..]-(n) RETURN n
MATCH (roy:Person {name:'Roy Redgrave'})-[:HAS_CHILD*..]-(n) RETURN DISTINCT n
MATCH (jem:Person {name:'Jemma Redgrave'})-[:HAS_CHILD*..]-(a) RETURN a
MATCH (jem:Person {name:'Jemma Redgrave'})-[:HAS_CHILD*..]-(a) RETURN DISTINCT a
MATCH (van:Person {name:'Vanessa Redgrave'})-[:HAS_CHILD*..]-(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[:HAS_CHILD*..]-(n) RETURN DISTINCT n
MATCH (van:Person {name:'Vanessa Redgrave'})-[:HAS_CHILD*1..2]-(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[:HAS_CHILD*2]-(n) RETURN n
MATCH (van:Person {name:'Vanessa Redgrave'})-[:HAS_CHILD*2..]-(n) RETURN n
MATCH (n:Person)-[:BORN_IN*0..]->(m) RETURN n.name,m.name
MATCH (n:Person)-[:BORN_IN*0..]->(m:City) RETURN n.name,m.name
MATCH (n:Person)-[:BORN_IN*0..]->(m:Person) RETURN n.name,m.name
MATCH (n:Film)<-[:ACTED_IN*0..]-(m) RETURN n.title,n,m
MATCH (n:Film)<-[:ACTED_IN*0..]-(m:Person) RETURN n.title,n,m
MATCH (n:Film)<-[:ACTED_IN*0..]-(m:Film) RETURN n.title,n,m
MATCH (n:Film)<-[:ACTED_IN*0..]-(m:City) RETURN n.title,n,m

9

#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 0
#12 4925.6   check[i]
#12 4925.6     Which is: 1
#12 4925.6 test/test_cypher.cpp:125: Failure
#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 5
#12 4925.6   check[i]
#12 4925.6     Which is: 6
#12 4925.6 test/test_cypher.cpp:125: Failure
#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 3
#12 4925.6   check[i]
#12 4925.6     Which is: 4
#12 4925.6 test/test_cypher.cpp:125: Failure
#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 4
#12 4925.6   check[i]
#12 4925.6     Which is: 5
#12 4925.6 test/test_cypher.cpp:125: Failure
#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 2
#12 4925.6   check[i]
#12 4925.6     Which is: 3
#12 4925.6 test/test_cypher.cpp:125: Failure
#12 4925.6 Expected equality of these values:
#12 4925.6   ctx->result_->Size()
#12 4925.6     Which is: 9
#12 4925.6   check[i]
#12 4925.6     Which is: 10

From this
MATCH (n1:Person {name:'Liam Neeson'})-[*3]->(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})-[*..]->(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})-[*2..3]->(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})-[:MARRIED*..]->(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})-[:MARRIED|ACTED_IN*..]->(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})-[:MARRIED|ACTED_IN*2..]->(n2) RETURN n2
MATCH (n1:Person {name:'Michael Redgrave'})-[*3]->(n2) RETURN n2
To this
MATCH (n1:Person {name:'Liam Neeson'})<-[*2]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[*4]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[*..]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[*..]-(n2) RETURN DISTINCT n2
MATCH (n1:Person {name:'Liam Neeson'})<-[:MARRIED*..]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[:MARRIED|HAS_CHILD*..]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[:MARRIED|HAS_CHILD*2..4]-(n2) RETURN n2
MATCH (n1:Person {name:'Liam Neeson'})<-[:MARRIED|HAS_CHILD*2..4]-(n2) RETURN DISTINCT n2

11
#12 4929.0 Expected equality of these values:
#12 4929.0   ctx->result_->Size()
#12 4929.0     Which is: 17
#12 4929.0   check[i]
#12 4929.0     Which is: 20
#12 4929.1 test/test_cypher.cpp:125: Failure
#12 4929.1 Expected equality of these values:
#12 4929.1   ctx->result_->Size()
#12 4929.1     Which is: 34
#12 4929.1   check[i]
#12 4929.1     Which is: 51


MATCH p = (n {name:'Rachel Kempson'})-[*1..2]->() RETURN p
MATCH p = (n {name:'Rachel Kempson'})<-[*1..2]-() RETURN p
MATCH p = (n {name:'Rachel Kempson'})-[*1..2]-() RETURN p this
MATCH p = (n {name:'Rachel Kempson'})-[*0..1]-() RETURN p
MATCH p = (n {name:'Rachel Kempson'})-[*2..3]-() RETURN p this
MATCH p = (n {name:'Rachel Kempson'})-[*0..1]->()-[]->() RETURN p
MATCH p = (n {name:'Rachel Kempson'})-[*0..3]->() RETURN p,length(p)
MATCH (n {name:'Rachel Kempson'}) RETURN exists((n)-[*3]->())
MATCH (n {name:'Rachel Kempson'}) RETURN exists((n)-[:MARRIED*3]->())
MATCH (n {name:'Rachel Kempson'}) RETURN exists((n)-[:MARRIED|HAS_CHILD*3]->())

21

use this
MATCH (m:Account {id:1688849860276589})-[:transfer*2]-(n:Account) RETURN count(n);
MATCH (m:Account {id:2533274790404746})-[:transfer*3]-(n:Account) RETURN count(n);

MATCH (m:Person)-[:guarantee*2]-(n:Person) RETURN count(n);
MATCH (m:Account{id:6474749098079904})-[:withdraw*3]-(n:Account) RETURN count(n);


#12 5061.6 Expected equality of these values:
#12 5061.6   ctx->result_->Size()
#12 5061.6     Which is: 7
#12 5061.6   check[i]
#12 5061.6     Which is: 12
#12 5061.6 test/test_cypher.cpp:125: Failure
#12 5061.6 Expected equality of these values:
#12 5061.6   ctx->result_->Size()
#12 5061.6     Which is: 5
#12 5061.6   check[i]
#12 5061.6     Which is: 8
#12 5061.7 test/test_cypher.cpp:125: Failure
#12 5061.7 Expected equality of these values:
#12 5061.7   ctx->result_->Size()
#12 5061.7     Which is: 5
#12 5061.7   check[i]
#12 5061.7     Which is: 8
#12 5061.7 test/test_cypher.cpp:125: Failure
#12 5061.7 Expected equality of these values:
#12 5061.7   ctx->result_->Size()
#12 5061.7     Which is: 3
#12 5061.7   check[i]
#12 5061.7     Which is: 6

match (:Person {name:'Vanessa Redgrave'})<-[:HAS_CHILD]-(p)-[:ACTED_IN*0..]->(m) return p.name,m order by p.name
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN m.name this
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN DISTINCT m.name this
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN DISTINCT m.name ORDER BY m.name this
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN DISTINCT m.name ORDER BY m.name LIMIT 5
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN DISTINCT m.name ORDER BY m.name SKIP 2 this
MATCH (n:Person {name:'Vanessa Redgrave'})-[*2]-(m:Person) RETURN DISTINCT m.name ORDER BY m.name SKIP 2 LIMIT 3


### cr1
sf1 2251799813685615
sf10 4626605016826793378
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

Produce Results                                                    
  Distinct                                           
    Expand(All) [other <-- medium EdgeFilter e2 & medium]
      Filter [ isAsc && head && last ]
        Variable Length Expand(All) [acc -->*1..3 other]
          Node Index Seek [acc]  IN []

Produce Results                                                    
  Distinct                                           
    Expand(All) [other <-- medium EdgeFilter e2 & medium]
      Variable Length Expand(All) [acc -->*1..3 other VarLenEdgeFilter isAsc && head && last]
        Node Index Seek [acc]  IN []

Produce Results                       
  Sort [{<3>: 1:1, 0:1, 2:1}]                               
    Distinct                                           
      Project [ ... ]
        Filter [ Filter... ]
          Expand(All) [other <-- medium EdgeFilter...]
            Variable Length Expand(All) [acc -->*1..3 other VarLenEdgeFilter...]
              Node Index Seek [acc]  IN []


Produce Results
  Aggregate [count(acc)]
    Variable Length Expand(All) [acc -->*1..3 other VarLenEdgeFilter...]
      Node Index Seek [acc]  IN []

MATCH p = (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
with distinct other.id as id
RETURN DISTINCT count(id);

MATCH p = (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;


MATCH p = (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
WITH getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

MATCH p = (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640
WITH getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

MATCH p = (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)
WHERE head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640
WITH getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

<!-- pure var len expand -->
<!-- 3hop -->
MATCH (acc:Account {id:4626605016826793378})-[e1:transfer *1..3]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640
RETURN count(acc);

<!-- 4hop -->
MATCH (acc:Account {id:4626605016826793378})-[e1:transfer *1..4]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640
RETURN count(acc);

<!-- 5hop -->
MATCH (acc:Account {id:4626605016826793378})-[e1:transfer *1..5]->(other:Account)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640
RETURN count(acc);

检查什么时候返回
stack path path_unique同步

<!-- check asc, use this -->
MATCH p = (acc:Account {id:8725999155939212})-[e1:transfer *2..2]->(other:Account)
WHERE isDesc(getMemberProp(e1, 'timestamp'))=true
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
order by len desc
RETURN path;

### cr2
MATCH (p:Person {id:572153})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
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

|    | Produce Results                                                                                                                                                                                                                                                                                                |
|    |     Sort [{<2>: 1:0, 0:1}]                                                                                                                                                                                                                                                                                     |
|    |         Project [otherId,sumLoanAmount,sumLoanBalance]                                                                                                                                                                                                                                                         |
|    |             Apply                                                                                                                                                                                                                                                                                              |
|    |                 Argument [otherId,sumLoanAmount,sumLoanBalance]                                                                                                                                                                                                                                                |
|    |                 Aggregate [otherId,sumLoanAmount,sumLoanBalance]                                                                                                                                                                                                                                               |
|    |                     Apply                                                                                                                                                                                                                                                                                      |
|    |                         Argument [otherId,loanAmount,loanBalance]                                                                                                                                                                                                                                              |
|    |                         Distinct                                                                                                                                                                                                                                                                               |
|    |                             Project [otherId,loanAmount,loanBalance]                                                                                                                                                                                                                                           |
|    |                                 Apply                                                                                                                                                                                                                                                                          |
|    |                                     Expand(All) [other <-- loan EdgeFilter (({e3.timestamp > 1627020616747}&&{e3.timestamp < 1669690342640}))]                                                                                                                                                                 |
|    |                                         Argument [other]                                                                                                                                                                                                                                                       |
|    |                                     Distinct                                                                                                                                                                                                                                                                   |
|    |                                         Project [other]                                                                                                                                                                                                                                                        |
|    |                                             Variable Length Expand(All) [acc <--*1..3 other VarLenEdgeFilter ((({isdesc(false,getmemberprop(false,e2,timestamp)) = true}&&{head(false,getmemberprop(false,e2,timestamp)) < 1669690342640})&&{last(false,getmemberprop(false,e2,timestamp)) > 1627020616747}))] |
|    |                                                 Expand(All) [p --> acc ]                                                                                                                                                                                                                                       |
|    |                                                     Node Index Seek [p]  IN []

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

MATCH (person:Person {id:524333})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*1..3]->(dst:Account)
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true AND
head(getMemberProp(e2, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e2, 'timestamp')) < 1669690342640
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path;

|    | Produce Results                                                                                                                                                                                                                                                                                 |
|    |     Project [path]                                                                                                                                                                                                                                                                              |
|    |         Apply                                                                                                                                                                                                                                                                                   |
|    |             Argument [path,len]                                                                                                                                                                                                                                                                 |
|    |             Sort [{<1>: 1:0}]                                                                                                                                                                                                                                                                   |
|    |                 Distinct                                                                                                                                                                                                                                                                        |
|    |                     Filter [{hasduplicates(false,path) = false}]                                                                                                                                                                                                                                |
|    |                         Project [path,len]                                                                                                                                                                                                                                                      |
|    |                             Apply                                                                                                                                                                                                                                                               |
|    |                                 Variable Length Expand(All) [src -->*1..3 dst VarLenEdgeFilter ((({isasc(false,getmemberprop(false,e2,timestamp)) = true}&&{head(false,getmemberprop(false,e2,timestamp)) > 1627020616747})&&{last(false,getmemberprop(false,e2,timestamp)) < 1669690342640}))] |
|    |                                     Argument [src]                                                                                                                                                                                                                                              |
|    |                                 Project [src]                                                                                                                                                                                                                                                   |
|    |                                     Expand(All) [person --> src ]                                                                                                                                                                                                                               |
|    |                                         Node Index Seek [person]  IN []

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

MATCH (p1:Person {id:256946})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
WITH DISTINCT loan
WITH sum(loan.loanAmount) as sumLoanAmount, count(distinct loan) as numLoans
RETURN round(sumLoanAmount * 1000) / 1000 as sumLoanAmount, numLoans;

|    | Produce Results                                                                                                                                                                                                                                  |
|    |     Project [sumLoanAmount,numLoans]                                                                                                                                                                                                             |
|    |         Apply                                                                                                                                                                                                                                    |
|    |             Argument [sumLoanAmount,numLoans]                                                                                                                                                                                                    |
|    |             Aggregate [sumLoanAmount,numLoans]                                                                                                                                                                                                   |
|    |                 Apply                                                                                                                                                                                                                            |
|    |                     Argument [loan]                                                                                                                                                                                                              |
|    |                     Distinct                                                                                                                                                                                                                     |
|    |                         Project [loan]                                                                                                                                                                                                           |
|    |                             Expand(All) [pN --> loan ]                                                                                                                                                                                           |
|    |                                 Variable Length Expand(All) [p1 -->*1..5 pN VarLenEdgeFilter (({mininlist(false,getmemberprop(false,edge,timestamp)) > 1627020616747}&&{maxinlist(false,getmemberprop(false,edge,timestamp)) < 1669690342640}))] |
|    |                                     Node Index Seek [p1]  IN []

MATCH (p1:Person {id:256946})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
return count(loan);

MATCH (p1:Person {id:375587})-[edge:guarantee*1..5]->(pN:Person)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
return count(p1);

MATCH (p1:Person {id:375587})-[edge:guarantee*1..5]->(pN:Person)
return count(p1);



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

4.12
寻找max时间，分析原因

注意使用benchmark的数据测试，先跑出自己实现的结果my1 my2 my5 my11
再找到其中开销最大的时间


after.txt是运行自己的实现
old.txt是原来的实现

主要问题在cr1，max运行时间优化很少
cr1

edgecount pathcount spacecount
10936 5699 592310
2689 1493 154932
10382 6133 637548

after1.txt
4626888965704655708 0.2058720588684082
4626329589164019088 0.25562000274658203
12669672486865881 time spent: 0.35648155212402344
4622108564024989084 time spent: 0.5616426467895508
4626605016826793378 time spent: 0.6526474952697754 44s

old1
4626888965704655708 0.21819281578063965
4626329589164019088 0.2729661464691162
0.5249123573303223 12669672486865881 time spent: 0.48700475692749023
0.5794200897216797 4622108564024989084 time spent: 0.5761604309082031 17s
0.7588093280792236 4626605016826793378 time spent: 0.7418525218963623 67s

edgecount pathcount spacecount resultpathcount 228
21594 17458 1716980
3457 2958 297606
15308 12992 1303782

cr2
edgecount pathcount
2196 855
2754 978
29752 17382

after2.txt
0.08237075805664062 58605 time spent: 0.08237075805664062
0.08308672904968262 343909 time spent: 0.08308672904968262
0.9110314846038818 572153 time spent: 0.9110314846038818

old2
0.07891702651977539 58605 time spent: 0.07891702651977539
0.08574938774108887 343909 time spent: 0.08574938774108887
0.9342560768127441 572153 time spent: 0.9342560768127441

edgecount pathcount
5994 4714
4413 3648
35698 29885


cr5
edgecount pathcount
5088 3401
8705 4121
9104 4936

after5
0.1695249080657959 261070 time spent: 0.1695249080657959
0.2139873504638672 174552 time spent: 0.2139873504638672
0.257296085357666 524333 time spent: 0.257296085357666

old5
0.1698470115661621 261070 time spent: 0.1698470115661621
0.20034217834472656 174552 time spent: 0.20034217834472656
0.2596423625946045 524333 time spent: 0.2596423625946045

edgecount pathcount
6626 5602
14113 12079
16726 13176



cr11

edgecount pathcount
11 9
13 13
14 13

after11
0.0008559226989746094 459442 time spent: 0.0008559226989746094
0.0008862018585205078 314024 time spent: 0.0008862018585205078
0.0009596347808837891 188648 time spent: 0.0009596347808837891

old11
0.0008983612060546875 459442 time spent: 0.0008983612060546875
0.001005411148071289 314024 time spent: 0.001005411148071289
0.0010445117950439453 188648 time spent: 0.0010445117950439453

edgecount pathcount
32 11
46 13
53 16












4.17
空间开销指标评估思路
    重写allocator
