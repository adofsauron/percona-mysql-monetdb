
start transaction;
alter table nation drop constraint nation_fk1;
alter table supplier drop constraint supplier_fk1;
alter table customer drop constraint customer_fk1;
alter table partsupp drop constraint partsupp_fk1;
alter table partsupp drop constraint partsupp_fk2;
alter table orders drop constraint orders_fk1;
alter table lineitem drop constraint lineitem_fk1;
alter table lineitem drop constraint lineitem_fk2 ;
-- alter table lineitem drop constraint lineitem_suppkey ;
-- alter table lineitem drop constraint lineitem_partsuppkey ;

alter table region drop constraint region_pk ;
alter table nation drop constraint nation_pk ;
alter table supplier drop constraint supplier_pk ;
alter table customer drop constraint customer_pk ;
alter table part drop constraint part_pk ;
alter table partsupp drop constraint partsupp_pk ;

alter table orders drop constraint orders_pk ;
alter table lineitem drop constraint lineitem_pk ;

commit;
