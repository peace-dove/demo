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


explain MATCH p = (acc:Account {id:2251799813685615})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(getMemberProp(e1, 'timestamp'))=true AND
head(getMemberProp(e1, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
RETURN DISTINCT other.id as otherId
ORDER BY otherId;


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

( ( {isasc(false,getMemberProp(false,e1,timestamp)) = true} && {head(false,getMemberProp(false,e1,timestamp)) > 90} ) && {last(false,getMemberProp(false,e1,timestamp)) < 120})


lgraph_import -c /root/scripts/import.conf --overwrite 1 \
    --dir /root/lgraph_db_sf1 --delimiter "|" --v3 0


1. 调整程序结构，realconsume
2. dfsstate 加上count，用于记录一个点的邻居数量
3. 如何取timestamp，参考GetMemberProp实现