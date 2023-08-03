
mysql --local-infile=1


load data local infile '/tmp/region.tbl'    into table region       fields terminated by '|';
load data local infile '/tmp/nation.tbl'    into table nation       fields terminated by '|';
load data local infile '/tmp/part.tbl'      into table part         fields terminated by '|';
load data local infile '/tmp/supplier.tbl'  into table supplier     fields terminated by '|';
load data local infile '/tmp/partsupp.tbl'  into table partsupp     fields terminated by '|';
load data local infile '/tmp/customer.tbl'  into table customer     fields terminated by '|';
load data local infile '/tmp/orders.tbl'    into table orders       fields terminated by '|';
load data local infile '/tmp/lineitem.tbl'  into table lineitem     fields terminated by '|';

