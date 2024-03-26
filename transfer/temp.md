### shell start
```bash
python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph
```

### query
```sql
explain MATCH p = (acc:Account {id:10})-[e1:transfer *1..3]->(other:Account)<-
[e2:signIn]-(medium)
WHERE isAsc(relationships(e1, 'timestamp'))=true AND
head(relationships(e1, 'timestamp')) > 90 AND
last(relationships(e1, 'timestamp')) < 120 AND
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
WHERE isAsc(relationships(e1, 'timestamp'))=true AND
head(relationships(e1, 'timestamp')) > 1627020616747 AND
last(relationships(e1, 'timestamp')) < 1669690342640 AND
e2.timestamp > 1627020616747 AND
e2.timestamp < 1669690342640 AND
medium.isBlocked = true
RETURN DISTINCT other.id as otherId
ORDER BY otherId;

( ( {isasc(false,relationships(false,e1,timestamp)) = true} && {head(false,relationships(false,e1,timestamp)) > 90} ) && {last(false,relationships(false,e1,timestamp)) < 120})


lgraph_import -c /root/scripts/import.conf --overwrite 1 \
    --dir /root/lgraph_db_sf1 --delimiter "|" --v3 0