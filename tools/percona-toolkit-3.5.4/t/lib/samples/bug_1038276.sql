DROP DATABASE IF EXISTS bug_1038276;
CREATE DATABASE bug_1038276;
USE bug_1038276;
CREATE TABLE lt (
  id int not null auto_increment primary key,
  b  varchar(300)
) ENGINE=InnoDB;
CREATE TABLE rt (
  id int not null auto_increment primary key,
  b  varchar(300)
) ENGINE=InnoDB;
INSERT INTO lt VALUES (null, "0x89504E470D0A1A0A0000000D4948445200000079000000750802000000E55AD965000000097048597300000EC300000EC301C76FA8640000200049444154789C4CBB7794246779FFBBF78F7B7EBE466177677772CE3D9D667AA67BA62776CE39545557CE3974EE9EB049AB9556392210414258083"); 
