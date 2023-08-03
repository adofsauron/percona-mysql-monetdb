
start transaction;

--alter table region drop primary key;
--alter table nation drop primary key;
--alter table part drop primary key;
--alter table supplier drop primary key;
--alter table partsupp drop primary key;
--alter table orders drop primary key;
--alter table lineitem drop primary key;
--alter table customer drop primary key;


-- for table region
alter table region set read only;

-- for table nation
alter table nation set read only;

-- for table part
alter table part set read only;

-- for table supplier
alter table supplier set read only;

-- for table partsupp
alter table partsupp set read only;

-- for table customer
alter table customer set read only;

-- for table orders
alter table orders set read only;
create ordered index order_idx on orders(o_totalprice);

-- for table lineitem
alter table lineitem set read only;

commit;
