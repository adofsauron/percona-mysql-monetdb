
EXPLAIN format=tree
SELECT b.b1,x.c1 FROM b 
LEFT JOIN 
(
    SELECT c1 FROM c INNER JOIN a ON a.a1 = c.c1 and a.a1 >= 1 and a.a1 < 4 and c.c1 >= 1 and c.c1 <= 3
) x
ON b.b1 = x.c1 \G

