
start transaction;

alter table region drop primary key;
alter table nation drop primary key;
alter table part drop primary key;
alter table supplier drop primary key;
alter table partsupp drop primary key;
alter table orders drop primary key;
alter table lineitem drop primary key;
alter table customer drop primary key;


-- for table region
alter table region
add constraint region_pk primary key (r_regionkey);

-- for table nation
alter table nation
add constraint nation_pk primary key (n_nationkey);

alter table nation
add constraint nation_fk1 foreign key (n_regionkey) references region;

-- for table part
alter table part
add constraint part_pk primary key (p_partkey);

-- for table supplier
alter table supplier
add constraint supplier_pk primary key (s_suppkey);

alter table supplier
add constraint supplier_fk1 foreign key (s_nationkey) references nation;

-- for table partsupp
alter table partsupp
add constraint partsupp_pk primary key (ps_partkey,ps_suppkey);

-- for table customer
alter table customer
add constraint customer_pk primary key (c_custkey);

alter table customer
add constraint customer_fk1 foreign key (c_nationkey) references nation;

-- for table lineitem
alter table lineitem
add constraint lineitem_pk primary key (l_orderkey,l_linenumber);

-- for table orders
alter table orders
add constraint orders_pk primary key (o_orderkey);

-- for table partsupp
alter table partsupp
add constraint partsupp_fk1 foreign key (ps_suppkey) references supplier;

alter table partsupp
add constraint partsupp_fk2 foreign key (ps_partkey) references part;

-- for table orders
alter table orders
add constraint orders_fk1 foreign key (o_custkey) references customer;

-- for table lineitem
alter table lineitem
add constraint lineitem_fk1 foreign key (l_orderkey) references orders;

alter table lineitem
add constraint lineitem_fk2 foreign key (l_partkey,l_suppkey) references
        partsupp;

commit;
