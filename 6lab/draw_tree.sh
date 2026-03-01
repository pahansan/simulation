#!/bin/bash
docker exec --interactive --tty neo4j cypher-shell -u neo4j -p password "
MATCH (n) DETACH DELETE n;
CREATE (:Node {Id: 1});
CREATE (:Node {Id: 2});
CREATE (:Node {Id: 3});
CREATE (:Node {Id: 4});
CREATE (:Node {Id: 5});
CREATE (:Node {Id: 6});
CREATE (:Node {Id: 7});
CREATE (:Node {Id: 8});
CREATE (:Node {Id: 9});
CREATE (:Node {Id: 10});
MATCH (a:Node {Id: 9}), (b:Node {Id: 2}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 2}), (b:Node {Id: 8}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 8}), (b:Node {Id: 6}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 6}), (b:Node {Id: 3}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 8}), (b:Node {Id: 1}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 3}), (b:Node {Id: 10}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 3}), (b:Node {Id: 7}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 7}), (b:Node {Id: 5}) MERGE (a)-[:CONNECTED]->(b);
MATCH (a:Node {Id: 1}), (b:Node {Id: 4}) MERGE (a)-[:CONNECTED]->(b);
"
