echo "start cr1"
array1=("2251799813685615" "4614220667607332302" "4614782243171217523" "3378249476361720" "3659999330980836" "4615345742880441418")

for element in "${array1[@]}"
do
    python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph -s "MATCH p = (acc:Account {id:$element})-[e1:transfer *1..3]->(other:Account)<-
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
ORDER BY accountDistance, otherId, mediumId" | grep time
echo $element
done
echo "end cr1"


echo "start cr2"
array2=("37445" "66671" "32025" "19395" "58203")
for element in "${array2[@]}"
do
    python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph -s "MATCH (p:Person {id:$element})-[e1:own]->(acc:Account) <-[e2:transfer*1..3]-
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
ORDER BY sumLoanAmount DESC, otherId ASC" | grep time
done
echo "end cr2"


echo "start cr5"
array5=("28292" "37445" "66671" "32025" "19395" "43555")
for element in "${array5[@]}"
do
    python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph -s "MATCH (person:Person {id:$element})-[e1:own]->(src:Account)
WITH src
MATCH p=(src)-[e2:transfer*3..3]->(dst:Account)
WHERE isAsc(getMemberProp(e2, 'timestamp'))=true AND
head(getMemberProp(e2, 'timestamp')) > 1627020616747 AND
last(getMemberProp(e2, 'timestamp')) < 1669690342640
WITH DISTINCT getMemberProp(nodes(p), 'id') as path, length(p) as len
ORDER BY len DESC 
WHERE hasDuplicates(path)=false 
RETURN path" | grep time
done
echo "end cr5"


echo "start cr11"
array11=("66671" "42151" "30756" "76694" "8155")
for element in "${array11[@]}"
do
    python3 src/python/FMA_shell/lgraph_shell/lgraph_cypher.py -c /usr/local/etc/lgraph.json -u admin -P 73@TuGraph -s "MATCH (p1:Person {id:$element})-[edge:guarantee*1..5]->(pN:Person) -[:apply]->(loan:Loan)
WHERE minInList(getMemberProp(edge, 'timestamp')) > 1627020616747 AND
maxInList(getMemberProp(edge, 'timestamp')) < 1669690342640
WITH DISTINCT loan
WITH sum(loan.loanAmount) as sumLoanAmount, count(distinct loan) as numLoans
RETURN round(sumLoanAmount * 1000) / 1000 as sumLoanAmount, numLoans" | grep time
done

echo "end cr11"
